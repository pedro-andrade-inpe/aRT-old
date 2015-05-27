\docType{methods}
\alias{createLayer}
\alias{createLayer,aRTdb-method}
\alias{createLayer,aRTlayer-method}
\alias{cell}
\alias{cells}

\title{Create a layer}
\name{createLayer}
\description{Creates a new layer in a database and returns it. When created using an \code{aRTdb} object,
it returns an empty layer. When created using an \code{aRTlayer} object, it fills it with cells according
to a given resolution.}
\usage{
\S4method{createLayer}{aRTdb}(object, lname, proj=NA)
\S4method{createLayer}{aRTlayer}(object, lname, rx, ry, allbox)
}

\arguments{
   \item{object}{Object of \code{aRTdb} or \code{aRTlayer} class.}
   \item{lname}{Name of the new layer.}
   \item{proj}{Projection in proj.4 format, and dafault is no projection. 
         Some projections are not supported by TerraLib.}
   \item{rx}{The x axis resolution of the cell layer.}
   \item{ry}{The y axis resolution of the cell layer.}
   \item{allbox}{Will new layer have cells in the whole box of the original layer, or the cells will comprise only where there is some geometry?}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
  See \code{\link{aRTlayer-class}} to the description of this class, and
  \code{\link{openLayer}} to just open an existent layer. See \code{\link{createAndFillColumn}} to
  the description on how to fill cellular spaces.
  See also \pkg{spproj} for conversion between projections.
}
% vim: syntax=tex
