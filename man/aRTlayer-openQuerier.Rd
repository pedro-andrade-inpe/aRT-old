\docType{methods}
\alias{openQuerier,aRTlayer-method}
\alias{createQuerier,aRTlayer-method}
\title{Querier}
\name{aRTlayer-openQuerier}
\description{Initializer of \code{aRTquerier} objects.}

\usage{
\S4method{openQuerier}{aRTlayer}(object, geom= c("", "points", "lines", "polygons", "cells"))
}

\arguments{
   \item{object}{Object of class \{\code{aRTlayer},\code{aRTtheme}\}.}
   \item{geom}{Will geometry be retrieved? One of "points", "lines", "polygons", "cells"}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  See \code{\link{aRTquerier-class}} to the description of this class.
}
% vim: syntax=tex
