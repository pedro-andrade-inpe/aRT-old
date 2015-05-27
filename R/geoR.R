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

# geoR.R
# functions to convert data from and to _geoR_ package
# gr == geoR

".aRT2gr" <- function(x)
{
	stop("not implemented yet")
}

".gr2aRTpoints" <- function(x)
{
  if(any(names(x) == "coords"))  x <- x$coords

  x <- as.matrix(x)
  if(!is.matrix(x)) stop("argument must be a matrix or data-frame")
  pts <- list()
  pts$id <- as.character(1:nrow(x))
  colnames(x) <- c("x", "y")
  pts$points <- apply(x, 1, as.list)
  class(pts)="aRTgeometry"
  return(pts)
}

".gr2aRTraster" <- function(x)
{
  names(x)[3] <- "z"
  return(x)
}

".gr2aRTpolygons" <- function(x)
{
  if(any(names(x) == "borders")) x <- x$borders

  pols <- list()
  pols$polygons<-list(list(x=x[,1], y=x[,2]))
  pols$id<-paste(1:length(pols$polygons))
  class(pols) = "aRTgeometry"
  return(pols)
}

".gr2aRTattributes" <- function(x)
{
  if(any(names(x) == "data")) x <- x$data


  if(is.vector(x)){
    # Paulo, mudei esta funcao names(x)<-xname, foi para "data"
    xname <- deparse(substitute(x))
    x <- as.data.frame(x)
    names(x) <- "data"
  }
  if(is.matrix(x))
    x <- as.data.frame(x)
  artdata <- list(id = as.character(1:nrow(x)))
  artdata <- c(artdata, as.list(x))
  return(artdata)
}

