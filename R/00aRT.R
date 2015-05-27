.packageName <- "aRT"
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

require(sp)

.mkGen = function(func, set)
{
	if(!isGeneric(func)) setGeneric(func, set)
	return(invisible())
}

.mkGen(".aRTcall", function(obj, call, ...)     standardGeneric(".aRTcall"))
#.mkGen("summary",  function(object, ...)        standardGeneric("summary"))
setGeneric("summary",  function(object, ...)        standardGeneric("summary"))
.mkGen("plot",     function(x, y, ...)          standardGeneric("plot"))

.mkGen("as.aRTgeometry", function(object) standardGeneric("as.aRTgeometry"))

# openDb has "..." because of the TerraView integration (.external)
.mkGen("openDb",              function(object, ...) standardGeneric("openDb"))
.mkGen("openLayer",           function(object, ...) standardGeneric("openLayer"))
.mkGen("openTheme",           function(object, ...) standardGeneric("openTheme"))
.mkGen("openQuerier",         function(object, ...) standardGeneric("openQuerier"))
.mkGen("openTable",           function(object, ...) standardGeneric("openTable"))
.mkGen("createGPM",           function(object, ...) standardGeneric("createGPM"))
.mkGen("createView",          function(object, ...) standardGeneric("createView"))
.mkGen("createDb",            function(object, ...) standardGeneric("createDb"))
.mkGen("createLayer",         function(object, ...) standardGeneric("createLayer"))
.mkGen("createTheme",         function(object, ...) standardGeneric("createTheme"))
.mkGen("createTable",         function(object, ...) standardGeneric("createTable"))
.mkGen("createAndFillColumn", function(object, ...) standardGeneric("createAndFillColumn"))
.mkGen("reload",              function(object, ...) standardGeneric("reload"))

.mkGen("createRelation", function(object, attr, rtable, rattr = attr)
	standardGeneric("createRelation"))

.mkGen("createColumn", function(object, 
                                colname,
								type="c",
								length=10,
								key=FALSE,
								decimals=3,
								autonumber=FALSE) standardGeneric("createColumn"))

.mkGen("showDbs",     function(object) standardGeneric("showDbs"))
.mkGen("showLayers",  function(object) standardGeneric("showLayers"))
.mkGen("showTables",  function(object) standardGeneric("showTables"))
.mkGen("getID",       function(object) standardGeneric("getID"))
.mkGen("showViews",   function(object) standardGeneric("showViews"))
.mkGen("showThemes",  function(object) standardGeneric("showThemes"))

.mkGen("importTable", function(object, table, ID, data, ...)   standardGeneric("importTable"))
.mkGen("importShape", function(object, layer, proj = NA, ...)  standardGeneric("importShape"))
.mkGen("importMif",   function(object, file, lname, ...)       standardGeneric("importMif"))
.mkGen("importRaster",function(object, file, lname, ...)       standardGeneric("importRaster"))
.mkGen("importSpData",function(object, spdata, lname, ...)     standardGeneric("importSpData"))

.mkGen("deleteDb",    function(object, dbname,    force=FALSE) standardGeneric("deleteDb"))
.mkGen("deleteTheme", function(object, themename, force=FALSE) standardGeneric("deleteTheme"))
.mkGen("deleteLayer", function(object, layername, force=FALSE) standardGeneric("deleteLayer"))
.mkGen("deleteView",  function(object, viewname,  force=FALSE) standardGeneric("deleteView"))
.mkGen("deleteTable", function(object, tablename, force=FALSE) standardGeneric("deleteTable"))

.mkGen("addPermission",  function(object, ...)        standardGeneric("addPermission"))
.mkGen("dropUser",       function(object, ...)        standardGeneric("dropUser"))
.mkGen("getPermissions", function(object, ...)        standardGeneric("getPermissions"))
.mkGen("addPoints",      function(object, points)     standardGeneric("addPoints"))
.mkGen("addPolygons",    function(object, polygons)   standardGeneric("addPolygons"))
.mkGen("addLines",       function(object, lines, ...) standardGeneric("addLines"))
.mkGen("addRaster",      function(object, raster)     standardGeneric("addRaster"))
.mkGen("addRows",        function(object, data)       standardGeneric("addRows"))
.mkGen("addShape",       function(object, file, table, ID="", length=10) standardGeneric("addShape"))

.mkGen("getBox",      function(object) standardGeneric("getBox"))
.mkGen("getGeometry", function(object, calcal=TRUE) standardGeneric("getGeometry"))
.mkGen("getProj",     function(object) standardGeneric("getProj"))
.mkGen("getVisual",   function(object) standardGeneric("getVisual"))
.mkGen("getPoints",   function(object, ...) standardGeneric("getPoints"))
.mkGen("getLines",    function(object, ...) standardGeneric("getLines"))
.mkGen("getPolygons", function(object, ...) standardGeneric("getPolygons"))
.mkGen("getCells",    function(object, ...) standardGeneric("getCells"))
.mkGen("getRaster",   function(object, ...) standardGeneric("getRaster"))
.mkGen("getData",     function(object, ...) standardGeneric("getData"))
.mkGen("getPixels",   function(object, ...) standardGeneric("getPixels"))

.mkGen("getDistance",     function(object, ID, layer=NULL) standardGeneric("getDistance"))
.mkGen("getNN",           function(object, layer=object, ID, quantity=1) standardGeneric("getNN"))

.mkGen("getMetric",       function(object, ...)   standardGeneric("getMetric"))

.mkGen("getSetOperation", function(object, operation, ID=NULL, as.sp=TRUE)
	standardGeneric("getSetOperation"))

.mkGen("getOperation",    function(object, operation, ID=NULL, distance=NULL, as.sp=TRUE)
	standardGeneric("getOperation"))

.mkGen("getClip",         function(object, geometry, as.sp=TRUE) standardGeneric("getClip"))

.mkGen("getRelation",     function(object, relation, layer=object, ID=NULL)
	standardGeneric("getRelation"))

.mkGen("nextFrame",    function(object)       standardGeneric("nextFrame"))
.mkGen("updateColumns",function(object, data) standardGeneric("updateColumns"))

.mkGen("setVisible", function(object, points, lines, polygons, check)
	standardGeneric("setVisible"))

.mkGen("setVisual",  function(object, visual, attribute = NULL, mode = "e")
	standardGeneric("setVisual"))

.mkGen("simplify",    function(object, snap, maxdist) standardGeneric("simplify"))
.mkGen("removeDup",   function(object)                standardGeneric("removeDup"))

