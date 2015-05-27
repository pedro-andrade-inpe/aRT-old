\name{aRTcomponent}
\docType{class}
\alias{aRTcomponent-class}
\alias{.aRTcall,aRTcomponent-method}
\alias{show,aRTcomponent-method}
\alias{summary,aRTcomponent-method}

\title{Class aRTcomponent}
\description{Basic virtual class, that defines functions to be used by
all other classes.}

\section{Slots}{
  \describe{
    \item{\code{pointer}:}{An external pointer to a C++ object. DO NOT change it!}
  }
}

\section{Methods}{
  \describe{
    \item{show:}{Prints some information of the object in a formatted way.}
    \item{summary:}{Return a list with informations about the object.}
  }
}
\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}

\seealso{
  See \code{aRT-METHOD} for a method of this class  or
      \code{CLASSNAME-class} for links to other classes.
}
\keyword{classes}
% vim: syntax=tex
