\name{aRTtable-updateColumns}

\alias{updateColumns}
\alias{updateColumns,aRTtable-method}

\title{Update table columns}

\description{
	Update a set of table data. It does not need to have all
	columns, and if there are columns inexistent in the table, 
	they will be created.
}
\usage{
\S4method{updateColumns}{aRTtable}(object, data)
}
\arguments{
  \item{object}{Object of class \code{aRTtable}.}
  \item{data}{A \code{data.frame}.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\seealso{
	See \code{aRTtable-class} to show the description of this class.
}
% vim: syntax=tex
