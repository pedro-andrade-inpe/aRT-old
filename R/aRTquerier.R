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

setMethod("initialize", "aRTquerier",
		  function(.Object,
                   parent,
                   geom = c("", "points","lines","polygons", "cells", "none"),
                   loadatt = TRUE,
				   table = "",
				   att = "",
				   chronon = c("nochronon", "second", "minute",
							   "hour", "day", "month", "year",
    						   "dayofweek", "dayofmonth", "dayofyear",
							   "monthofyear", "season", "weekofyear",
							   "hourofday", "minuteofhour", "secondofminute"),
				   stat = c("nostat", "sum", "max", "min",
							"count", "validcount", "stddev",
							"kernek", "mean", "variance",
							"skewness",	"kurtosis",	"median",
							"varcoeff",	"mode"))
{

  stat    = match.arg(stat)
  chronon = match.arg(chronon)
  geom    = match.arg(geom)

  if (is(parent, "aRTlayer"))
  {
    if(stat    != "nostat")    stop("Can't use stat when building a query from layer")
    if(chronon != "nochronon") stop("Can't use chronon when building a query from layer")
    out = .aRTcall(parent, "cppLayerOpenQuerier", geom=geom, loadatt=loadatt)
  }
  else if (is(parent, "aRTtheme"))
  {
	sum = summary(parent)

	if(table != "" && !any(names(sum$tables) == table)) stop("No table choosed")
	if(att   != "" && !any(sum$tables[[table]] == att)) stop("Attribute does not exist")
  
	out = .aRTcall(parent,
                   "cppThemeOpenQuerier",
                   geom=geom,
				   loadatt=loadatt,
				   stat=stat,
				   chronon=chronon,
				   table=table,
				   att=att)
  }
  else stop("Argument \"parent\" must an object of class aRTlayer or aRTtheme")

  if (is.null(out))
    stop("Could not create the aRTquerier object")

  .Object@pointer <- out
  reg.finalizer(.Object@pointer, .aRTremove)
  .Object
})

setMethod("nextFrame", "aRTquerier", function(object)
{
  .aRTcall(object, "cppNextFrame")
})

setMethod("getData", "aRTquerier", function(object, quantity = -1, as.sp=TRUE) {
  if(quantity < 0) quantity = 0
  result = .aRTcall(object,
                    "cppGetQuerierData",
	                quantity=as(quantity, "integer"))

  if(class(result) == "externalptr") # without attributes
  {
     result = new("aRTgeometry", .external = result)
     if(as.sp) result = getGeometry(result)
  }
  else if(class(result) == "list") # with attributes
  {
	geom = new("aRTgeometry", .external = result$geometry)
	geom = getGeometry(geom)

	if( !any(duplicated(getID(geom))) ) rownames(result$attributes) = getID(geom)

	if(class(geom) == "SpatialPolygons")          result = SpatialPolygonsDataFrame(geom, result$attributes)
	if(class(geom) == "SpatialLines")             result = SpatialLinesDataFrame   (geom, result$attributes)
	if(class(geom) == "SpatialPointsDataFrame") { result = SpatialPointsDataFrame  (geom, result$attributes)
	                                              rownames(result@coords) = rownames(result@data) }
  }
 
  result
})

