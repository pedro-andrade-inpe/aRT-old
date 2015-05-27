\docType{methods}
\alias{setVisible}
\alias{setVisible,aRTtheme-method}

\title{Set visiblility}

\name{aRTtheme-setVisible}

\description{
	Sets the visibility of a theme to be visualized in a TerraLib-based GIS.
	When a theme is created it is visible. This function does not work correctly yet.
}

\usage{
\S4method{setVisible}{aRTtheme}(object, points = NULL, lines = NULL, polygons = NULL, check = NULL)
}

\arguments{
  \item{object}{Object of class \code{aRTtheme}.}
  \item{points}{Will the points be visible?}
  \item{lines}{Will the lines be visible?}
  \item{polygons}{Will the polygons be visible?}
  \item{check}{?}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
   See \code{aRTtheme-class} to the description of this class.
}

% vim: syntax=tex
