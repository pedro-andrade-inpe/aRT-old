\docType{methods}

\alias{createTable}
\alias{createTable,aRTlayer-method}
\alias{createTable,aRTdb-method}
\alias{createTable}

\title{Create an attribute table}
\name{aRTdb-createTable}

\description{Creates a new attribute table in the database and returns it.
If the table is created from an \code{aRTdb}, then its type will be external.
In the case of creating it from layers, it can be any type but external.
}

\details{TerraLib uses some tables in the database, and then you cannot create
tables with these names. Names as \dQuote{\code{Points...}}, \dQuote{\code{Polygons...}},
(other geometries), \dQuote{\code{tempTable}} and \dQuote{\code{te_...}} are reserved.
Note that names with non letters characters are also prohibited.
}

\usage{
\S4method{createTable}{aRTdb}(object, tablename, ID="ID", length=16)
\S4method{createTable}{aRTlayer}(object, tablename=NULL, genID=NULL, type="static", ID="ID", link=ID, length=16, itimeatt="time", ftimeatt="time")
}

\arguments{
   \item{object}{Object of \code{aRTlayer} or \code{aRTdb} class.}
   \item{tablename}{Name of the table.}
   \item{ID}{The name of the unique identifier of the table.}
   \item{length}{The maximum length of ID instances.}
   \item{genID}{Populate the table IDs linking with the layer geometries?
         Used with static and dynamic attributes tables only (default is TRUE in the case of static tables and FALSE otherwise).}
   \item{type}{The type of the table. See \code{\link{aRTtable-class}} for the
         available types.}
   \item{link}{The attribute of the table that links it with the IDs of geometry tables. Usually, the unique identifier of the table is a good option.} 
   \item{itimeatt}{Initial time attribute name}
   \item{ftimeatt}{Final time attribute name. If it is the same as itimeatt (the default),
         the temporal table has only one temporal attribute.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  See \code{\link{aRTtable-class}} to the description of table types.
}
% vim: syntax=tex
