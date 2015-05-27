\name{aRTdb-deleteLayer}
\alias{deleteLayer}
\alias{deleteLayer,aRTdb-method}
\title{deleteLayer}
\description{
	Function for deleting a layer from the database. It also deletes the themes created from this layer.
}
\usage{
\S4method{deleteLayer}{aRTdb}(object,layername,force=FALSE)
}
\arguments{
  \item{object}{Object of aRTdb class.}
  \item{layername}{Name of the layer.}
  \item{force}{As it is a dangerous operation, this argument asks for avoiding
        a confirmation (defult is FALSE).}
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
