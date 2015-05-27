# importing sp data into databases
# Last change: 20101017 Pedro

require(aRT)
require(geoR)

aRTsilent(FALSE)
data(ca20)

conn=openConn(user="root", pas="art", db="mysql")
if(any(showDbs(conn) == "ca20"))
  deleteDb(conn, "ca20", force=T)
db <- createDb(conn, "ca20")

# adding SpatialPoints (aRT will create the ID's)

xc = round(runif(10), 2)
yc = round(runif(10), 2)
xy = cbind(xc, yc)
xy.sp = SpatialPoints(xy)

rownames(xy.sp@coords) = paste(1:10)

l=importSpData(db, xy.sp, "lpoints0",tname="tpoints0")

# adding SpatialPointsDataFrame

ca20df <- as.data.frame(ca20)
head(ca20df)
class(ca20df) <- "data.frame"
coordinates(ca20df) <- c("east", "north")
ca20df$ID <- as.character(1:nrow(ca20df))

n=names(ca20df)
n[1]="data"
names(ca20df)=n

l=importSpData(db, ca20df, "lpoints1","tpoints1")

# adding SpatialPolygons

pol2sp <- function(i){
  pol <- as.matrix(ca20[c("reg1","reg2","reg3")][[i]])
  if(!isTRUE(all.equal(pol[1,], pol[nrow(pol),])))
    pol <- rbind(pol,pol[1,])
  Polygons(list(Polygon(pol)), as.character(i))
}
polysSP<- SpatialPolygons(sapply(1:3, pol2sp))

lpolygons = importSpData(db,polysSP,"lpolygons0","tpolygons0")

# adding SpatialPolygonsDataFrame

df=data.frame(a=1:3,b=4:6)
polysSPDF=SpatialPolygonsDataFrame(polysSP,df)

lpolygons = importSpData(db,polysSPDF,"lpolygons1","tpolygons1")

