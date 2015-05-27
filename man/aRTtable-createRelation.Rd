\name{aRTtable-createRelation}

\alias{createRelation}
\alias{createRelation,aRTtable-method}

\title{Create a new table relation}

\description{
	(not implemented)
	Function that creates a new relation between two database tables.
	To access the joining of this two tables create an \code{aRTtheme}
    object using both tables.
}
\usage{
\S4method{createRelation}{aRTtable}(object, attr, rtable, rattr=attr)
}

\arguments{
  \item{object}{\code{aRTtable} to be linked.}
  \item{attr}{This tables attribute to be joined.}
  \item{rtable}{The related table.}
  \item{rattr}{Related table attribute. Default is \code{attr}.}
%  \item{cascadedel}{Delete data from object if their links are removed. Default is \code{FALSE}.}
%  \item{relname}{The relation's name.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTtable-class} to show the description of this class.
}
% vim: syntax=tex
