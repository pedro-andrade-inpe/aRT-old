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

setMethod("initialize", "aRTlayer", function(.Object,
                                             db,
											 layername,
											 create = FALSE,
											 proj = NA,
                                             .external = NULL)
{
  if( !is.null(.external) ) ## TODO: what is it for???
  {
    .Object@pointer <- .external
    return(.Object)
  }

  if (!is(db, "aRTdb"))   stop("Argument \"db\" must be an aRTdb object\n")
  if (is.null(layername)) stop("layername cannot be NULL")

  if(is.na(proj)) proj = ""

  if(create)
  {
    out <- .aRTcall(db,
                    "cppDbCreateLayer",
                    layername=layername,
			        projection=proj)
  }
  else # open
  {
    out <- .aRTcall(db,
                    "cppDbOpenLayer",
                    layername=layername)
  }

  if (is.null(out))
    stop("Could not create the aRTlayer object\n")
  .Object@pointer <- out
  reg.finalizer(.Object@pointer, .aRTremove)
  .Object
})

setMethod("showTables", "aRTlayer", function(object) { .aRTcall(object, "cppLShowTables") })

setMethod("getID", "aRTlayer", function(object) {
    t = openTable(object)
    key = summary(t)$keys[1]
    return(getData(t)[,key])
})

# import a static table
setMethod("importTable", "aRTlayer", function(object, table, ID, data, ...)
{
	if(is(data, "Spatial")) data = data@data

	ID = casefold(ID)
	t = new("aRTtable", object, table=table, ID=ID, c=TRUE, gen=FALSE, ...)

	colnames(data) = casefold(colnames(data))
	colnames = colnames(data)
    classes = substr(sapply(data, class),1,1)
    for(i in 1:length(colnames))
    {
		if(colnames[i] != ID)
		{
			createColumn(t,
			             colnames[i],
						 type = classes[i],
						 length=max(nchar(as.matrix(data[i]), type="byte")))
		}
    }
	addRows(t, data.frame(data))
	t
})


setMethod("createTheme", "aRTlayer", function(object, themename=summary(object)$layer, tables=NULL, viewname=themename, ID=NULL, attrest="")
{
    new("aRTtheme", object, theme=themename, c=TRUE, tables=tables, viewname=viewname, attrest=attrest, ID=ID)
})

setMethod("openTheme", "aRTlayer", function(object, tname) {
    new("aRTtheme", object, tname)
})


setMethod("addShape", "aRTlayer", function(object, file, table, ID="", length=10)
{
	.aRTcall(object,
	         "cppAddShape", 
	         filename = file,
 	         tablename = table,
	 	     ID = ID,
			 length = as.integer(length))
	return(invisible())
})

setMethod("addPoints", "aRTlayer", function(object, points) {
    if(is(points,"SpatialPointsDataFrame"))
	{
		if(is.factor(points[["ID"]])) points[["ID"]] = paste(points[["ID"]])
		if(!is.character(points[["ID"]])) stop("ID has an invalid type (use character or factor)")
		points = as.aRTgeometry(points)
	}

    if(class(points) != "aRTgeometry")
        stop("'points' must be SpatialPointsDataFrame or aRTgeometry")
				
	.Call("cppAddPoints", object@pointer, points@pointer, PACKAGE="aRT")
	return(invisible())
})

setMethod("addPolygons", "aRTlayer", function(object, polygons) {
    if(is(polygons,"SpatialPolygons")) polygons = as.aRTgeometry(polygons) 

	if(class(polygons) != "aRTgeometry")
		stop("'polygons' is an invalid object")
	
	.Call("cppAddPolygons", object@pointer, polygons@pointer, PACKAGE="aRT")
	return(invisible())
})

setMethod("addLines", "aRTlayer", function(object, lines) {
	if(is(lines, "SpatialLines")) lines = as.aRTgeometry(lines)
	
	if(class(lines) != "aRTgeometry")
	        stop("'lines' is an invalid object")
			
	.Call("cppAddLines", object@pointer, lines@pointer, PACKAGE="aRT")

	return(invisible())
})

setMethod("addRaster", "aRTlayer", function(object, raster) {
  if(!is(raster,"aRTraster")) raster = new("aRTraster",raster)
    
  if(summary(object)$raster == "yes") stop("the layer already has raster data")

  .Call("cppAddRaster", object@pointer, raster@pointer, PACKAGE="aRT")
  return(invisible())
})

setMethod("getProj", "aRTlayer", function(object) {
  .aRTcall(object, "cppGetProj")
})

setMethod("getBox", "aRTlayer", function(object) {
  .aRTcall(object, "cppGetBox")
})

