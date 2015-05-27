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

setMethod(".aRTcall", "aRTcomponent", function(obj, call, ...)
{
	.Call(call, obj@pointer, list(...), PACKAGE = "aRT")
})

setMethod("show", "aRTcomponent", function(object) {
	   .aRTcall(object, "cppPrint")
	   return (invisible())
})


setMethod("summary", "aRTcomponent", function(object, ...) {
  .aRTcall(object, "cppSummary")
})


