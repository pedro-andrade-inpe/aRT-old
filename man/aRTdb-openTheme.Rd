\docType{methods}

\alias{openTheme}
\alias{openTheme,aRTlayer-method}
\alias{openTheme,aRTdb-method}

\title{Open a theme}

\name{aRTdb-openTheme}

\description{Open a database theme.}

\usage{
\S4method{openTheme}{aRTdb}(object, tname)
\S4method{openTheme}{aRTlayer}(object, tname)
}

\arguments{
  \item{object}{A layer or a database where the theme is stored.}
  \item{tname}{The theme name. Default is the layer name.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
   See \code{aRTCLASS-class} to the description of CLASS,
   and \code{\link{createTheme}} for how to create themes.
}
% vim: syntax=tex
