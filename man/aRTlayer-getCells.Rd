\name{aRTlayer-getCells}

\alias{getCells}
\alias{getCells,aRTlayer-method}

\title{Cells data from a layer}

\description{
	Function that returns the cells of a layer.
}

\usage{
\S4method{getCells}{aRTlayer}(object, as.sp = TRUE, slice = FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{as.sp}{Return Spatial data (TRUE, default) or an object of class \code{aRTgeometry} (FALSE)?}
  \item{slice}{An number indicating the slice size of the querier. Data will be read in blocks of size at most slice. It makes reading data faster for large datasets. The default (FALSE) forces the data to be read once.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTlayer-class} to the description of this class.
}
% vim: syntax=tex