setMethod("getNN", "aRTlayer", function(object, layer=object, ID, quantity=1) {
	.Call("cppGetNearestNeighbors",
		  object@pointer,
		  list(ID=ID,
		       quantity=as.integer(quantity)),
		  layer@pointer,
		  PACKAGE="aRT")
})

setMethod("getDistance", "aRTlayer", function(object, ID, layer=object) {
#	if(!is.null(layer)) stop("Distances between tow layers not supported yet")
	.Call("cppGetDistance",
	      object@pointer,
		  ID,
		  layer@pointer,
		  PACKAGE="aRT")
})

setMethod("getRelation", "aRTlayer", function(object,
                                              relation = c("contains",
											               "overlaps",
    													   "crosses",
														   "disjoint",
														   "touches",
														   "within",
														   "intersects",
														   "equals",
														   "covers",
														   "coveredby"),
											  layer=object,
											  ID=NULL) {

  if(length(relation) > 1)
  {
    resp = getRelation(object, relation[1], layer, ID)
    for(rel in 2:length(relation))
    {
		resp = c(resp, getRelation(object, relation[rel], layer, ID))
    }
    return(unique(resp))
  }

  relation = match.arg(relation)
  result = .Call("cppGetRelation",
                   object@pointer,
                   list(relation=relation,
                        ID=ID),
		           layer@pointer,
                   PACKAGE="aRT")
  if(summary(object)$raster == "no") result = unique(result)
  if(summary(layer)$layer == summary(object)$layer) # remove the object itself from the list
  {
    result = result[which(is.na(match(result,ID)))]
  }
  result
})

setMethod("getSetOperation", "aRTlayer",
function(object,
         operation = c("union",
		               "intersection",
					   "difference",
					   "xor"),
         ID = NULL,
		 as.sp = TRUE)
{
  operation = match.arg(operation)

  if(is.null(ID))
  {
    if(any(operation == c("difference", "xor")))
      stop("This operation requires two object IDs")
    else
    {
      t = openTable(object)
      ID = aRT::getData(t)[[summary(t)$keys[1]]]
    }
  }

  result <- .aRTcall(object, "cppGetSetOperation", operation=operation, ID=ID)

  data = new("aRTgeometry", .external = result)
  if(as.sp) data = getGeometry(data)
  # to solve a minor terralib bug
  if(data@polygons[[1]]@ID == "")
  {
  	data@polygons[[1]]@ID = "1"
  }

  data
})

setMethod("getOperation", "aRTlayer",
function(object,
         operation = c("centroid",
				       "hull",
					   "buffer"),
		 ID = NULL,
		 distance = NULL,
		 as.sp = TRUE)
{

  operation = match.arg(operation)
  result <- .aRTcall(object, "cppGetOperation", operation=operation, dist=distance, ID=ID)

  data = new("aRTgeometry", .external = result)
  if(as.sp) data = getGeometry(data)
  data
})


setMethod("getMetric", "aRTlayer", function(object,
                                            metric = c("area",
											           "length"),
#													   "distance",
#													   "withinDist"),
											ID = NULL) {
#											distance = NULL){
#											origin = NULL) {
  metric = match.arg(metric)
  .aRTcall(object, "cppGetMetric", metric=metric, ID=ID)
})

setMethod("getPoints", "aRTlayer", function(object, table=NULL, as.sp=TRUE, slice=FALSE) {
	if(!is.null(table) || slice) as.sp = TRUE
	su = summary(object)
    if(su$points == 0) return(NULL)

	q = openQuerier(object, geom = "points")

	e = summary(q)$elements
	if(!slice) slice = e
	first = e%%slice
	if(first == 0) first = slice

	data = aRT::getData(q, first, as.sp=as.sp)

	qtde = ((e-first)/slice)-1
	if(qtde > 0)
	{
		for(i in 1:qtde)
		{
			ptsaux = aRT::getData(q, slice)
	
			data@coords = rbind(data@coords, ptsaux@coords)
			data@data   = rbind(data@data,   ptsaux@data)
		}
		s = getBox(object)
		data@bbox = bbox(cbind(x=s$x, y=s$y))
	}
	
	if( !is.null(table) )
	{
		by.y = summary(table)$keys

		if( is.null(by.y)     ) by.y = "id"
		if( length(by.y) == 0 ) by.y = "id"
		if( length(by.y) > 1  ) stop("Table must have only one unique ID in this function")

		d = aRT::getData(table)

		data.merged=merge(data.frame(data), d, by.x="ID", by.y=by.y)
		coordinates(data.merged) = c("coords.x1","coords.x2")
		data = data.merged
	}

	return(data)
})

