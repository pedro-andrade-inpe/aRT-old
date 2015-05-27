\name{aRTlayer-getSetOperation}

\docType{methods}

\alias{getSetOperation,aRTlayer-method}
\alias{getSetOperation}
\alias{union}
\alias{difference}
\alias{xor}
\alias{intersection}

\title{Set Operations}

\description{
Return the result of an operation involving sets (union, intersection, difference and xor). The result has "" as ID.
}

\usage{
\S4method{getSetOperation}{aRTlayer}(object, operation= c("union", "intersection", "difference", "xor"), ID = NULL, as.sp = TRUE)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{operation}{One of:
  \itemize{
  	\item union
  	\item intersection
  	\item difference
  	\item xor (does not work yet)
  }}
  \item{ID}{The ids which to be used in the operation. \code{difference} and \code{xor} use only two ids. \code{union} and \code{intersection} have as default all objects of the layer.}
  \item{as.sp}{Return Spatial data or an object of class \code{aRTgeometry}}
}

\note{If one polygon has two consecultive and equal points, these algorithms
may not work, therefore you need to remove this unwanted data before.}

\examples{
\dontrun{
## difference between two geometries
getSetMetric(layer, "difference", ids=c("1", "2"))

## union of all geometries of the layer
getSetMetric(layer, "union")
}
}

\keyword{connection}
\author{Pedro Ribeiro de Andrade}

\seealso{
   See \code{aRTlayer-class} to the description of this class.
}

% vim: syntax=tex
