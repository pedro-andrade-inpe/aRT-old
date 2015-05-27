# Last change: 20101017 Pedro

require(geoR)
require(aRT)

data(ca20)

Pols = list()
i = 1

for(reg in c("reg1","reg2","reg3"))
{
	mat=as.matrix(ca20[[reg]])
	if(reg != "reg1")
		p = Polygon(rbind(mat,mat[1,]))
	else
		p = Polygon(mat)

	Pols[[i]] = Polygons(list(p),paste(i))
	i = i + 1
}

SPolygons = SpatialPolygons(Pols)

Points = SpatialPoints(ca20$coords)

SPoints = SpatialPointsDataFrame(Points, data.frame(ID=paste(1:178)))

conn=openConn(user="root", pas="art", db="mysql")

if(any(showDbs(conn) == "ca20")) deleteDb(conn, "ca20", force=T)

db=createDb(conn, "ca20")

lpoints   = createLayer(db,"lpoints")
lpolygons = createLayer(db,"lpolygons")

addPoints(lpoints, SPoints)
addPolygons(lpolygons, SPolygons)

createTable(lpoints, "tpoints", gen=T)
createTable(lpolygons, "tpolygons", gen=T)

total = 0

for(i in 1:3)
{
	x = getRelation(lpoints, "within", lpolygons, ID=paste(i))
	total = total + length(x)
	cat(paste("Pontos no poligono", i, "=", length(x), "\n"))
}

th = createTheme(lpoints, "points",vi="view")
th = createTheme(lpolygons, "poligons", vi="view")

