\docType{methods}
\alias{openQuerier}
\alias{createQuerier}
\alias{openQuerier,aRTtheme-method}
\alias{createQuerier,aRTtheme-method}
\title{"aRTquerier"}
\name{aRTtheme-openQuerier}
\description{Initializer of \code{aRTquerier} objects.}

\usage{
\S4method{openQuerier}{aRTtheme}(object, geom=c("", "points", "lines", "polygons", "none"), loadatt=TRUE, chronon = c("nochronon", "second", "minute", "hour", "day", "month", "year", "dayofweek", "dayofmonth", "dayofyear", "monthofyear", "season", "weekofyear", "hourofday", "minuteofhour", "secondofminute"), stat = c("nostat", "sum", "max", "min", "count", "validcount", "stddev", "kernek", "mean", "variance", "skewness", "kurtosis", "median", "varcoeff", "mode"))
}

\arguments{
   \item{object}{Object of class \code{aRTtheme}\}.}
   \item{geom}{Which geometry will be retrieved? One of "points", "polygons", "lines", "cells".}
   \item{loadatt}{Will attributes be retrieved?}
   \item{chronon}{A temporal slicing.}
   \item{stat}{An operator to be applied over the elements of the same slice.}
}

\section{Chronon}{
    \itemize{
    \item nochronon
    \item second
	\item minute
	\item hour
	\item day
	\item month
	\item year
	\item dayofweek
	\item dayofmonth
	\item dayofyear
	\item monthofyear
	\item season
	\item weekofyear
	\item hourofday
	\item minuteofhour
	\item secondofminute
	}
}

\section{Stat}{
	\itemize{
	\item nostat
	\item sum
	\item max
	\item min
	\item count
	\item validcount
	\item stddev
	\item kernek
	\item mean
	\item variance
	\item skewness
	\item kurtosis
	\item median
	\item varcoeff
	\item mode
  }
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  See \code{\link{aRTquerier-class}} to the description of this class.
}
% vim: syntax=tex
