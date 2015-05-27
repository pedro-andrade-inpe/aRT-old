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

setMethod("initialize", "aRTgeometry", function(.Object, data = NULL, .external = NULL)
{
  if( !is.null(.external) ) { pointer = .external}
  else pointer = { .Call("cppGeometry", data, PACKAGE="aRT")}

  .Object@pointer <- pointer
  reg.finalizer(.Object@pointer, .aRTremove)
  .Object
})

setMethod("getGeometry", "aRTgeometry", function(object, calcal=TRUE)
{
  data = .aRTcall(object, "cppGetGeo")

  sum = summary(object)

  if(sum$type == "points")
  {
    xy.sp=SpatialPoints(data$mat)
    df = data.frame(ID = data$ID)
    data = SpatialPointsDataFrame(xy.sp, df)		
  }
  else if(sum$type == "lines")    { data = SpatialLines(data) }
  else if(sum$type == "polygons") { data = SpatialPolygons(data, 1:length(data)) }
  else if(sum$type == "cells")    { data = SpatialPolygons(data, 1:length(data)) }

  if(calcal && sum$type == "polygons")
  {
    for(i in 1:length(data@polygons))
    {
      for(j in 1:length(data@polygons[[i]]@Polygons))
      {
        x = sp:::.spFindCG(data@polygons[[i]]@Polygons[[j]]@coords)
        data@polygons[[i]]@Polygons[[j]]@labpt = x$cents
        data@polygons[[i]]@Polygons[[j]]@area  = x$area
      }

      areas <- sapply(data@polygons[[i]]@Polygons, function(x) slot(x, "area"))
      Sarea <- sum(abs(areas))
      data@polygons[[i]]@area=Sarea

      marea <- which.max(areas)
      which_list <- ifelse(length(data@polygons[[i]]@Polygons) == 1, 1, marea)


      lpt <- t(sapply(data@polygons[[i]]@Polygons, function(x) slot(x, "labpt")))
      labpt <- lpt[which_list,]
      data@polygons[[i]]@labpt = as.numeric(labpt)
    }
  }

  data
})

setMethod("simplify", "aRTgeometry", function(object, snap, maxdist)
{
	.aRTcall(object,"cppSimplify",snap=snap,maxdist=maxdist)
	return(invisible())
})

setMethod("removeDup", "aRTgeometry", function(object)
{
	.aRTcall(object,"cppRemoveDuplicated")
	return(invisible())
})

setMethod("getClip", "aRTgeometry", function(object, geometry, as.sp = TRUE)
{
  result = .Call("cppGetClip", object@pointer, geometry@pointer, PACKAGE = "aRT")

  data = new("aRTgeometry", .external = result)
  if(as.sp) data = getGeometry(data)
  data
})

