\name{aRTtable-createColumn}

\alias{createColumn}
\alias{createColumn,aRTtable-method}
\title{Create a column}

\description{
	Create an empty column in a table.
}
\usage{
\S4method{createColumn}{aRTtable}(object, colname, type="c", length=10, key=FALSE, decimals=3, autonumber=FALSE)
}

\arguments{
  \item{object}{Object of class \code{aRTtable}.}
  \item{colname}{The name of the new column.}
  \item{type}{string representing the column type (see below).}
  \item{length}{The maximum length of the values if they are strings.}
  \item{key}{If the column will have unique values.}
  \item{decimals}{Max number of decimal digits after the decimal point.}
  \item{autonumber}{Whether the elements of the column start with one and will be automatically created by the DBMS.}
}

\section{Column Types}{
 \describe{
 	\item{i}{integer}
	\item{d}{decimal (=i)}
  	\item{s}{string}
	\item{c}{character (=s)}
	\item{f}{factor (=s)}
	\item{n}{numeric}
	\item{r}{real (=n)}
  }
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTtable-class} to show the description of this class.
}
% vim: syntax=tex
