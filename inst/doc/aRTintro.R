
library(aRT)

aRTsilent(FALSE)

con <- openConn(user="root", pas="art")

if( any(showDbs(con) =="bodmin") ) deleteDb(con, "bodmin", 
  force=TRUE)

db <- createDb(con, db="bodmin")

require(splancs)
data(bodmin)
names(bodmin)
bodmin$poly = bodmin$poly[-1,]

raster <- kernel2d(as.points(bodmin), bodmin$poly, h0=2, 
  nx=100, ny=200)

g <- cbind(expand.grid(x = raster$x, y = raster$y), as.vector(raster$z))
coordinates(g) <- c("x", "y")
gridded(g) <- TRUE
fullgrid(g)=TRUE

layer.raster <- createLayer(db, l="raster")
addRaster(layer.raster, g)

lcells = createLayer(layer.raster, "cells", rx=10)
lcells

theme.raster<-createTheme(layer.raster, "raster", v="view")
setVisual(theme.raster, visualRaster(col = terrain.colors(20)), mode="raster")

