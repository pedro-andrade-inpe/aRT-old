\name{aRTlayer-getDistance}
\alias{getDistance}
\alias{getDistance,aRTlayer-method}
\title{getDistance}

\description{
	Function that returns the distance between two spatial objects.
}

\usage{
\S4method{getDistance}{aRTlayer}(object, ID, layer=object)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{ID}{A vector with two IDs. The first belongs to object, and the second to layer.}
  \item{layer}{The layer where is the second object. The default is the same layer as object.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
