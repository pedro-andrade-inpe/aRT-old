\name{aRTgeometry-getClip}

\docType{methods}

\alias{getClip,aRTgeometry-method}
\alias{getClip}
\alias{clipping}

\title{getClip}

\description{Returns a clip of a set of polygons or lines over a given set of polygons.
It returns the result of this operation over each element, and maintain the respective IDs.}

\usage{
\S4method{getClip}{aRTgeometry}(object, geometry, as.sp)
}

\arguments{
  \item{object}{Object of class \code{aRTgeometry}, containing polygons exclusive or lines.}
  \item{geometry}{Object of class \code{aRTgeometry}, containing polygons. If you have cells, you have to convert them to polygons.}
  \item{as.sp}{Convert the result to a sp object or keep it as a TerraLib object in memory?}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}

\seealso{
   See \code{aRTgeometry-class} to the description of this class.
}

% vim: syntax=tex
