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

# generateIDs is used in static tables for generating IDs from the
# layer geometries.
# only external tables are allowed to be built from aRTdb's
setMethod("initialize", "aRTtable", function(.Object, 
                                             parent,
                                             tablename = NULL,
										     create = FALSE,
											 type = c("default",
                                                      "static",
                                                      "media",
                                                      "external",
                                                      "event",
                                                      "dynatt",
                                                      "alldyn",
                                                      "dyngeom"),
											 genID = NULL,
											 ID= "ID",
											 link = "ID",
											 length = 16,
											 itimeatt = "time",
											 ftimeatt = "time")
{
  type = match.arg(type)
  ID = casefold(ID)
  link = casefold(link)

  if (is(parent, "aRTlayer"))
  {
    if(is.null(tablename)) tablename = summary(parent)$layer
    if(type == "default") type = "static"
    if(type == "external") stop("Only aRTdb objects can create external tables")

	if( is.null(genID) ) # genID is useful only with static tables
	{
		genID = (type == "static")
	}

    if(create)	functionname = "cppLayerCreateTable"
	else 		functionname = "cppLayerOpenTable"

  }
  else if (is(parent, "aRTdb"))
  {
	if(is.null(tablename)) stop("tablename cannot be NULL if creating from database")
	if(type == "default") type = "external"
	if(type != "external") stop("aRTdb can only create external tables")
	
    if(create)	functionname = "cppDbCreateTable"
	else 		functionname = "cppDbOpenTable"
  }
  else
  {
    stop("Argument \"parent\" must be an object of aRTlayer or aRTdb\n")
  }
  .Object@pointer = .aRTcall(parent,
							 functionname,
    	                     tablename = tablename,
							 type = type,
							 generateids = genID,
							 ID = ID,
							 link = link,
							 itimeatt = itimeatt,
							 ftimeatt = ftimeatt,
							 length = as(length, "integer"))

  	reg.finalizer(.Object@pointer, .aRTremove)
	.Object
})

setMethod("createRelation","aRTtable",
           function(object,
		            attr,
					rtable,
					rattr = attr)
{
	summrtable = summary(rtable)
	summobject = summary(object)

	if(!is.object(rtable) || class(rtable) != "aRTtable")
		stop("Argument \"rTable\" must be an aRTtable")
	if(!any(summobject$columns == attr))
		stop(paste("Table", summary(table)$name, " does not have attribute \"", attr, "\"", sep = ""))
	if(!any(summrtable$columns == rattr))
		stop(paste("Table", summrtable$name, " does not have attribute \"", rattr, "\"", sep = ""))
	
	if(summobject$type == "external")
		stop("Object can't be an external table")
	
	if(summrtable$type != "external")
		stop("Relation table must be an external table")
	
	.aRTcall(object, "cppCreateRelation", attr=attr, rtable=summrtable$name, rattr=rattr)
})

setMethod("addRows", "aRTtable", function(object, data) {
	if(!is.data.frame(data)) stop("data must be a data.frame")

    ID=casefold(summary(object)$keys) # the keys of the table in the database

	position = 0

	colnames(data) = casefold(colnames(data))

	if (length(ID) > 0)
	{
		# search for the primary keys in the data.frame
		for(i in 1:length(ID))
		{
			if(!any(ID[i] == colnames(data)))
			{
				if(position != 0) stop("The table has more than one missing key.")
				position = i
			}
		}
	
		if(position != 0)
		{
		    cat("Using rownames as ID\n")
			l=list()
			l[[ID[position]]]=rownames(data)
			data=cbind(data, l)
		}
	}

	.aRTcall(object,
             "cppAddRows",
		     colnames= colnames(data),
		     length=length(rownames(data)),
		     matrix  = as.matrix(format.data.frame(data, trim=T)))
			   # format.data.frame to convert the elements to string
	return(invisible())
})

