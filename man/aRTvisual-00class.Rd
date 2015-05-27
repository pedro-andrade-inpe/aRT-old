\name{aRTvisual}

\docType{class}

\alias{aRTvisual}
\alias{aRTvisual-class}
\alias{show,aRTvisual-method}
\alias{initialize,aRTvisual-method}

\title{Class aRTvisual}
\description{Class that contains configurations about the visual of a theme. Some
attributes are used for specific geometries. The attribute names are the same of TerraLib
objects, if you want to use R values (lty, for instance), we recommend to use \code{visualPoints()},
\code{visualLines()}, \code{visualPolygons()} and \code{visualRaster()}.}

\section{Objects from the Class}{
\code{new("aRTvisual", color="blue")}
}

\section{Slots}{
  \describe{
    \item{\code{color}}{internal color of the elements, or a vector of colors of a raster.}
	\item{\code{ccolor}}{the contour color.}
	\item{\code{style}}{the style of the drawing. It depends on the geometry.}
	\item{\code{transp}}{the transparency factor of the color, a integer between 0 and 100.}
	\item{\code{ctransp}}{the transparency factor of ccolor.}
	\item{\code{cwidth}}{the contour width.}
	\item{\code{width}}{width of the line.}
	\item{\code{size}}{the size of the object to be drawn.} 
  }
}

\author{Pedro Ribeiro de Andrade}
\keyword{classes}

% vim: syntax=tex
