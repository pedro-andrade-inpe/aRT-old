\name{aRTdb-deleteView}
\alias{deleteView}
\alias{deleteView,aRTdb-method}
\title{deleteView}
\description{
	Function to delete a view from the database. Note that all themes belonging to 
this view will be removed.
}

\usage{
\S4method{deleteView}{aRTdb}(object, viewname, force=FALSE)
}
\arguments{
  \item{object}{Object of the aRTdb class.}
  \item{viewname}{Name of the view.}
  \item{force}{As it is a dangerous operation, this argument asks for avoiding
        a confirmation (defult is FALSE).}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