setMethod("getLines", "aRTlayer", function(object, as.sp=TRUE, slice=FALSE) {
	if(slice) as.sp = TRUE
	su = summary(object)
	if(su$lines == 0) return(NULL)

	q = openQuerier(object, geom = "lines")

	e=summary(q)$elements
	if(!slice) slice = e
	first = e%%slice
	if(first == 0) first = slice

	data = aRT::getData(q, first, as.sp=as.sp)

	qtde = (e-first)/slice
	if(qtde > 0)
	{
		for(i in 1:qtde)
		{
			geomaux = aRT::getData(q, slice)
	
			data@lines = c(data@lines, geomaux@lines)
		}
		s = getBox(object)
		data@bbox = bbox(cbind(x=s$x, y=s$y))
	}

	return(data)
})

setMethod("getPolygons", "aRTlayer", function(object, as.sp=TRUE, slice=FALSE) {
	if(slice) as.sp = TRUE
	su = summary(object)
	if(su$polygons == 0) return(NULL)

	q = openQuerier(object, geom = "polygons")

	e=summary(q)$elements
	if(!slice) slice = e
	first = e%%slice
	if(first == 0) first = slice

	data = aRT::getData(q, first, as.sp=as.sp)

	qtde = (e-first)/slice
	if(qtde > 0)
	{
		for(i in 1:qtde)
		{
			geomaux = aRT::getData(q, slice)
	
			data@polygons = c(data@polygons, geomaux@polygons)
		}
		s = getBox(object)
		data@bbox = bbox(cbind(x=s$x, y=s$y))
		data@plotOrder = 1:(length(getID(data)))
	}

	return(data)
})

setMethod("getCells", "aRTlayer", function(object, as.sp=TRUE, slice=FALSE) {
	if(slice) as.sp = TRUE
	su = summary(object)
	if(su$cells == 0) return(NULL)

	q = openQuerier(object, geom = "cells")

	e=summary(q)$elements
	if(!slice) slice = e
	first = e%%slice
	if(first == 0) first = slice

	data = aRT::getData(q, first, as.sp=as.sp)

	qtde = (e-first)/slice
	if(qtde > 0)
	{
		for(i in 1:qtde)
		{
			geomaux = aRT::getData(q, slice)
	
			data@polygons = c(data@polygons, geomaux@polygons)
		}
		s = getBox(object)
		data@bbox = bbox(cbind(x=s$x, y=s$y))
		data@plotOrder = 1:(length(getID(data)))
	}

	return(data)

})

setMethod("getRaster", "aRTlayer", function(object, as.sp=TRUE) {
  raster=.aRTcall(object, "cppGetRaster")

  raster = new("aRTraster",.external=raster)
  if(as.sp)
  {
	raster = getRaster(raster)
  }
  
  raster
})

setMethod("openQuerier", "aRTlayer", function(object, geom = c("", "points", "lines", "polygons", "cells"))
{
  geom = match.arg(geom)
  err = "Layer has more than one geometry, choose one value for geom"
  if(geom == "") # if there is more than one geometry and the user
                 # does not specify the geom we stop with an error
  {
    sum = summary(object)

    if(sum$polygons > 0) geom = "polygons"
    if(sum$lines > 0   ) { if(geom == "") geom = "lines"  else stop(err) }
    if(sum$points > 0  ) { if(geom == "") geom = "points" else stop(err) }
    if(sum$cells > 0  )  { if(geom == "") geom = "cells"  else stop(err) }
    if(geom == "") stop("Layer does not have any geometry")
  }

  new("aRTquerier", object, loadatt=FALSE, geom=geom)
})

setMethod("openTable", "aRTlayer", function(object, tablename=NULL, type="default") {

	if(type=="media")
		ret = new("aRTtable", object, type=type)
	else
	{	
        if(is.null(tablename)) tablename = showTables(object)[1]

		if(!any(showTables(object) == tablename))
    	    stop("Table does not exist\n")
		ret = new("aRTtable", object, tab=tablename, type=type)
	}
	return(ret)
})

setMethod("createTable", "aRTlayer", function(object,
                                              tablename=NULL,
                                              genID=NULL,
											  type="static",
											  ID="ID",
											  link=ID,
											  length=16,
											  itimeatt="time",
											  ftimeatt="time") {
	if(type=="media")
		ret = new("aRTtable", object, type=type, link=link, c=TRUE, length=length, itimeatt=itimeatt, ftimeatt=ftimeatt)
	else
	{
		if(any(showTables(object) == tablename))
        	stop("Table already exists\n")
		ret = new("aRTtable", object, tab=tablename, gen=genID, type=type, ID=ID, link=link, c=TRUE, length=length, itimeatt=itimeatt, ftimeatt=ftimeatt)
	}
	return(ret)
})

setMethod("createLayer", "aRTlayer", function(object, lname, rx, ry=rx, allbox=FALSE) {
    pointer = .aRTcall(object, "cppCell", lname=lname,rx=rx,ry=ry,allbox=allbox)
    return(new("aRTlayer",NULL,"",.external=pointer))
})

