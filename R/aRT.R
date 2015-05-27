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

aRTversion <- function()
{
	ver <- packageDescription("aRT", fields="Version")
	dat <- packageDescription("aRT", fields="Date")
	result=list()
	result$aRT=paste(ver, dat)
	result$TerraLib=.Call("cppTerraLibVersion",PACKAGE="aRT")
	result
} 

# TEST FUNCTION
.pp <-function(value)
{
    .Call("cppPP", value, PACKAGE="aRT")
}

.aRTconfirm = function(force)
{
    c = "Y"
    if(!force)  c = .Call("cppConfirm", PACKAGE="aRT")

    return (c == "Y")
}

# set the silent mode
aRTsilent <- function(value = NULL)
{
    .Call("cppSilent", value, PACKAGE="aRT")
}

.catSilent = function(...)
{
	if( !aRTsilent() ) cat(...)
	return( invisible() )
}

# "destructor" function
.aRTremove <- function(pointer)
{
	.Call("cppRemove", pointer, PACKAGE="aRT")
	return(invisible())
}

#.First.lib <- function(lib, pkg) {
#    library.dynam("aRT", pkg, lib)
#}

