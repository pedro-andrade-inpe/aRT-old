
require(aRT)

data(recife.districts)

#simplifying poligons

subset = recife.districts[c(18)]
g = as.aRTgeometry(subset)
simplify(g,60,90) 
res = getGeometry(g)

plot(subset, lty=0, col="yellow")
plot(res, add=T, lwd=2)


