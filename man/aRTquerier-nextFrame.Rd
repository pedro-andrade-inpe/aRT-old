\docType{methods}
\alias{nextFrame}
\alias{nextFrame,aRTquerier-method}
\title{Load next frame}
\name{aRTquerier-nextFrame}

\description{
	Loads the next frame of a temporal slicing. If the query does not 
	have a chronon this function returns an error.
    To check the number of slides use \code{summary(querier)}.
}

\usage{
\S4method{nextFrame}{aRTquerier}(object)
}

\arguments{
  \item{object}{Object of class \code{aRTquerier}.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\keyword{chron}
\seealso{
   See \code{aRTquerier-class} to the description of this class.
}

% vim: syntax=tex
