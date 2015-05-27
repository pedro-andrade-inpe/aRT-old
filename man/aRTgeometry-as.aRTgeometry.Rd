\name{as.aRTgeometry}

\alias{as.aRTgeometry,SpatialPolygons-method}
\alias{as.aRTgeometry,SpatialLines-method}
\alias{as.aRTgeometry,SpatialPointsDataFrame-method}
\alias{as.aRTgeometry,SpatialPoints-method}
\alias{as.aRTgeometry}

\title{as.aRTgeometry}

\description{
Converts a Spatial data to the \pkg{aRT} format. It does not convert \code{SpatialPoints} because this class does not have ID,
but it converts \code{SpatialPointsDataFrame}.
}

\usage{
\S4method{as.aRTgeometry}{SpatialPointsDataFrame}(object)
\S4method{as.aRTgeometry}{SpatialLines}(object)
\S4method{as.aRTgeometry}{SpatialPolygons}(object)
}

\arguments{
  \item{object}{Object of \pkg{sp} class.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
   \code{\link{getGeometry}} to convert the result to \code{sp} again.
}



% vim: syntax=tex
