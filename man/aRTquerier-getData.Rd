\name{aRTquerier-getData}
\alias{getData}
\alias{getData,aRTtable-method}
\alias{getData,aRTtheme-method}
\alias{getData,aRTquerier-method}

\title{Get data}

\description{
	Function that returns all attributes of an object (theme, table or querier).
}

\usage{
\S4method{getData}{aRTquerier}(object, quantity = -1, as.sp = TRUE)
\S4method{getData}{aRTtable}(object)
\S4method{getData}{aRTtheme}(object)
}

\arguments{
  \item{object}{Object of class \{\code{aRTtheme},\code{aRTtable},\code{aRTquerier}\}}
  \item{quantity}{The number of elements to be read from the database. The default value indicates that all object will be retrieved.}
  \item{as.sp}{Return Spatial data or an object of class \code{aRTgeometry}}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
   See \code{aRTCLASS-class} to the description of CLASS.
}
% vim: syntax=tex
