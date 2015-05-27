/**************************************************************************\
 *  aRT: R-TerraLib API                                                   *
 *  Copyright (C) 2003-2011  LEG                                          *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU Lesser General Public      *
 *  License along with this library.                                      *
\**************************************************************************/

#include <stdio.h>

#include "aRTcomponent.h"
#include "aRTdlcommon.h"

extern "C" {
	
void aRTdlcommon::CreateLinkIds(string tablename, string idname)
{
}

SEXP aRTdlcommon::DeleteTable(SEXP data)
{
	string table_name = GET_STRING_ELEMENT(data, "tablename");

	if ( Database->tableExist(table_name) )
	{
		if( Database->deleteTable(table_name) )
			PrintSilent("Table removed successfully\n");
		else
			error("Table can not be removed\n");
	}
	else
		error("Table does not exist\n");

	return RNULL;
}

}

