### R code from vignette source 'aRTfillcell.Rnw'

###################################################
### code chunk number 1: "Rsettings"
###################################################
options(width=69)


###################################################
### code chunk number 2: "Loading package"
###################################################
require(aRT)
con=openConn(name="default")


###################################################
### code chunk number 3: aRTfillcell.Rnw:57-75
###################################################
db=createDb(con, "rondonia", replace=TRUE)
lcenso    = createLayer(db, "censo")
censofilename = system.file("shapes/censopop2000_bd.shp", package="aRT")
addShape(lcenso,    tab="censo",    file=censofilename, ID="CODIGO")
tcenso    = createTheme(lcenso)

lroads    = createLayer(db, "roads")
roadsfilename = system.file("shapes/rondonia_roads_lin.shp", package="aRT")
addShape(lroads,    tab="roads",    file=roadsfilename, ID="object_id_5")
troads    = createTheme(lroads)

lpoints = createLayer(db, "urban")
urbanfilename = system.file("shapes/rondonia_urban_centers_pt.shp", package="aRT")
addShape(lpoints,    tab="urban",    file=urbanfilename, ID="object_id_2")
tpoints    = createTheme(lpoints)

rm(db)
invisible(gc())


###################################################
### code chunk number 4: "Loading package"
###################################################
db=openDb(con, "rondonia")
lcenso    = openLayer(db, "censo")
tcenso    = openTheme(db, "censo")
tcenso

lroads    = openLayer(db, "roads")
troads    = openTheme(db, "roads")
troads

lpoints = openLayer(db, "urban")
tpoints = openTheme(db, "urban")
tpoints


###################################################
### code chunk number 5: aRTfillcell.Rnw:94-98
###################################################
par(mar=c(.5,.5,.5,.5))
plot(lcenso, col="yellow")
plot(lroads, col="blue", add=T, lwd=2)
plot(lpoints, bg="red", pch=22, add=T)


###################################################
### code chunk number 6: "Creating cells"
###################################################
lcells = createLayer(lcenso, "cells", rx=4000)
lcells


###################################################
### code chunk number 7: aRTfillcell.Rnw:119-122
###################################################
par(mar=c(.5,.5,.5,.5))
plot(tcenso, col="red",lwd=2)
plot(lcells, add=TRUE)


###################################################
### code chunk number 8: "Creating cells"
###################################################
lcells2 = createLayer(lcenso, "cellsbox", rx=10000, ry=5000, all=TRUE)


###################################################
### code chunk number 9: aRTfillcell.Rnw:146-149
###################################################
par(mar=c(.5,.5,.5,.5))
plot(tcenso, col="red",lwd=2)
plot(lcells2, add=T)


###################################################
### code chunk number 10: "Filling cells"
###################################################
tcells = openTable(lcells, "cells")
tcells


###################################################
### code chunk number 11: "Distance"
###################################################
# repare aqui que, caso o tema possua apenas pontos, nao eh necessario usar o argumento 'geom'
createAndFillColumn(tcells, newatt="dpoints", target=tpoints, geometry="point", strategy="distance")


###################################################
### code chunk number 12: "Area"
###################################################
createAndFillColumn(tcells, "length_roads", target=troads, strategy="length")


###################################################
### code chunk number 13: "visualize-operations"
###################################################
theme = createTheme(lcells, "theme_geometric_operations")


###################################################
### code chunk number 14: aRTfillcell.Rnw:195-219
###################################################
d = getData(theme)
df = d@data

par(mfcol = c(1, 2), mar=c(.5,.5,.5,.5), mgp=c(1.8,.8,0))

plotColoured = function(spatialdata, attribute, slices, colors)
{
    df = spatialdata@data

    vcolors=rep(colors[1], dim(df)[1])

    for(i in 1:length(slices))
    {
        vcolors[which(df[,attribute] > slices[i])] = colors[i]
    }
    plot(spatialdata, col=vcolors)
    box()
}

plotColoured(d, "dpoints", c(0, (1:5)*10000), heat.colors(6))
plot(lpoints, pch=21, add=T)

plotColoured(d, "length_roads", c(0, 0.001, 500, 1000, 2000, 3000, 4000), c("gray", heat.colors(6)[6:1]))
plot(lroads, add=T, lwd=2)


###################################################
### code chunk number 15: "Area"
###################################################
createAndFillColumn(tcells, "maxrenda", tcenso,   att="RENDIMENTO", strat="maximum")


###################################################
### code chunk number 16: "Area"
###################################################
createAndFillColumn(tcells, "pessoas",  tcenso,   att="NUMERO_PES", strat="sumwba")
createAndFillColumn(tcells, "rendacap", tcenso,   att="RENDAPCAPI", strat="averagewba")


###################################################
### code chunk number 17: "visualize-operations"
###################################################
theme = createTheme(lcells, "theme_bya")


###################################################
### code chunk number 18: aRTfillcell.Rnw:255-265
###################################################
cells = getData(theme)
polygons = getData(tcenso)

par(mfcol = c(2, 2), mar=c(.5,.5,.5,.5), mgp=c(1.8,.8,0))

plotColoured(polygons, "NUMERO_PES", c(0, 50, 100, 200, 400, 800, 1600, 3200), heat.colors(8))
plotColoured(cells, "pessoas", c(0, 5, 10, 20, 40, 80, 160, 320), heat.colors(8))

plotColoured(polygons, "RENDAPCAPI", c(0, 50, 100, 200), heat.colors(4))
plotColoured(cells, "rendacap", c(0, 50, 100, 200), heat.colors(4))


###################################################
### code chunk number 19: "Area"
###################################################
theme = createTheme(lcells, "mytheme")

theme

cells = getData(theme)
#polygons = getData(tcenso)
#for(i in 1:length(d@polygons))
#    d@polygons[[i]]@labpt = c(0,0)

#d@data = d@data[,-(1:3)] # removing the attributes of type string

#for(i in 1:dim(d@data)[2]) # normalizing the attributes
#{
#    mmax = max(d@data[,i])
#    d@data[,i] = d@data[,i]/mmax
#}


###################################################
### code chunk number 20: "visualize-operations"
###################################################
plotColoured = function(spatialdata, attribute, slices, colors)
{
    df = spatialdata@data

    vcolors=rep(colors[1], dim(df)[1])

    for(i in 1:length(slices))
    {
        vcolors[which(df[,attribute] > slices[i])] = colors[i]
    }
    plot(spatialdata, col=vcolors)
    box()
}

plotColoured(cells, "dpoints", c(0, (1:5)*10000), heat.colors(6))


