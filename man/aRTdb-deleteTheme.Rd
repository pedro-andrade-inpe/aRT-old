\name{aRTdb-deleteTheme}

\alias{deleteTheme}
\alias{deleteTheme,aRTdb-method}

\title{Delete a theme}
\description{
	Function to delete a theme from the database. Themes are created from layers, 
but they belong to a view. They can be removed only from \code{aRTdb} objects.
}

\usage{
\S4method{deleteTheme}{aRTdb}(object, themename, force=FALSE)
}

\arguments{
  \item{object}{Object of aRTdb class.}
  \item{themename}{name of the theme}
  \item{force}{As it is a dangerous operation, this argument asks for avoiding
        a confirmation (defult is FALSE).}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
