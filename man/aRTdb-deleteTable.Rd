\name{aRTdb-deleteTable}
\alias{deleteTable}
\alias{deleteTable,aRTdb-method}
\title{deleteTable}
\description{
	Function to delete a table from the database.
}
\usage{
\S4method{deleteTable}{aRTdb}(object,tablename,force=FALSE)
}
\arguments{
  \item{object}{Object of aRTdb class.}
  \item{tablename}{name of the table.}
  \item{force}{As it is a dangerous operation, this argument asks for avoiding
        a confirmation (defult is FALSE).}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
	\code{aRTtable-class} for description of TerraLib tables.
}
% vim: syntax=tex
