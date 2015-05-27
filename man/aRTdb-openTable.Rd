\docType{methods}
\alias{openTable}
\alias{openTable,aRTlayer-method}
\alias{openTable,aRTdb-method}
\alias{openTable}

\title{Open a table}

\name{aRTdb-openTable}

\description{Opens an existent table.}

\usage{
\S4method{openTable}{aRTdb}(object, tablename=NULL, type="default")
\S4method{openTable}{aRTlayer}(object, tablename=NULL, type="default")
}

\arguments{
   \item{object}{Object of class \code{aRTlayer} or \code{aRTdb}.}
   \item{tablename}{Name of the table.}
   \item{type}{The type of the table. It is used only in media tables, which have no name. In this case, type must be \dQuote{media} }.
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  See \code{\link{aRTtable-class}} to the description of this class, and table types.
}
% vim: syntax=tex
