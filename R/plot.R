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

setMethod("plot", "aRTtheme", function(x, y, ...) {
  out <- openQuerier(x, loadatt=FALSE)
  plot(aRT::getData(out), ...)
  return(invisible())
})

setMethod("plot", "aRTlayer", function(x, y, ...) {
  summ = summary(x)
  iferr = function(xx) { if(is.null(xx)) stop("Error loading data") }
  if(summ$raster == "yes") { out <- getRaster(x);   iferr(out); image(out, asp=1, ...) }
  if(summ$polygons > 0   ) { out <- getPolygons(x); iferr(out); plot(out, ...)      }
  if(summ$lines > 0      ) { out <- getLines(x);    iferr(out); plot(out, ...)      }
  if(summ$points > 0     ) { out <- getPoints(x);   iferr(out); plot(out, ...)      }
  if(summ$cells > 0      ) { out <- getCells(x);    iferr(out); plot(out, ...)      }
  box()
  return(invisible())
})

