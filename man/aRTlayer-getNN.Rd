\name{aRTlayer-getNN}

\alias{getNN}
\alias{getNN,aRTlayer-method}

\title{Get Nearest Neighbors}

\description{
	Function that returns the nearest neighbours of an object, based on
	the Euclidean distances.
}

\usage{
\S4method{getNN}{aRTlayer}(object, layer=object, ID, quantity=1)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{layer}{Object of class \code{aRTlayer}. The returned IDs belong to this layer.}
  \item{ID}{The spatial object id of an object belonging to \code{object}.}
  \item{quantity}{The number of neighbors to be returned.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
