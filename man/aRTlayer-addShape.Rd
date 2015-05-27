\docType{methods}

\alias{addShape}
\alias{addShape,aRTlayer-method}

\title{Add a shapefile to a layer}

\name{aRTlayer-addShape}

\description{Import a shapefile to a layer, without loading the data into \R. It creates
a static table to store the attributes.}
\usage{
\S4method{addShape}{aRTlayer}(object, file, table, ID, length=10)
}

\arguments{
   \item{object}{Object of class \code{aRTlayer}.}
   \item{file}{File name, with \dQuote{.shp}.}
   \item{table}{Name of the table.}
   \item{ID}{The name of the unique identifier of the table.}
   \item{length}{The maximum length of id instances.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
  See \code{\link{aRTlayer-class}} to the description of this class.
  \code{\link{aRTtable-class}} to the description of this class.
  \code{read.shape} from \code{maptools} to load the file in \R (it is
  important to find out which attribute is the id before import the
  data to a layer).  
}

% vim: syntax=tex
