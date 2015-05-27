\name{aRTtable-addRows}

\alias{addRows}
\alias{addRows,aRTtable-method}

\title{Add rows to a table}

\description{
	Function to add new rows to a table. It requires an exact matching among colnames and
	the table colnames (although they do not have to be in the same order).

	If the table has one of the primary keys missing (or the primary key missing, if there is only one), 
    this function uses rownames as values.

    If a row already exists in the database, it will be updated.
}
\usage{
\S4method{addRows}{aRTtable}(object, data)
}
\arguments{
  \item{object}{Object of class \code{aRTtable}.}
  \item{data}{A \code{data.frame}.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}
\keyword{chron}
\seealso{
	See \code{aRTtable-class} to show the description of this class.
}
% vim: syntax=tex
