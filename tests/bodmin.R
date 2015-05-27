##
## Example of a spatial point proccess
## Data from the splancs package
## The example tranfer the data to the DBMS
## as well a raster with a kernel estimate of the intensity function
##
## Last change: 20101017 Pedro

## 1. Getting started
require(aRT)
require(splancs)
data(bodmin)

## Kernel estimate from the intensity function of the
## point proccess performed within R using splancs
ker <- kernel2d(as.points(bodmin), bodmin$poly, h0=2, nx=100, ny=100)
image(ker, asp=1)

## 2. Transfering data and image (raster) to a DB
con=openConn(user="root", pas="art", db="mysql")
con

if(any(showDbs(con)=="bodmin"))
  deleteDb(con, "bodmin", force=T)

db = createDb(con, "bodmin")

## 2.1 Preparing and transfering the points
xy <- as.data.frame(bodmin[1:2])
coordinates(xy) <- ~x+y

l_points <- importSpData(db, xy, "coords")
## this avoids (encapsulates):
## a - converting to SpatialPointsDataFrame (with ID specification).
## b - createLayer()
## c - addpoints()
## d - createTable()

## 2.2 Preparing and transfering the image (raster)

## converting the kernel to "sp"
g <- cbind(expand.grid(x = ker$x, y = ker$y), as.vector(ker$z))
coordinates(g) <- c("x", "y")
gridded(g) <- TRUE

## plot from "sp"
image(g)

contour(ker, add = T)  
fullgrid(g)=TRUE

l_kernel <- createLayer(db, "kernel")
addRaster(l_kernel,g)

## Ploting from the database
plot(l_kernel)
plot(l_points, add=T)

## 3. (Optional) Setting visualisations for terraView
thp <- createTheme(l_points, "points", view="view")
thk <- createTheme(l_kernel, "raster", view="view")
setVisual(thk, visualRaster(color=terrain.colors(7)), mode="r")

