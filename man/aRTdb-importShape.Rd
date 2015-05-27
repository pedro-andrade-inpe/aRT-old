\name{aRTdb-importShape}
\alias{importShape}
\alias{importShape,aRTdb-method}
\title{importShape}
\description{
	Function to import a shapefile to the database. It create a layer and
	import the data to it.

	This function is just a wrapper to \code{addShape}.
}
\usage{
\S4method{importShape}{aRTdb}(object, layer, proj, ...)
}
\arguments{
  \item{object}{Object of class \code{aRTdb}.}
  \item{layer}{Name of the layer that will contain the data.}
  \item{proj}{A proj.4 descriptor (default is NA).}
  \item{...}{Arguments passed to \code{addShape}.}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
