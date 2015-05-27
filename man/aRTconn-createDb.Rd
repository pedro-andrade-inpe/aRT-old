\name{aRTconn-createDb}
\docType{methods}
\alias{createDb}
\alias{createDb,aRTconn-method}
\title{Create a new database}
\description{Create a new database from a virtual connection and returns a
connection to it.}

\usage{
\S4method{createDb}{aRTconn}(object, dbname, replace=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{dbname}{Database name.}
  \item{replace}{Delete the database if it already exists? Default is FALSE.}
}

\examples{
\dontrun{
con=openConn()
database = createDb(con, "database")
dbx = createDb(con, "database") # error, database exists
db = createDb(con, "database", delete=TRUE) # OK
}}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  \code{\link{openDb}} opens (without create) databases.
  \code{\link{aRTconn}} shows the description of this class.
}

% vim: syntax=tex
