\name{aRTlayer-addLines}

\alias{addLines}
\alias{addLines,aRTlayer-method}
\alias{addLines,aRTnetwork-method}

\title{Add lines to a layer or to a network.}

\description{
	Function to add a set of lines to a layer or to a network.
}
\usage{
\S4method{addLines}{aRTlayer}(object, lines)
\S4method{addLines}{aRTnetwork}(object, lines, weight)
}
\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{lines}{A set of lines described in an \code{aRTgeometry} object.}
  \item{weight}{The weight of the lines for computing distances.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
	See \code{aRTlayer-class} and  \code{aRTnetwork-class} to show the description of these class.
}
% vim: syntax=tex
