##
## Creates a DB from a geodata object (data from the package geoR)
##
## Last change: 20110323 Pedro

## 1. Getting started

##
## 1.1 Loading packages
##
require(geoR)
require(aRT)

aRTsilent(FALSE)

##
## 1.2 Loading and visualising the (geo)data
##
data(parana)

##
## 1.3 Creating a connection with the DBMS (MySQL)
##
con <- openConn(u="root", pas="art", dbms="mysql")
con
## optional arguments allows choice of: host, port, user, password

##
## 1.4 querying the DBMS, deleting and creating (terralib) data-bases
##
showDbs(con)

pr <- createDb(con, "parana", replace=TRUE)
pr
showDbs(con)
##
## 2. populating the data-base
##

##
## 2.1 Layer 1: the data points
##

## creating the layer
l_dados <- createLayer(pr, "dados")
l_dados

## loading the point geometry

## converting to the sp format
Points = SpatialPoints(parana$coords)
PointsDF = SpatialPointsDataFrame(Points, data.frame(ID=paste(1:143)))
PointsDF[1:3,]

## loading the point coordinates to the DB
addPoints(l_dados, PointsDF)

## At this stage it is by terralib standards it is compulsory to create
## a table before being able to access the geometry data 
## creating a static table
## (others are documented in aRTtable)
t_dados = createTable(l_dados, "t_dados", gen=T)
t_dados

## current status of the DB
pr

## visualising point locations directly from the data-base
## (not using an R object)

# Now inserting the attributes associated to the points
artdata <- data.frame(id=paste(1:143), data=parana$data)
length(parana$data)
artdata[1:5,]
names(artdata)

createColumn(t_dados, "data", type="numeric")
updateColumns(t_dados, artdata)

## and check the current status of t_dados against the previous
t_dados

## PEDRO: please check this:
## Obs: all the call above are encapsulated by a single call
##      to ???? (importspData) ???


## Optional: Creating "views" and "themes" for a
## TL based application such as TerraView
tema_dados <- createTheme(l_dados, "estacoes", view = "view")
setVisual(tema_dados, visualPoints(pch=15, color="black", size=5))

## At this stage you can open terraview and visualise
## there the points and their attributes

##
## 2.2 Layer 2: the polygon with the border of the state
##

## Converting to "sp"
artpols <- SpatialPolygons(list(Polygons(list(Polygon(parana$borders)),"1")))

## create layers and loading the polygon (notice again the compulsory
## call to createTable() even if no attribute is to be loaded
l_pol <- createLayer(pr, "contorno")
addPolygons(l_pol, artpols)
createTable(l_pol, "t_pol")

## PEDRO: please check this:
## Obs: all the call above are encapsulated by a single call
##      to ???? (importspData) ???

## yet again checking the status of the DB
pr

## optional: visualisations for TV
tema_pol <- createTheme(l_pol, "contorno", view="view")
setVisual(tema_pol, visualPolygons(transp=100))

rm(list=ls())

aRTsilent(FALSE)

con <- openConn(u="root", pas="art", dbms="mysql")
con

showDbs(con)

pr <- openDb(con, "parana")
pr

l_dados <- openLayer(pr, "dados")
l_pol   <- openLayer(pr, "contorno")
t_dados <- openTheme(pr, "estacoes")
t_borda <- openTheme(pr, "contorno")
t_dados
t_borda

plot(t_dados)
plot(t_borda, add=TRUE)
box()

points  = getData(t_dados)
borders = getData(t_borda)

parana = as.geodata(points, data.col=2)
parana$borders = borders@polygons[[1]]@Polygons[[1]]@coords

## geoR stuff:
## Geostatistical parameter estimation and spatial prediction 
##  using geoR (could likelywise be other geostats package)
##
ml <- likfit(parana, trend="1st", ini=c(1000, 100))
gx <- seq(50,800, by=10)
gy <- seq(-100,650, by=10)
loc0 <- expand.grid(gx,gy)
#points(parana, bor=parana$borders)
#points(loc0, pch=".", col=2)
loc1 <- polygrid(loc0, bor=parana$bor)
#points(loc1, pch="+", col=4)

KC <- krige.control(trend.d="1st", trend.l="1st", obj=ml)
kc <- krige.conv(parana, loc=loc0, krige= KC, bor=parana$borders)

image(kc, col=terrain.colors(15))

coordinates(loc1)=~Var1+Var2
spixdf = SpatialPixelsDataFrame(loc1, data.frame(pred=kc$predict))

l_pred <- createLayer(pr, "pred")
addRaster(l_pred, spixdf)

## current status of the DB
pr

## Optional : setting a TV visualisation
th <- createTheme(l_pred, "raster", view="view")
setVisual(th, visualRaster(color = terrain.colors(15)), mode="r")

## and another visualisation option, subseting the data
th3=createTheme(l_dados,"maiores", att="data > 300", view="select")

## 3. Retrieving (importing) data from the DB to an R object
th3data <- getData(th3)
th3data[1:5,]
class(th3data)

## 4. Some plots
par(mfcol=c(1,2), mar=c(.5,.5,.5,.5))

plot(l_pred, col=terrain.colors(15))
plot(l_dados, add=T)
plot(l_pol, add=T)

plot(th3data, pch=19)
plot(l_pred, col=terrain.colors(15),add=T)
plot(l_pol,add=T)
plot(l_dados,add=T)
plot(th3data,add=T,pch=19)

