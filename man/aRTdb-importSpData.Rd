\name{aRTdb-importSpData}
\alias{importSpData}
\alias{importSpData,aRTdb-method}
\title{importSpData}
\description{
	Function to import a Spatial data to the database. It create a layer and
   a table, and then import the data to them.
}
\usage{
\S4method{importSpData}{aRTdb}(object,spdata,lname,tname=lname, proj=NA,thname=NULL,view=thname)
}
\arguments{
  \item{object}{Object of class \code{aRTdb}.}
  \item{spdata}{object of class \code{Spatial}.}
  \item{lname}{Name of the layer that will store the data.}
  \item{tname}{Name of the table (default is NA).}
  \item{proj}{A string containing a PROJ.4 description.}
  \item{thname}{If used, create a theme with its name and returns it.}
  \item{view}{Name of the view.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
