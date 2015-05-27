\name{aRTlayer}

\docType{class}

\alias{aRTlayer}
\alias{aRTlayer-class}
\alias{initialize,aRTlayer-method}
\alias{plot,aRTlayer-method}

\title{Class aRTlayer}
\description{Class that represents a layer of information in the database. It can insert
geometries into the database, process them, and create/open \code{aRTtable}, \code{aRTtheme}, 
or \code{aRTquerier} objects.
TerraLib layers can store different geometries within a single layer.}

\section{Objects from the Class}{
Objects can be created by calling \code{createLayer()} and \code{openLayer()}, methods of \code{aRTdb}.
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
    \item{addLines:}{Inserts lines in the layer.}
    \item{addPoints:}{Inserts points in the layer.}
    \item{addPolygons:}{Inserts polygons (rings) in the layer.}
    \item{addRaster:}{Inserts a raster data in the layer.}
    \item{addShape:}{Inserts data from a shapefile in the layer.}
	\item{createLayer:}{Creates a layer with cellular data, based on the spatial data of the layer.}
	\item{createTable:}{Creates a table and returns it.}
	\item{createTheme:}{Creates a theme and returns it.}
	\item{getBox:}{Returns the bunding box of the layer.}
	\item{getDistance:}{Returns the distance betweeen layer objects.}
	\item{getLines:}{Returns the lines of the layer.}
	\item{getMetric:}{Returns a metric operations (buffer, length, area, convex hull).}
	\item{getNN:}{Returns the nearest neighbours of some geometries. (Not implemented yet)}
	\item{getOperation:}{Returns the centroid, convex hull, or buffer of a given object.}
	\item{getPoints:}{Returns the points of the layer.}
	\item{getPolygons:}{Returns the polygons of the layer.}
	\item{getProj:}{Returns information about the projection of the layer.}
	\item{getRaster:}{Returns the raster of the layer.}
	\item{getRelation:}{Looks for spatial relations as contains, crosses, touches.}
	\item{getSetOperation:}{Calculates a set operation (union, difference, intersection, xor).}
	\item{importTable:}{Imports a data.frame to a new table.}
 	\item{openTable:}{Opens an existent table.}
 	\item{openTheme:}{Opens an existent theme.}
 	\item{openQuerier:}{Opens a query over the layer.}
	\item{plot:}{Plots the data.}
    \item{showTables:}{Shows all information tables in the layer.}
 }
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}
\keyword{classes}
\seealso{
 See \code{aRTlayer-METHOD} for a method of this class or \code{CLASSNAME-class} for links to other classes.
 Package \pkg{spproj} for conversion of projections.
}

% vim: syntax=tex
