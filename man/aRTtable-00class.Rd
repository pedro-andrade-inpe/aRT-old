\name{aRTtable}
\docType{class}
\alias{aRTtable}
\alias{aRTtable-class}
\alias{initialize,aRTtable-method}

\title{Class aRTtable}
\description{Class that represents a DBMS table. It can be one of the various TerraLib
table types.

Tables are created from layers or directly from the database. When it is created from a layer, 
ther attributes are be linked to the geometries of that layer. When it is created from a database,
it does not have any geometry associated. For selecting data joining tables with the links use \code{aRTthemes}.}

\section{Objects from the Class}{
Objects can be created by calling \code{openTable()} or \code{createTable()}, from \code{aRTlayer} of form \code{aRTdb}.
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
	\item{addRows:}{Adds rows to the table.}
	\item{createColumn:}{Creates a column in the table.}
	\item{createAndFillColumn:}{Creates a column based on some operation of another layer. 
		It works only with tables of cellular layers.}
	\item{createRelation:}{Creates a relation between an attribute of the table and an attribute of another table.}
	\item{getData:}{Returns the table values.}
	\item{updateColumns:}{Adds columns and(or) update them.}
  }
}

\section{Table types}{
In TerraLib there are several types of table, implementing the entity relationship,
temporal and other models. They are:
\describe{
\item{static}{Table which each geomety has one, and only one, respective row.}
\item{media}{A media table has only two columns: object\_id and media\_name. 
      It associates an url to a geometry double-click in a TerraLib-based GIS.
      A layer can have only one media table, and this table does not have a name.}
\item{external}{Each row is not implicitly connected to a geometry, but
      the table can have foreign keys to all static, event and dynamoc tables.}
\item{event}{One geometry occours in a specific time interval, as murderers.}
\item{dynatt}{Attributes changing in the time, but the objects have static geometry.}
\item{alldyn}{Fully dynamic (attributes and geometry). (not implemented yet)}
\item{dyngeom}{Dynamic Geometry. (not implemented yet)}
\item{default}{The default value depending on the parent. If the class is aRTlayer,
      default will be static, else (aRTdb) it will be external.}
}
}

\author{Pedro Ribeiro de Andrade}
\keyword{classes}
\keyword{chron}
\seealso{
 See \code{aRTtable-METHOD} for a method of this class or \code{CLASSNAME-class} for links to other classes.
}

% vim: syntax=tex
