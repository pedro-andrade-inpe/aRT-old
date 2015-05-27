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

setMethod("initialize", "aRTdb", function(.Object,
                                          conn,
										  dbname,
										  create = FALSE,
										  update = FALSE,
                                          views = "", # owner of the views. you can connect to a db with another user
										  .external = NULL)
{
  if( !is.null(.external) )
  {
  	cat("Using a TerraLib application external pointer\n")
	.Object@pointer <- .Call("cppOpenDbPointer",.external,PACKAGE="aRT")
	# never think in reg.finalizer. This pointer comes from TerraView!
	return(.Object)
  }

  if (!is(conn, "aRTconn")) stop("Argument \"conn\" must be an object of aRTconn\n")
  if (is.null(dbname))     stop("dbname can not be NULL")

  if(create)
  {
    out <- .aRTcall(conn,
                    "cppConnCreateDb",
		 	        dbname=dbname)
  }
  else # open
  {
    out <- .aRTcall(conn,
                    "cppConnOpenDb",
		 	        dbname=dbname,
			        update=as(update, "integer"))
  }

  if (is.null(out)) stop("Could not create the aRTdb object\n")

  .Object@pointer <- out
  reg.finalizer(.Object@pointer, .aRTremove)
  reload(.Object, views)
  .Object
})

setMethod("importSpData","aRTdb",function(object,spdata,lname,tname=lname,proj=NA,thname=NULL,view=thname){
	l=createLayer(object,lname,proj=proj)
	
	if(class(spdata) == "SpatialPoints")
		spdata=SpatialPointsDataFrame(spdata,data.frame(ID=I(paste(1:length(coordinates(spdata)[,1])))))

	if(is(spdata, "SpatialPoints"))   addPoints  (l,spdata)
	if(is(spdata, "SpatialPolygons")) addPolygons(l,spdata)
	if(is(spdata, "SpatialLines"))    addLines   (l,spdata)

	if(is(spdata,"SpatialPointsDataFrame"))
	{
		importTable(l,tname,"id",spdata@data)
	}
	else if(any(c(is(spdata, "SpatialPolygonsDataFrame"),is(spdata, "SpatialLinessDataFrame"))))
	{
		data = spdata@data
		if(!any(colnames(data) == "id"))
		{
			data=cbind(id=getID(spdata), data)
		}
		importTable(l,tname,"id",data)		
	}

	if(is.null(thname))	return(l)
	
	createTheme(l,thname,view=view)
})

setMethod("reload", "aRTdb", function(object, user="") {
	.aRTcall(object, "cppReload", user=user)
	return(invisible())
})

setMethod("openLayer", "aRTdb", function(object, lname) {
	if(!any(showLayers(object) == lname))
        stop("Layer does not exist\n")
	new("aRTlayer", object, l=lname)
})

setMethod("createLayer", "aRTdb", function(object, lname, proj=NA) {
	if(any(showLayers(object) == lname))
        stop("Layer already exists\n")
	new("aRTlayer", object, l=lname, c=T, proj=proj)
})

setMethod("showLayers", "aRTdb", function(object) { .aRTcall(object, "cppList")       })
setMethod("showTables", "aRTdb", function(object) { .aRTcall(object, "cppDShowTables")})
setMethod("showViews",  "aRTdb", function(object) { .aRTcall(object, "cppShowViews")  })
setMethod("showThemes", "aRTdb", function(object) { .aRTcall(object, "cppShowThemes") })

# import an external table
setMethod("importTable", "aRTdb", function(object, table, ID, data)
{
	if(is(data, "Spatial")) data = data@data
	
	t = new("aRTtable", db, table=table, ID=ID, c=T, gen=T)

    colnames = colnames(data)
    classes <- substr(sapply(data, class),1,1)
    for(i in 1:length(colnames))
    {
		if(colnames[i] != ID)
		{
			createColumn(t, colnames[i], type = classes[i], length=max(nchar(as.matrix(data[i]))))
		}
    }
	addRows(t, data)
	t
})

setMethod("importShape", "aRTdb", function(object, layer, proj = NA, ...) {
  if(any(showLayers(object) == layer))
      stop("Layer already exists\n")

  l = new("aRTlayer", object, l=layer, proj=proj, c=T)
  addShape(l, ...)
  return(l)
})

setMethod("importRaster", "aRTdb", function(object, file, lname, dummy) {
  if(any(showLayers(object) == lname))
      stop("Layer already exists\n")

  .aRTcall(object, "cppImportRaster", filename=file, layername=lname, dummy=dummy)

  l = openLayer(db, lname)
  return(l)
})

setMethod("deleteLayer", "aRTdb", function(object, layername, force=FALSE) {
	if(.aRTconfirm(force))
	    .aRTcall(object, "cppDeleteLayer", layername=layername)
	return(invisible())
})

setMethod("deleteTable", "aRTdb", function(object, tablename, force=FALSE) {
	if(.aRTconfirm(force))
    .aRTcall(object, "cppDeleteTable", tablename=tablename)
	return(invisible())
})

setMethod("deleteView", "aRTdb", function(object, viewname, force=FALSE) {
	if(.aRTconfirm(force))
    .aRTcall(object, "cppDeleteView", viewname=viewname)
	return(invisible())
})

setMethod("deleteTheme", "aRTdb", function(object, themename, force=FALSE) {
	if(.aRTconfirm(force))
    .aRTcall(object, "cppDeleteTheme", themename=themename)
	return(invisible())
})


setMethod("createTable", "aRTdb", function(object, tablename, ID="ID", length=16) {
	if(any(showTables(object) == tablename))
   	    stop("Table already exists\n")
	new("aRTtable", object, tablename=tablename, type="external", create=TRUE, ID=ID, length=length)
})

setMethod("openTheme", "aRTdb", function(object, tname) {
    new("aRTtheme", object, tname)
})


setMethod("openTable", "aRTdb", function(object, tablename=NULL, type="default") {
	if(type=="media")
		ret = new("aRTtable", object, type=type)
	else
	{	
		if(!any(showTables(object) == tablename))
    	    stop("Table does not exist\n")
		ret = new("aRTtable", object, tab=tablename, type=type)
	}
	return(ret)
})

setMethod("importMif", "aRTdb", function(object, file, lname) {
    pointer = .aRTcall(object, "cppImportMif", lname=lname,file=file)
    return(new("aRTlayer",NULL,"",.external=pointer))
})

