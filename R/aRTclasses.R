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

setClass("aRTcomponent", representation(pointer = "externalptr", "VIRTUAL"))

setClass("aRTconn",     contains = "aRTcomponent")
setClass("aRTdb",       contains = "aRTcomponent")
setClass("aRTlayer",    contains = "aRTcomponent")
setClass("aRTgeometry", contains = "aRTcomponent")
setClass("aRTraster",   contains = "aRTcomponent")
setClass("aRTtheme",    contains = "aRTcomponent")
setClass("aRTtable",    contains = "aRTcomponent")
setClass("aRTquerier",  contains = "aRTcomponent")
setClass("aRTnetwork",  contains = "aRTcomponent")
setClass("aRTgpm",      contains = "aRTcomponent")

