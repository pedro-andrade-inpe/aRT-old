#  aRT : API R - TerraLib                                                
#  Copyright (C) 2003-2011  LEG                                          
#                                                                        
#  This program is free software; you can redistribute it and/or modify  
#  it under the terms of the GNU General Public License as published by  
#  the Free Software Foundation; either version 2 of the License, or     
#  (at your option) any later version.                                   
#                                                                        
#  This program is distributed in the hope that it will be useful,       
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
#  GNU General Public License for more details.                          
#                                                                        
#  You should have received a copy of the GNU Lesser General Public      
#  License along with this library.

require(sp)

#====================================================================
# preparing .aRTspdefs object which stores class definitions for
# building objects outside R.
# It is necessary to avoid gc() to kill them
.aRTspdefs = list()
.aRTspdefs$polygon  = methods:::getClassDef("Polygon")
.aRTspdefs$polygons = methods:::getClassDef("Polygons")
.aRTspdefs$line     = methods:::getClassDef("Line")
.aRTspdefs$lines    = methods:::getClassDef("Lines")

.aRTinitSpDefs = function()
{
    .Call("cppInitSpDefs", .aRTspdefs, PACKAGE="aRT")
    return(invisible())
}
#====================================================================

setMethod("as.aRTgeometry", "SpatialPoints", function(object) {
	stop(paste("aRT does not support objects of this class.",
	           "Use SpatialPointsDataFrame with an attribute called ID instead."))
})

setMethod("getID", "SpatialPointsDataFrame", function(object) as.vector(object@data$ID))

setMethod("getID", "SpatialPolygons", function(object) {
	Srs <- slot(object, "polygons")
	return(sapply(Srs, function(i) slot(i, "ID")))
})

setMethod("getID", "SpatialLines", function(object) {
	Sls <- slot(object, "lines")
	return(sapply(Sls, function(i) slot(i, "ID")))
})

setMethod("as.aRTgeometry", "SpatialPointsDataFrame", function(object) {
	if(!any(names(object@data) == "ID"))
		stop("The data.frame must have a column called 'ID'")
	
	new("aRTgeometry", object)
})

setMethod("as.aRTgeometry", "SpatialLines", function(object) {
	new("aRTgeometry", object)
})

setMethod("as.aRTgeometry", "SpatialPolygons", function(object) {
	new("aRTgeometry", object)
})

