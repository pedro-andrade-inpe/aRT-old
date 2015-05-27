\name{aRTlayer-getPolygons}

\alias{getPolygons}
\alias{getPolygons,aRTlayer-method}

\title{Polygons data from a layer}

\description{
	Function that returns the polygons of a layer.
}

\usage{
\S4method{getPolygons}{aRTlayer}(object,as.sp=TRUE, slice=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{as.sp}{Return Spatial data (TRUE, default) or an object of class \code{aRTgeometry} (FALSE)?}
  \item{slice}{In the case of very large databases, you can use a numeric value for slices indicating that getPolygons will read slices of the database instead of reading all once. It can make the reading process faster.}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
