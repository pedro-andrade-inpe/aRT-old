\name{aRTdb}
\docType{class}
\alias{aRTdb-class}
\alias{aRTdb}
\alias{initialize,aRTdb-method}

\title{Class aRTdb}

\description{Class that manipulates a database. Data is stored in layers (geometries and
their attributes) or external tables (attributes not directly connected to any geometry).
\code{aRTdb} objects use more memory than any other object of this package because
it stores the definitions of all themes, layers, and tables. Objects of these three classes store only a
pointer to its \dQuote{father}, the database. 
We recommend to work with only one object of this class per time.
}

\section{Objects from the Class}{
Objects can be created by using \code{createDb} or \code{openDb}, methods of \code{aRTconn}.
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
	\item{createLayer:}{Creates a new layer returns it.}
    \item{createTable:}{Creates an empty table in the database.}
    \item{createView:}{Creates an empty View in the database.}
    \item{deleteLayer:}{Deletes a layer of the database.}
    \item{deleteTable:}{Deletes a table of the database.}
    \item{deleteTheme:}{Deletes a theme of the database.}
    \item{deleteView:}{Deletes a view in the database.}
    \item{importMif:}{Imports a MID/MIF file to the database.}
    \item{importShape:}{Imports a shapefile to the database.}
    \item{importSpData:}{Imports data in the sp format to the database.}
    \item{importTable:}{Imports a data.frame to a database table.}
 	\item{openLayer:}{Opens an existent layer.}
 	\item{openTheme:}{Opens an existent theme. (Themes can be created only from layers.)}
 	\item{reload:}{Reloads the data about themes and layers. It is useful when something has been changed from outside R.}
    \item{showLayers:}{Shows all the available layers.}
    \item{showThemes:}{Shows all the available themes.}
    \item{showViews:}{Show all the available views.}
	\item{summary:}{Returns some information about the database.}
  }
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}

\keyword{classes}
\seealso{
 See \code{aRTdb-METHOD} for a method of this class or
     \code{CLASSNAME-class} for links to other classes.
}

% vim: syntax=tex