setMethod("createColumn", "aRTtable",
		  function(object,
                   colname,
				   type="c",
				   length=10,
				   key=FALSE,
				   decimals=3,
				   autonumber=FALSE) {
	colname = casefold(colname)

	.aRTcall(object,
             "cppCreateColumn",
		     colname=colname,
		     type=type,
			 length=as(length, "integer"),
			 decimals=as(decimals, "integer"),
			 autonumber=autonumber,
			 key=as(key, "integer"))
	return(invisible())
})

setMethod("updateColumns", "aRTtable",function(object, data){
	summ = summary(object)
	columns = casefold(summ$columns)
	keys = summ$keys
	table = summ$name

	# check if some attribute does not belong to the table, and insert the attribute
    colnames(data) = casefold(colnames(data))
	colnames = colnames(data)

	if(length(keys) == 1) # it only works with one key
	{
		poskey = which(colnames == keys)

		if(is.null(data[[keys]]))
		{
			stop(paste("Object ID column \"", keys, "\" was not found in the data.frame.", sep=""))
		}

		data[[keys]]=paste(data[[keys]])

		if(poskey != 1)
		{
			data <- data[, c(poskey, (1:ncol(data))[-poskey])]
		}
	}

    classes <- substr(sapply(data, class),1,1)
    for(i in 1:length(colnames))
    {
		if(any(columns == colnames[i]))
			.catSilent("Checking for column \'", colnames[i], "\' in table \'", table, "\' ... yes\n", sep="")
		else
		{
			createColumn(object, colnames[i], type = classes[i], length=max(nchar(as.matrix(data[i]))))
		}
    }

	.aRTcall(object,
             "cppUpdateColumns",
		     colnames= colnames(data),
		     length=length(rownames(data)),
		     matrix  = as.matrix(format.data.frame(data)))
			 # format.data.frame to convert the elements to string

	return (invisible())
})

# returns a terralib format string to representing dates
toDate=function(year = 0, month = 1, day = 1, hour = 0, minute = 0, second = 0)
{
	z = ""
	if     (year < 10)   z = "000"
	else if(year < 100)  z = "00"
	else if(year < 1000) z = "0"
	resp = paste(z, year, sep="")

	sep = c("-","-"," ",":",":")
	times = c(month, day, hour, minute, second)
	
	for(i in 1:length(times))
	{
		z = ""
		if(times[i] < 10) z = "0"
		resp = paste(resp, sep[i], z, times[i], sep="")
	}
	return(resp);
}

setMethod("getData", "aRTtable", function(object) {
	.Call("cppGetTableData", object@pointer, PACKAGE="aRT")
})

setMethod("createAndFillColumn", "aRTtable", function(object, newatt="", target,
           strategy=c("average", "stdev", "sum", "minimum", "maximum", "majority", "distance", "xdistance", "count", "presence", "area", "length", "averagewba", "sumwba", "percentage"), geometry=c("point","line","polygon","raster"), att="", proportion=FALSE, itime="", ftime="", maxerror=50, raster=NULL, validValues=NULL, dummy = NULL, default = NULL, band=NULL) {

	strategy = match.arg(strategy)
	geometry = match.arg(geometry)

	tname = ""
	if(is(target,"aRTlayer"))
	{
		if (summary(target)$raster != "yes") stop("The layer must have a raster data")
		israster = TRUE
	}
	else israster = FALSE

	if(israster)
	{
		func = "cppFillCellRaster"
		raster=getRaster(target, as.sp=FALSE)@pointer
		if (is.null(dummy))   stop("Dummy value is compulsory for raster-based strategies.")
		if (is.null(default)) stop("Default value is compulsory for raster-based strategies.")
	}
	else if((att != "") && (!any(strategy == c("averagewba", "sumwba"))))
	{
		func = "cppFillCellNonSpatial"
	}
    else
	{
		func = "cppFillCellGeom"
	}

	.Call(func, object@pointer, list(newatt=newatt,att=att,strategy=strategy,geometry=geometry,dist=maxerror,proportion=as.logical(proportion),tname=tname,
		  itime=itime, ftime=ftime, raster=raster, dummy=as.numeric(dummy), default=as.numeric(default),validValues=as.numeric(validValues), band=as.numeric(band)), target@pointer, PACKAGE="aRT")
	return(invisible())
})

