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

# these variables have the same name as terraLib objects
# functions visual{Points,Polygons,Lines,Raster} have as parameters
# names used in R (lwd,... 
setClass("aRTvisual",
    representation(color   = "matrix",
	               ccolor  = "matrix",
                   style   = "integer",
                   transp  = "integer",
                   ctransp = "integer",
                   cwidth  = "integer",
                   width   = "integer",
				   size    = "integer")
)

setMethod("initialize",
          "aRTvisual",
          function(.Object,
                   color   = "white",
				   ccolor  = "black",
                   style   = 1,
                   transp  = 0,
                   ctransp = 0,
                   cwidth  = 1,
                   width   = 1,
				   size    = 5)
{
   .Object@color   = col2rgb(color)
   .Object@ccolor  = col2rgb(ccolor)
   .Object@style   = as.integer(style)
   .Object@transp  = as.integer(transp)
   .Object@ctransp = as.integer(ctransp)
   .Object@cwidth  = as.integer(cwidth)
   .Object@width   = as.integer(width)
   .Object@size    = as.integer(size)
   
   .Object
})

setMethod("show", "aRTvisual", function(object)
{
	cat(paste("Object of Class aRTvisual\n\n"))
	
	cat(paste("Number of Colors:",     length(object@color)/3,  "\n"))
	cat(paste("Contour colors:",       length(object@ccolor)/3, "\n"))
	cat(paste("Style:",                object@style,            "\n"))
	cat(paste("Transparency:",         object@transp,           "\n"))
	cat(paste("Contour transparency:", object@ctransp,          "\n"))
	cat(paste("Width:",                object@width,            "\n"))
	cat(paste("Contour width:",        object@cwidth,           "\n"))
	cat(paste("Size:",                 object@size,             "\n"))
	
	return(invisible())
})

setMethod("getVisual", "aRTtheme", function(object) {
  params = .aRTcall(object, "cppGetVisual")
  return (new("aRTvisual",
             color = params$color,
			 transp = params$transp))
})

setMethod("setVisual", "aRTtheme", function(object,
                                            visual,
											attribute = NULL,
											mode = c("equalsteps",
											         "stddeviation",
													 "quantil",
													 "uniquevalue",
													 "raster")) {
  mode = match.arg(mode)
  .aRTcall(object, "cppSetVisual", visual = getVisual(visual), attribute = attribute, mode = mode)
  return(invisible())
})

setMethod("getVisual", "aRTvisual", function(object) {
	visual = object
	list(color   = visual@color,
	     ccolor  = visual@ccolor,
         style   = visual@style,
         width   = visual@width,
         cwidth  = visual@cwidth,
	     transp  = visual@transp,
		 ctransp = visual@ctransp,
		 size    = visual@size)
})

visualPoints = function(pch = "x", color = "black", size = 5)
{
         if(pch == 19)  pch = 2
	else if(pch == 21)  pch = 6
	else if(pch == 15)  pch = 7 # it was 22. Followed Elias tips
	else if(pch == 23)  pch = 8
	else if(pch == 26)  pch = 4
	else if(pch == 27)  pch = 5
	else if(pch == "+") pch = 0
	else if(pch == "*") pch = 1
	else if(pch == "x") pch = 3
	else stop("unrecognized pch\n")

  new("aRTvisual", color=color, style=pch, size=size)
}

visualPointsSlice = function(pch = "x", color = terrain.colors(10), size = 5, ...)
{
	visualPoints(pch, color, size, ...)
}

visualLines = function(color = "black", lwd=0, ...)
{
#  if(lty == 1) lty = 2 # TODO
  new("aRTvisual", color=color, style = lwd, ...)
}

visualPolygons = function(...)#color, lcolor)
{
#  if(lty == 1) lty = 2 # TODO

  new("aRTvisual", ...)#color="white", lcolor="black", style=lty)
}

visualRaster = function(color = terrain.colors(10))
{
  new("aRTvisual", color = color)
}

