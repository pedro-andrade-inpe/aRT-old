\name{aRTdb-createView}
\alias{createView}
\alias{createView,aRTdb-method}
\title{createView}
\description{
	Function that creates an empty View in the database. Views are not manipulated in aRT, 
	themes can be pushed inside a view when they are created.  Two themes
	in a same view can be visualized together in a TerraLib-based GIS.
}
\usage{
\S4method{createView}{aRTdb}(object, viewname="Viewname")
}
\arguments{
  \item{object}{object of aRTdb class.}
  \item{viewname}{Name of the view.}
}

\author{Marcos Aurelio Carrero and Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTdb-class} to show the description of this class.
}
% vim: syntax=tex
