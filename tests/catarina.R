# Exemplo de calculo de metricas espaciais
# utilizando o conjunto de dados catarina
# Last change: 20101017 Pedro

require(aRT)
aRTsilent(FALSE)

con=openConn(user="root", pas="art", db="mysql")
con
if(any(showDbs(con)=="catarina"))
  deleteDb(con, "catarina", force=T)

db<-createDb(con,"catarina")

data(catarina)

lcities = createLayer(db, "cities")
addPolygons(lcities, catarina)
t = createTable(lcities, "cities")

hull = getOperation(lcities, "hull")
center = getOperation(lcities, "centroid")
area = getMetric(lcities, "area")

# polígonos cuja area for maior que 1.100.000.000
ids = area$ID[which(area$area > 1.1*10^9)]
ids
# construindo area de plotagem 2x2
par(mfcol=c(2,2), mar=c(.5,.5,.5,.5))

# plota os municipios do estado
plot(catarina, col="red")
box()

# plotagem com o buffer dos maiores municipios
plot(catarina[ids,])
plot(catarina, col="gray", bor="white", add=T)
plot(hull[ids,],  col="green", add=T)
plot(catarina[ids,], col="gray", add=T)
box()

##center = getOperation(lcities, "centroid")
lcenter = createLayer(db, "center")
addPoints(lcenter, center)
t = createTable(lcenter,"center")

oesteids = getID(center[which(coordinates(center)[,1] < 193472),])
oesteids
# alguns poligonos escolhidos a dedo
ID=c("67","241","290","101","76")
buffer = getOperation(lcities, "buffer", dist=3500, ID=ID)

# plota o buffer dos poligonos escolhidos

plot(catarina[oesteids,], col="gray")
plot(catarina, bor="gray",col="white", add=T)
plot(catarina[oesteids,], bor="white", col="gray", add=T)
plot(buffer, col="yellow", add=T)
plot(catarina[ID,], add=T, col="gray")
box()

# funcao para desenhar os vizinhos mais proximos de 
# um layer de pontos dado uma distancia maxima
drawNearest = function(distance)
{
	aRTsilent(TRUE)
	plot(catarina[oesteids,], asp=1, bor="white", col="gray")
	plot(catarina, bor="gray", col="white",add=T) 
	plot(catarina[oesteids,], bor="white", col="gray", add=T)

	plot(center, add=T, pch=19, id=oesteids)
	id = oesteids 

	# cria um SpatialLines com uma linha, depois só
	# muda esta linha e plota
	l=Line(center[c(1,2),]@coords)
	L = Lines(l, paste(1,2, sep="_"))
	SL=SpatialLines(list(L))

	# para cada par de pontos calcula a distancia e plota
	# se for menor que o limiar
	for(i in 1:(length(id)-1))
	{
        for(j in (i+1):(length(id)))
	  {
		d = getDistance(lcities, paste(c(id[i],id[j])))
		if(d < distance)
		{
		  SL@lines[[1]]@Lines[[1]]@coords=center[c(which(getID(center) == id[i]),which(getID(center) == id[j])),]@coords
		  plot(SL, add=T, col="blue")
		}
	  }
	}
	box()
	aRTsilent(FALSE)
}

drawNearest(15000)

