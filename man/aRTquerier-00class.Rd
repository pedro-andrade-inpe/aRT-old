\name{aRTquerier}
\docType{class}
\alias{aRTquerier-class}
\alias{initialize,aRTquerier-method}

\title{Class aRTquerier}

\description{Class that manipulates the result of a database query.
It returns the objects one by one, and can slide a temporal \code{aRTtheme}.}

\section{Objects from the Class}{
Objects can be created by calling \code{openQuerier()} from \code{aRTlayer} or
from \code{aRTtheme}.
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
    \item{getData:}{Returns the next element of the query result in the actual slide.}
	\item{nextFrame:}{Loads the next frame, if there is a temporal sliding. A temporal
	query needs to load one slide before using \code{getData} for getting its elements.
	Non-temporal queries can't use this function. Note that you can do a non-temporal
	query in a temporal layer.}
  }
}

\author{Pedro Ribeiro de Andrade}

\keyword{classes}
\keyword{chron}

% vim: syntax=tex
