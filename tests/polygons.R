# Exemplo de realização de métricas de conjuntos
# usando o conjunto de dados pol3
# Last change: 20101017 Pedro

require(aRT)
data(pol3)

plot(pol3)

con=openConn(user="root", pas="art", db="mysql")
showDbs(con)

if(any(showDbs(con)=="pol3"))
	deleteDb(con, "pol3", force=T)

db <- createDb(con, "pol3")

l_pol <- createLayer(db, "poligonos")
addPolygons(l_pol, pol3)
t_pol <- createTable(l_pol,"table_pol")

polygons = getPolygons(l_pol)

par(mfcol=c(2,4), mar=c(.3,.3,.3,.3))

plot(polygons)

getOperation(l_pol, "buffer", d=5000)->buffer

plot(polygons, col="gray", lty=0)
plot(polygons["1",], add=T, col="green")
plot(buffer["1",], add=T, border="red", lty=2)

plot(polygons, col="gray", lty=0) 
plot(polygons["2",], add=T, col="green")
plot(buffer["2",], add=T, border="red", lty=2)

plot(polygons, col="gray", lty=0) 
plot(polygons["3",], add=T, col="green")
plot(buffer["3",], add=T, border="red", lty=2)

# calculando a uniao e interseccao de alguns conjuntos
getSetOperation(l_pol, "union", ID=c("2","1"))->datau
getSetOperation(l_pol, "inter", ID=c("2","3"))->datai
getSetOperation(l_pol, "diff", ID=c("2","3"))->datad
#getSetOperation(l_pol, "x", ID=c("2","3"))->datax

# inserindo estes resultados em um novo layer e
# calculando as suas cascas convexas
l2<-createLayer(db, "ui")

datau@polygons[[1]]@ID = "1"
datai@polygons[[1]]@ID = "2"

data=SpatialPolygons(c(datau@polygons, datai@polygons), 1:2)

addPolygons(l2, data)

createTable(l2,"table_ui")

getOperation(l2, "hull")->hull2

# plotando o resultado das operacoes juntamente
# com as suas cascas convexas
plot(polygons, col="gray", lty=0) 
plot(datau, col="yellow", add=T)
plot(hull2["1",], border="blue", add=T)

plot(polygons, col="gray", lty=0) 
plot(datai, col="orange", add=T)
plot(hull2["2",], border="blue", add=T)

plot(polygons, col="gray", lty=0) 
plot(datad, col="orange", add=T)

plot(polygons, col="gray", lty=0) 
#plot(datax, col="orange", add=T)
