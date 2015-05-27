\name{aRTdb-importTable}
\alias{importTable}
\alias{importTable,aRTdb-method}
\alias{importTable,aRTlayer-method}

\title{Import a table}

\description{
	Function to import a table. It will be external if imported to an
	\code{aRTdb}, or static if to an \code{aRTlayer}. For temporal tables,
    you have to work with the primitives \code{createTable}, \code{createColumn},
    \code{addRows} and \code{updateColumns}.
}

\usage{
\S4method{importTable}{aRTdb}(object, table, ID, data)
\S4method{importTable}{aRTlayer}(object, table, ID, data, ...)
}

\arguments{
  \item{object}{Object of class \code{aRTdb}.}
  \item{table}{The name of the table to be created.}
  \item{ID}{Table's unique ID.}
  \item{data}{data.frame containing the table data.}
  \item{...}{Other arguments passed to \code{createTable}.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
