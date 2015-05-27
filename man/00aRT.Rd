
\name{aRT}
\alias{aRT}
\alias{aRT-classes}

\title{R-TerraLib API}
\description{
aRT is a package integrating R with the TerraLib geoprocessing library.
TerraLib contains a set of Geographic Information System (GIS) classes written
in C++ to support building GIS applications. It manipulates points, lines,
polygons and raster data (grids), storing them in a Data-Base Management
System (DBMS). It has functions for data importing and management, 
as well as spatial-temporal queries.
The  main objectives of TerraLib are:
\itemize{
	\item Encapsulate various DBMS, for example MySQL, PostgreSQL, PostGIS and Oracle-spatial;
	\item Give high-level support do spatial operations;
	\item Enable database access without directly using SQL queries;
	\item Retrieve spatial objects from DBMS in a proxy-like way.
}

The main motivation for the package development is to enable
the access to geographical databases and spatial/temporal operations
on them. TerraLib provides its own way to manipulate geographical databases,
and it has several internal tables to control geospatial data and
their relations. For mode information about TerraLib, please visit \url{http://www.terralib.org}.
}

\section{Classes}{
aRT encapsulates the main TerraLib concepts through the following classes:
\describe{
	\item{\code{aRTconn}:}{Stores a virtual DBMS connection and creates \code{aRTdb} objects.
		It can perform database administration tasks such as adding and removing permission to users.}
	\item{\code{aRTdb}:}{Stores a real connection to a database. 
		It can open/create aRTlayer objects  and open aRTtheme objects.}
	\item{\code{aRTlayer}:}{Represents an information layer inside a database.
		It performs spatial queries such as metrics and relations over its geometries.
		Each layer manipulates only geometry, but it can open/create \code{aRTtable}, \code{aRTtheme}, 
		and \code{aRTquerier} objects.}
	\item{\code{aRTtable}:}{Stores a database table. It retrieves data as data.frames.
		Each table has one specific type, and it can be static, temporal, external or media.
		Whenever you read data from the table, you get the whole data without any restriction.
		The way to select rows and columns is using \code{aRTtheme}.}
	\item{\code{aRTtheme}:}{Joins spatial and attribute tables in only one object. 
		It can perform attribute restrictions and create \code{aRTquerier} objects.}
    \item{\code{aRTquerier}:}{Encapsulates a database query.
		It can be temporal, if it was created from a temporal theme and if a slicing was defined. 
		It can retrieve data row by row, slide by slide (if temporal).}
	\item{\code{aRTvisual}:}{Defines a color and style configuration to be used within \code{aRTtheme} objects.}
	\item{\code{aRTgeometry}:}{Manipulates TerraLib objects of vectorial data within R.}
	\item{\code{aRTraster}:}{Manipulates TerraLib grids within R.}
	\item{\code{aRTnetwork}:}{A set of lines to compute distances in non-Euclidean spaces. Under development.}
	\item{\code{aRTgpm}:}{A class for building Generalized Proximity Matrixes. Under development.}
  }
}

\references{
\url{http://www.est.ufpr.br/aRT}\cr\url{http://www.terralib.org}}
\author{Pedro Ribeiro de Andrade, Marcos Aurelio Carrero, Paulo Justiniano Ribeiro Junior}
\keyword{database}

\seealso{\code{CLASSNAME-class} for links to \code{CLASSNAME}}
% vim: syntax=tex
