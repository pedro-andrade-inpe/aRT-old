\name{aRTlayer-getMetric}

\docType{methods}

\alias{getMetric,aRTlayer-method}
\alias{getMetric}
\alias{area}
\alias{length}

\usage{
\S4method{getMetric}{aRTlayer}(object, metric= c("area", "length"), ID=NULL)
}

\title{getMetric}

\description{Returns some spatial operation that from each spatial object of a layer.}

\arguments{
   \item{object}{Object of class \code{aRTlayer}.}
   \item{metric}{One metric among:
   \itemize{
      \item area
      \item length
      \item distance
      \item withinDist
   }}
   \item{ID}{A subset of the layer's IDs, over which the metrics will be computed. In the case of \code{NULL}, \code{getMetric} calculates the metrics over each element of the layer.}
}

\examples{
\dontrun{
getMetric(layer, "hull")
getMetric(layer, "buffer", dist=500)
}}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}

\seealso{
   See \code{aRTlayer-class} to the description of this class.
}

% vim: syntax=tex
