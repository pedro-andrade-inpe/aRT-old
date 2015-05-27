\name{simplify}
\alias{simplify,aRTgeometry-method}
\alias{simplify}
\title{simplify}

\description{
Simplifies a set of polygons or lines. The objects may lose some of their
geometric relations with other objects, such as touches, although
the algorithm is deterministic. See D. Douglas and T. K. Peucker. Algorithms
for the reduction of the number of points required to represent a line or its
caricature. The Canadian Cartographer. v. 10, n. 2, pp. 112-122, 1973.
}

\usage{
\S4method{simplify}{aRTgeometry}(object, snap, maxdist)
}

\arguments{
    \item{object}{Object of \pkg{aRTgeometry} class.}
    \item{snap}{Simplification threshold.}
    \item{maxdist}{The maximum distance between intermediary segments.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}

% vim: syntax=tex
