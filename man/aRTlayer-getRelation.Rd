\name{aRTlayer-getRelation}

\docType{methods}

\alias{getRelation,aRTlayer-method}
\alias{getRelation}
\alias{contain}
\alias{overlap}
\alias{cross}
\alias{disjoint}
\alias{within}
\alias{intersects}
\alias{equal}
\alias{cover}

\title{Spatial Relations}

\description{Returns elements of a given layer that satisfy a given spatial relation.
The spatial relation can refer to objects of a different layer.}

\usage{
\S4method{getRelation}{aRTlayer}(object, relation=c("contains", "overlaps", "crosses", "disjoint", "touches", "within", "intersects", "equals", "covers", "coveredby"), layer=object, ID)
}

\arguments{
  \item{object}{The target layer.}
  \item{relation}{One of "contains", "coveredby", "covers", "crosses", "disjoint", "equals", "intersects", "overlaps", "touches", or "within"}
  \item{layer}{The layer over which the spatial relations will be computed.}
  \item{ID}{The selected ids of layer to be used. If \code{NULL}, all the elements of layer will be used.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{connection}

\seealso{
   See \code{aRTlayer-class} to the description of this class.
}

% vim: syntax=tex
