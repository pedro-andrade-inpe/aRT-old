\name{aRTdb-showTables}

\alias{showTables}
\alias{showTables,aRTlayer-method}
\alias{showTables,aRTdb-method}

\title{Show tables}
\description{
	Function that shows all tables inside of a layer.
}

\usage{
\S4method{showTables}{aRTdb}(object)
\S4method{showTables}{aRTlayer}(object)
}

\arguments{
  \item{object}{object of class aRTlayer or aRTdb}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}

\note{When there aren't any tables in the layer, the message \dQuote{Tables could not be loaded} will be shown.}
\keyword{database}
\seealso{
	See \code{aRTlayer-class} to show the description of this class.
}
% vim: syntax=tex
