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

setMethod("initialize", "aRTraster", function(.Object, data = NULL, .external = NULL)
{   
  if( !is.null(.external) ) pointer = .external
  else
  { 
    if(!is(data,"SpatialGridDataFrame") && !is(data,"SpatialPixelsDataFrame"))
    {
        stop("data must be SpatialGridDataFrame or SpatialPixelsDataFrame")
    }                                 

    if(is(data,"SpatialPixelsDataFrame")) fullgrid(data)=TRUE
    data = as.image.SpatialGridDataFrame(data[1], 1, 2)
    
    if(!any(names(data)=="z")) stop ("'value' must have a z attribute")
    if(!any(names(data)=="x")) stop ("'value' must have a x attribute")
    if(!any(names(data)=="y")) stop ("'value' must have a y attribute")

    x = range(data$x)
    lines = dim(data$z)[1]
    diff = x[2] - x[1]             
    diff = (diff / (lines - 1)) / 2
    x[1] = x[1] - diff             
    x[2] = x[2] + diff

    y = range(data$y)
    columns = dim(data$z)[2]
    diff = y[2] - y[1]             
    diff = (diff / (columns - 1)) / 2
    y[1] = y[1] - diff             
    y[2] = y[2] + diff

    pointer = .Call("cppRaster", list(x=x,
                                      y=y,
                                      z=as.numeric(data$z),
                                      dim = dim(data$z)), PACKAGE="aRT")
  } 

  .Object@pointer <- pointer
  reg.finalizer(.Object@pointer, aRT:::.aRTremove)
  .Object
})

setMethod("getRaster", "aRTraster", function(object, as.sp=TRUE)
{
  raster = .aRTcall(object, "cppGetRast")

  if(as.sp)
  {
    raster <- cbind(expand.grid(x = raster$x, y = raster$y), data=as.vector(raster$z))
    coordinates(raster) <- c("x", "y")
    gridded(raster) <- TRUE
    fullgrid(raster)=TRUE
  }

  raster
})

setMethod("getPixels", "aRTraster", function(object, geometry, mode=NULL)
{
	.Call("cppGetPixels", object@pointer, list(mode=mode), geometry@pointer, PACKAGE="aRT")
})


