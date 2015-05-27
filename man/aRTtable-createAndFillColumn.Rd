\name{aRTtable-createAndFillColumn}

\alias{createAndFillColumn}
\alias{createAndFillColumn,aRTtable-method}
\alias{fillCell}
\alias{fillCells}

\title{Creates and fills a column}

\description{
	Create a new column in a table of a cellular layer, 
    filling it with the result of a spatial operation, which can also involve 
    some attribute.
}
\usage{
\S4method{createAndFillColumn}{aRTtable}(object, newatt="", target, strategy=
c("average", "stdev", "sum", "minimum", "maximum", "majority", "distance", "xdistance", "count", "presence", "area", "length", "averagewba", "sumwba", "percentage"), geometry=c("point", "line", "polygon", "raster"), att="", proportion=FALSE, itime="", ftime="", maxerror=50, raster=NULL, validValues=NULL, dummy=NULL, default=NULL, band=NULL)}

\arguments{
  \item{object}{Object of class \code{aRTtable}.}
  \item{newatt}{The name of the new column.}
  \item{target}{An \code{aRTraster} or \code{aRTtheme} object.}
  \item{strategy}{The way to calculate the attribute of each cell:
    \itemize{
      \item area: Total area of overlay between the cell and a layer of polygons.
      \item average: Average of an attribute of the objects that have some intersection with the cell, without taking into account their geometric properties. 
      \item averagewba: Average weighted by area, based on the proportion of the intersection area. Useful when you want to distribute atributes that represent averages, such as per capita income.
      \item count: Number of objects that have some overlay with the cell (requires argument geometry).
      \item distance: Distance to the nearest object of a chosen geometry (requires argument geometry).
      \item length: Total length of overlay between the cell and a layer of lines.
      \item minimum: Minimum value of an attribute among the objects that have some intersection with the cell, without taking into account their geometric properties.
      \item maximum: Maximum value of  an attribute among the objects that have some intersection with the cell, without taking into account their geometric properties.
      \item majority: More common value in the objects that have some intersection with the cell, without taking into account their geometric properties.
      \item percentage: Percentages of each class of a raster data. It creates one attribute for each class of the raster.
      \item presence: Boolean value pointing out whether some object has an overlay with the cell.
      \item stdev: Standard deviation of an attribute of the objects that have some intersection with the cell, without taking into account their geometric properties.
      \item sum: Sum of an attribute of the objects that have some intersection with the cell, without taking into account their geometric properties.
      \item sumwba: Sum weighted by area, based on the proportion of the intersection area. Useful when you want to preserve the total amount in both layers, such as population size.
      \item xdistance: Approximated distance to the nearest object of a chosen geometry (requires arguments geometry and maxerror).
    }
  }
  \item{geometry}{The geometry to compute the attribute. One of "point", "polygon", "line", "raster".}
  \item{att}{The atribute used to compute the new column. It is only required when the strategy uses an attribute (such as average, sum, etc.)}
  \item{proportion}{Whether the calculation will be based on the intersection area, or the weights are equal for each object whth some overlap.}
  \item{itime}{The initial time (for temporal tables).}
  \item{ftime}{The final time (for temporal tables).}
  \item{maxerror}{The maximum error when calculating the distance operations.}
  \item{raster}{An object of class \code{aRTraster}, used only in the strategy \dQuote{weighbra}.}
  \item{validValues}{A set of valid raster values, used only in the strategy \dQuote{weighbra}. The other values are ignored.}
  \item{dummy}{A value that will ignored when computing the strategy, used only for raster strategies.}
  \item{default}{A value that will be used to fill a cell whose attribute cannot be computed. Used only for raster strategies.}
  \item{band}{An integer value representing the band of the raster to be used. This argument is used only in the strategy \dQuote{weighbra}.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	See \code{aRTtable-class} to show the description of this class. See \code{\link{createLayer}} to know how to
    create a layer of cells. 
}
% vim: syntax=tex
