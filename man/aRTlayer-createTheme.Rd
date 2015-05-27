\docType{methods}
\alias{createTheme}
\alias{createTheme,aRTlayer-method}

\title{Create a theme}

\name{aRTlayer-createTheme}

\description{Creates a theme and returns it.}

\usage{
\S4method{createTheme}{aRTlayer}(object, themename=summary(object)$layer, tables=NULL, viewname=themename, ID=NULL, attrest="")
}

\arguments{
  \item{object}{Object of class \code{aRTlayer}.}
  \item{themename}{The theme name. The default is the same name of the layer.}
  \item{tables}{The tables from the layer to be joined in the theme. There are some
        restrictions joining tables, for example the theme can have only one type
		of temporal table. It can generate errors because the default are all tables
		of the layer.}
  \item{viewname}{The view name. The default is the theme name.}
  \item{ID}{A vector of IDs representing a collection of objects to be selected in the theme.}
  \item{attrest}{A string with a SQL like restriction of attributes (the where clause)}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
   See \code{aRTtheme-class} to the description of this class.
}
% vim: syntax=tex
