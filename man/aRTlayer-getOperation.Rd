\name{aRTlayer-getOperation}

\docType{methods}

\alias{getOperation,aRTlayer-method}
\alias{getOperation}
\alias{hull}
\alias{center}
\alias{centroid}

\usage{
\S4method{getOperation}{aRTlayer}(object, operation= c("centroid", "hull", "buffer"), ID = NULL, distance = NULL, as.sp = TRUE)
}

\title{getOperation}

\description{Returns some spatial operation that is not stored in the layer.
It returns the result of this operation over each element.}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{operation}{One of:
   \itemize{
	  \item centroid
	  \item hull
	  \item buffer
   }
  }
  \item{ID}{A subset of the layer's IDs, over which the metrics will be computed. In the case of \code{NULL}, \code{getMetric} calculates the metrics over each element of the layer.}
  \item{distance}{The distance, used only with buffer.}
  \item{as.sp}{A boolean value indicating whether the result should be a Spatial or an \code{aRTgeometry} object}.
}

\examples{
\dontrun{
getOperation(layer, "hull")
getOperation(layer, "buffer", dist=500)
}}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}

\seealso{
   See \code{aRTlayer-class} to the description of this class.
}

% vim: syntax=tex
