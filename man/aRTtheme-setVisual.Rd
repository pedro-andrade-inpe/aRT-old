\name{aRTtheme-setVisual}

\alias{setVisual}
\alias{setVisual,aRTtheme-method}

\title{Set the theme's visual}

\description{
Function to set the visual of a theme, to be visualized in a TerraLib-based GIS.
If the visual has only one color, it will set the default visual of the geometric
objects. If it has more than one color, the function will generate a slicing.
}

\usage{
\S4method{setVisual}{aRTtheme}(object, visual, attribute=NULL, mode=c("equalsteps", "stddeviation", "quantil", "uniquevalue", "raster"))
}

\arguments{
  \item{object}{Object of class \code{aRTtheme}.}
  \item{visual}{Object of class \code{aRTvisual}.}
  \item{attribute}{Theme attribute which values will be used to generate the slices.}
  \item{mode}{Type of slicing (see below).}
}

\section{Slicing}{
The theme visual will be sliced if \code{visual} contains more then one color.
Slices can be generated based on 5 different factors, represented by \code{mode}:
\itemize{
\item equalsteps
\item stddeviation
\item quantil
\item uniquevalue
\item raster
}
When slicing a raster, \code{mode} will always be \code{raster}, and it will
be divided into equal steps, according with its values (and no \code{attribute} is
used). The other four modes are used with attributes.
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTtheme-class} to show the description of this class.
}
% vim: syntax=tex
