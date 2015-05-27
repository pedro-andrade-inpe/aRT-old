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

setMethod("initialize", "aRTtheme", function(.Object,
                                             parent = NULL,
											 themename = summary(parent)$layer,
											 create = FALSE,
											 tables = NULL,
											 viewname = themename,
                                             attrest = "",
                                             ID = NULL)
{
# if tables == NULL the theme can't be created
# tables == showTables(aRTlayer) does not warrant that the theme will be opened,
# because TerraLib has some invalid table combinations (TODO: put it into help)
# tables and viewname are needed only when a new theme is going to be created
# themes can be created only from aRTlayers
  if (is.null(themename)) stop("themename cannot be NULL")

  if (is(parent, "aRTlayer"))
  {
	if (is.null(tables) && summary(parent)$raster == "no") tables = showTables(parent)

	if(!is.null(ID) )
    {
		if(is.null(tables)) stop("Can't select ID without a table\n")
		
		t = new("aRTtable", parent, table=tables[1])

		key = summary(t)$keys[1]

		if(is.na(key)) key = "id"
        
		# if attrest != "" we will add the two restrictions to the theme
		if(attrest == "") rest = paste(key, " = '", ID[1], "'", sep="")
		else              rest = paste(attrest, " and (", key, " = '", ID[1], "'", sep="")
		
		for(i in 2:length(ID))
		{
			rest = paste(rest, " or ", key, " = '", ID[i], "'", sep="")
		}

		if(attrest != "") rest = paste(rest, ")")

		attrest = rest
	}

    if(create)
    {
      out <- .aRTcall(parent,
	                  "cppLayerCreateTheme",
					  themename = themename,
			          viewname = viewname,
			          tables = tables,
					  attrest = attrest)
    }
    else
    {
      out <- .aRTcall(parent,
	                  "cppLayerOpenTheme",
					  themename = themename)
    }
  }
  else if(is(parent, "aRTdb"))
	  out <- .aRTcall(parent,
	                  "cppDbOpenTheme",
					  themename = themename)
  else stop("Invalid parent object")
  
  if (is.null(out)) stop("Could not create the aRTtheme object\n")

  .Object@pointer <- out
  reg.finalizer(.Object@pointer, .aRTremove)
  .Object
})

setMethod("setVisible", "aRTtheme", function(object, points=NULL, lines=NULL, polygons=NULL, check=NULL)
{
  if(!is.logical(c(points, lines, polygons, check)))
  	stop("All arguments must be logical values")

  .aRTcall(object,"cppSetVisible", points = points, lines = lines, polygons = polygons, check = check)
  return(invisible())
})

setMethod("getData", "aRTtheme", function(object) {
 q=openQuerier(object)
 aRT::getData(q, FALSE) # why FALSE? i cant explain...
})

setMethod("openQuerier", "aRTtheme", function(object, geom = c("", "points", "lines", "polygons", "none"), loadatt=TRUE, chronon = c("nochronon", "second", "minute", "hour", "day", "month", "year", "dayofweek", "dayofmonth", "dayofyear", "monthofyear", "season", "weekofyear", "hourofday", "minuteofhour", "secondofminute"), stat = c("nostat", "sum", "max", "min", "count", "validcount", "stddev", "kernek", "mean", "variance", "skewness", "kurtosis", "median", "varcoeff", "mode")) 
{
  geom = match.arg(geom)
  err = "Theme has more than one geometry, choose one value for geom"
  if(geom == "") # if there is more than one geometry and the user
                 # does not specify the geom we stop with an error
  {
    sum = summary(object)

    if(sum$polygons == "yes") geom = "polygons"
    if(sum$lines    == "yes") { if(geom == "") geom = "lines"  else stop(err) }
    if(sum$points   == "yes") { if(geom == "") geom = "points" else stop(err) }
    if(sum$cells    == "yes") { if(geom == "") geom = "cells"  else stop(err) }
    if(geom == "") stop("Theme does not have any geometry")
  }

  new("aRTquerier", object, geom=geom, loadatt=loadatt, chronon=chronon, stat=stat)
})

