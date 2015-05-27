\name{aRTlayer-getLines}

\alias{getLines}
\alias{getLines,aRTlayer-method}

\title{Lines data from a layer}

\description{
	Function that returns the lines of a layer.
}

\usage{
\S4method{getLines}{aRTlayer}(object,as.sp=TRUE, slice=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{as.sp}{Return Spatial data (TRUE, default) or an object of class \code{aRTgeometry} (FALSE)?}
  \item{slice}{In the case of very large databases, you can use a numeric value for slices indicating that getLines will read slices of the database instead of reading all once. It can make the reading process faster.}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
