\name{aRTdb-reload}
\alias{reload}
\alias{reload,aRTdb-method}
\title{reload}
\description{
	Function that reloads views from the database. One can change the owner of the views. See \code{openDb}.
}
\usage{
\S4method{reload}{aRTdb}(object, user)
}
\arguments{
  \item{object}{object of aRTdb class.}
  \item{user}{The owner of the views in the database.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{environment}
% vim: syntax=tex
