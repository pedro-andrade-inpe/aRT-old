# Example of spatial relations using the three geometric types
# This example uses data from Recife city, in Brazil
#
# Last change: 20101017 Pedro

require(aRT)
aRTsilent(FALSE)

con=openConn()
#con=openConn(user="root", pas="art", db="mysql")
con

if(any(showDbs(con)=="recife"))
  deleteDb(con, "recife", force=T)

db = createDb(con, "recife")

ldistricts = createLayer(db, "districts")
lpoints    = createLayer(db, "points")
lsewers    = createLayer(db, "sewers")

data(recife.districts)
data(recife.points)
data(recife.sewers)

addPolygons(ldistricts, recife.districts)
addPoints  (lpoints,    recife.points)
addLines   (lsewers,    recife.sewers)

tdistricts = createTable(ldistricts, "t_districts")
tpoints    = createTable(lpoints,    "t_points")
tsewers    = createTable(lsewers,    "t_sewers")

# remove R objects and create them again
# to show that the data are really stored in the database
rm(list=ls())
invisible(gc())

con=openConn()
#con=openConn(user="root", pas="art", db="mysql")
bdrecife <- openDb(con, "recife")
showLayers(bdrecife)

ldistricts = openLayer(bdrecife, "districts")
lpoints    = openLayer(bdrecife, "points")
lsewers    = openLayer(bdrecife, "sewers")

geodistricts = getPolygons(ldistricts)
geopoints    = getPoints(lpoints)
geosewers    = getLines(lsewers)

par(mfcol=c(2,2), mar=c(.5,.5,.5,.5))

# plotting districts and points
plot(geodistricts, usePolypath=FALSE, col="red")
plot(geopoints, pch=19, add=T)
box()

# plotting sewers
plot(geodistricts, col="gray", lty=0)
plot(geosewers, col="blue", add=T)
box()

## Spatial relations
# 1. districts which contain points
getRelation(ldistricts, "contains", lpoints)->rel1
rel1

# 2. touching neighbours of the districts which contain points
getRelation(ldistricts, "touch", ID=rel1)->rel2
rel2

plot(geodistricts, col="gray", lty=0)
plot(geodistricts[rel1,], col="yellow", add=T)
plot(geodistricts[rel2,], col="green", add=T)
box()

# 3. sewers crossing or within the districts which contain points
getRelation(lsewers, c("crosses","within"), layer=ldistricts, ID=rel1)->rel3

plot(geodistricts[rel1,], col="gray", lty=0)
plot(geodistricts, col="gray", lty=0, add=T)
plot(geodistricts[rel1,], col="yellow", lty=0, add=T)

plot(geosewers, add=T)
plot(geosewers[rel3,], col="red", add=T)

# creating themes with the same visual of the R plots
th1 = createTheme(ldistricts,    "districts1",    view="districts")
th2 = createTheme(lpoints, "points1", view="districts")

setVisual(th1, visualPolygons(color="red", transp=0))
setVisual(th2, visualPoints(pch=15))

th3 = createTheme(ldistricts,  "districts2",  view="sewers")
th4 = createTheme(lsewers, "sewers2", view="sewers")

setVisual(th3, visualPolygons(color="gray", ccolor="gray", transp=0))
setVisual(th4, visualLines(color="blue"))

# TerraView does not support 24 IDs in a same query (string too long)
# So we split in two
th5a = createTheme(ldistricts, "vizinhos3a", view="vizinhos", ID=rel2[1:12])
th5b = createTheme(ldistricts, "vizinhos3b", view="vizinhos", ID=rel2[13:24])
th6  = createTheme(ldistricts, "contem3",   view="vizinhos", ID=rel1)
th7  = createTheme(ldistricts, "todos3",    view="vizinhos")

setVisual(th5a, visualPolygons(color="green", transp=0))
setVisual(th5b, visualPolygons(color="green", transp=0))
setVisual(th6, visualPolygons(color="yellow", transp=0))
setVisual(th7, visualPolygons(color="gray", ccolor="gray", transp=0))

v = c(0, 15, 30, 45, 60, 75, length(rel3))
for(i in 1:6)
{
  name = paste("cruzam", i, sep="")
  th8  = createTheme(lsewers, name, view="final", ID=rel3[(v[i]+1):v[i+1]])
  setVisual(th8,  visualLines(color="red"))
}

th9  = createTheme(ldistricts,  "vizinhos4",  view="final", ID=rel1)
th10 = createTheme(lsewers,     "sewers4",    view="final")
th11 = createTheme(ldistricts,  "districts4", view="final")

setVisual(th8,  visualLines(color="red"))
setVisual(th9,  visualPolygons(color="yellow", ccolor="yellow", transp=0))
setVisual(th10, visualLines(color="black"))
setVisual(th11, visualPolygons(color="gray", ccolor="gray", transp=0))

# see and check them using terraView
