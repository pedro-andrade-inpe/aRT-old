\name{aRTlayer-getPoints}

\alias{getPoints}
\alias{getPoints,aRTlayer-method}

\title{Points data from a layer}

\description{
	Function that returns the points of a layer.
}

\usage{
\S4method{getPoints}{aRTlayer}(object, table=NULL, as.sp=TRUE, slice=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{table}{A table name, to be read from the database with the points as a SpatialPointsDataFrame. If \code{NULL}, the returned value does not contain any attribute but the ID.}
  \item{as.sp}{Return Spatial data or an object of class \code{aRTgeometry}}
  \item{slice}{In the case of very large databases, you can use a numeric value for slices indicating that getPoints will read slices of the database instead of reading all once. It can make the reading process faster.}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
