##
## The following script reproduces the example in the
## vignette "auckland" from the package \pkg{spdep}
## using \pkg{aRT} to transfer the data and some
## results to a TL DB
##
## Additionally this text ilustrates operation on polygon
## geometry such as extracting metrics and neighbourhoods
##
## Last change: 20101017 Pedro

##
## 1. Getting started
##
require(sp)
require(aRT)

aRTsilent(FALSE)

require(spdep)
data(auckland)

## results from a data-analysis using an \pkg{spdep} function
res <- probmap(auckland$Deaths.1977.85, 9*auckland$Under.5.1981)

## 2. Transfering to the DBMS

## 2.1 Preparing data: converting to \pkg{sp}
res = cbind(auckland, res)
res = cbind(data.frame(ID= paste(1:length(auckpolys)), res))

polygons = sapply(1:length(auckpolys), function(x)
{
  Polygons(list(Polygon(auckpolys[[x]])),ID=paste(x))
})
SP <- SpatialPolygons(polygons)

## 2.2 Transfering the the TL BD
con=openConn(user="root", pas="art", db="mysql")

if(any(showDbs(con) == "auckland"))
  deleteDb(con, "auckland", force=T)

db = createDb(con, "auckland")
db

l = createLayer(db, "districts")
addPolygons(l, SP)
t = createTable(l, "t_auck", gen=T)

updateColumns(t, res)
aRT::getData(t)[1:3,]

db

## plotting from the data-base
plot(l)

## 3. Preparing visualisations for TerraView

## a theme with the original counts
th=createTheme(l, "rawData")
thexpcount = createTheme(l,"texpcount",att = "expCount > 300")

plot(th)
plot(thexpcount,bor="red",add=T)

## and now "slicing the data" and adding a colorkey
## to the visualisation
v1=visualPolygons(color= c("#F7F7F7", "#CCCCCC", "#969696", "#636363", "#252525"), transp=0)
setVisual(th, v1, att="raw")

th=createTheme(l, "relativeRisk")
setVisual(th, v1, att="relRisk")

th=createTheme(l, "probabilityMap")
setVisual(th, v1, att="pmap")

th=createTheme(l, "expectedCounts")
setVisual(th, v1, att="expCount")

## and now reloading the data in TV we can see a visualisation
## of the data already there

## CAN I CHANGE A PRE-EXISTING VISUAL ON THE SAME THEME?
## == APPARENTLY YES == ANY RESTRICTIONS OR COMMENTS
## == IF NOT -- ISSUE AN ERROR MESSAGE WHEN TRYING!!!
# PEDRO: nao. tenho q colocar erro nisso, e talvez criar uma
# funcao do tipo removeVisual(theme)

th=createTheme(l, "expectedCounts4")
setVisual(th, visualPolygons(color=terrain.colors(10), transp=0), att="expCount")

## some options when "slicing" data for visualisation
## INCLUDE OPTIONS FOR SLICING HERE!
# PEDRO: use a opcao 'mode' (?setVisual para outras)
th=createTheme(l, "QuantilexpectedCounts")
setVisual(th, v1, att="expCount", mode="quantil")

## 4. Some operations on polygons

## 4.1 computing polygons attributes

## area and perimeter of each district
area = as.data.frame(getMetric(l, "area"))
area

perimeter = as.data.frame(getMetric(l, "length"))
perimeter

## adding area and perimeter to the polygon attribute table 
createColumn(t, "area")
updateColumns(t, area)
createColumn(t, "length")
updateColumns(t, perimeter)

head(aRT::getData(t))

## Operations on polygon geometry

## centroids
center = getOperation(l, "centroid")
center
## storing centroids in another layer
lcentroids = importSpData(db,center, "centroids")

## neighborhood, choosing the target
target = c("2","18","73")
colors = terrain.colors(5)
pols = getPolygons(l)
plot(pols, usePolypath=FALSE)
plot(pols[target,],col=colors[1],add=T, usePolypath=FALSE)

## Neighbours by touching,  1st and 2nd order
neigh = getRelation(l,"touches",ID=target)
neigh2 = getRelation(l,"touches",ID=neigh)

# nao funciona direito porque o dado nao esta certinho
# acho q na terralib tem uma variavel define um criterio
# de erro, entao tem que aumentar para passar aqui
# outra solucao eh usar o buffer, como nos vamos usar mais adiante
# o erro aqui pode ser tambem por causa da projecao (que nos nao estamos
# usando)
plot(pols[target,], usePolypath=FALSE)
plot(pols,add=T, usePolypath=FALSE)
plot(pols[neigh,],col=colors[2],add=T, usePolypath=FALSE)
plot(pols[neigh2,],col=colors[3],add=T, usePolypath=FALSE)
plot(pols[target,],col=colors[1],add=T, usePolypath=FALSE)

## Neighbours inside and crossing buffers
# (the buffer operation in the newest TerraLib is faster than this one)
# let us use a minor error (0.1) to define the neighbourhood criterion
buffer = getOperation(l,"buffer", dist=.1)
lb = importSpData(db,buffer,lname="buffer")

ov = getRelation(l,"overlaps",lb,ID=target)
ov2 = getRelation(l,"overlaps",lb,ID=ov)

plot(pols, usePolypath=FALSE)
plot(pols[ov2,],col=colors[3],add=T, usePolypath=FALSE)
plot(pols[ov,],col=colors[2],add=T, usePolypath=FALSE)
plot(pols[target,],col=colors[1],add=T, usePolypath=FALSE)

## Neighbours within a buffer radius (bigger than the error)
buffer2 = getOperation(l,"buffer", ID=target,dist=4)
lb2 = importSpData(db,buffer2,lname="buffer2")
plot(pols[target,],add=T, usePolypath=FALSE)
ov = getRelation(l,"overlaps",lb2,ID=target)

plot(pols,col="gray",lty=0, usePolypath=FALSE)
plot(pols[ov,],col=colors[2],add=T, usePolypath=FALSE)
plot(pols[target,],col=colors[1],add=T, usePolypath=FALSE)
plot(buffer2,add=T,bor="red",lty=2, usePolypath=FALSE)

rm(list=ls())
gc()

## connect to the DBMS
con=openConn(user="root", pas="art", db="mysql")

## checking for existing DB's
showDbs(con)

## connecting to a specific DB
db = openDb(con, "auckland")

## inspecting the data base contents
## inspecting geometry (ies)
db

## and their associates attribute tables
l = openLayer(db,"districts")
t = openTable(l, "t_auck")

## importing data
polygons = getPolygons(l)
plot(polygons, usePolypath=FALSE)
head(aRT::getData(t))

