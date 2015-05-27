\name{aRTdb-importMif}
\alias{importMif}
\alias{importMif,aRTdb-method}
\title{importMif}
\description{
	Function to import a mid-mif file to the database. It creates a layer and
	import the data to it.
}
\usage{
\S4method{importMif}{aRTdb}(object, file, lname)
}
\arguments{
  \item{object}{Object of class \code{aRTdb}.}
  \item{file}{The mid-mif file name.}
  \item{lname}{Name of the layer that will contain the data.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
