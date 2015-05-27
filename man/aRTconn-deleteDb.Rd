\name{aRTconn-deleteDb}

\alias{deleteDb}
\alias{deleteDb,aRTconn-method}

\title{Delete a database}

\description{
	Function to delete a database in the DBMS.
}

\usage{
\S4method{deleteDb}{aRTconn}(object, dbname, force=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{dbname}{Name of the database.}
  \item{force}{As it is a dangerous operation, this argument asks for avoiding
        a confirmation (defult is \code{FALSE}).}  
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	\code{aRTconn-class} shows the description of this class.
}
% vim: syntax=tex
