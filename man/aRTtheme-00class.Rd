\name{aRTtheme}

\docType{class}

\alias{aRTtheme-class}
\alias{aRTtheme}
\alias{initialize,aRTtheme-method}
\alias{plot,aRTtheme-method}

\title{Class aRTtheme}
\description{Class that enables database queries with temporal and (or) attribute
restrictions. It can select the objects from the  ids resulting from a spatial query.

Themes in TerraLib can have visual aspects, as colors and lines styles. These
configurations are not used when an \code{aRTtheme} object is created, besause it
can be used only for data processing. But there are functions to do so, for example
\code{setVisual} and \code{setVisible}.}

\section{Objects from the Class}{
Objects can be created by calling \code{createTheme} or \code{openTheme}, from \code{aRTlayer}. 
\code{openTheme} is a function of class \code{aRTdb} too.
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
    \item{getBox}{Returns the bounding box.}
    \item{getData}{Returns all attributes and geometry of the theme.}
    \item{getVisual}{Returns the visual configuration of the theme.}
 	\item{openQuerier:}{Opens a query over the theme.}
	\item{setVisible}{Changes the visibility of the theme (visible or not).}
    \item{setVisual}{Sets the visual of the theme.}
  }
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{classes}
\seealso{
 See \code{aRTtheme-METHOD} for a method of this class or \code{CLASSNAME-class} for links to other classes.
}
% vim: syntax=tex
