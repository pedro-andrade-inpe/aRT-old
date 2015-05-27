\name{aRTraster}
\alias{aRTraster}
\alias{aRTraster-class}
\alias{initialize,aRTraster-method}
\alias{getRaster,aRTraster-method}

\title{aRTraster}

\description{
	An \code{aRTraster} object represents a TerraLib raster in memory. Using it, one
    can execute raster operations without storing it in the database. 

	Objects of \code{aRTraster} can be directly inserted into layers, without
    converting them to Spatial objects.

	\code{aRTraster} is currently under development.
}

\section{Objects from the Class}{
Objects can be created by calls of the form \code{new("aRTraster", raster)}.
}

\section{Arguments}{
   \describe{
   \item{raster}{A raster data in the sp format.}
   }
}
				 
\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
    \item{getRaster:}{Returns the raster in the sp format.}
    \item{getPixels:}{Returns the pixels inside of a geometry.}
 }
}


\author{Pedro Ribeiro de Andrade}
\keyword{database}
% vim: syntax=tex
