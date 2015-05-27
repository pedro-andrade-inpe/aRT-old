\docType{methods}
\alias{openDb}
\alias{openDb,aRTconn-method}
\title{Open an existent database}
\name{aRTconn-openDb}
\description{Open a database connection from a virtual connection.}

\usage{
\S4method{openDb}{aRTconn}(object, dbname, update, views)
}

\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{dbname}{Database name.}
  \item{update}{When the database version is older than the actual TerraLib version, set it \code{TRUE} to update to the newest version.
You can connect to a database older than the actual version without updating it, but you have to take into account that unexpected errors may occour.}
  \item{views}{A string with a username. If used, aRT will load the views from that user instead of the views of the owner of the connection. Note that the permissions in the database are controlled by the user/password of the aRTconn, that still being used for the connection. This user is just for TerraLib purposes.}
}

\examples{
\dontrun{
con=openConn()
db=openDb(con, "database")
}}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  \code{\link{createDb}} to create new databases.
  \code{aRTconn-class} to show the description of this class.
}

% vim: syntax=tex
