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

#include "aRTexternaltable.h"

extern "C" {

void aRTexternaltable::Create(SEXP options)
{
	string id  = GET_STRING_ELEMENT  (options, aRTstrId);
	int length = GET_INTEGER_ELEMENT (options, "length");

	TeAttributeList attList;
	TeAttribute at;
			
    at.rep_.type_         = TeSTRING;
    at.rep_.numChar_      = length;
    at.rep_.name_         = id;
    at.rep_.isPrimaryKey_ = true;

    attList.push_back(at);
	//TeAttrTableType atttype; // apaguei aqui 2011/04/22
	string link = "";
	//atttype = TeAttrExternal;

	if( !Database->createTable(TableName, attList))
		{
			stringstream err;
			err << "Could not create the table:"
				<< Database->errorMessage() << endl;
			delete this;
			error( StreamToChar(err) );
		}
		
	TeTable attTable(TableName, attList, id, "", TeAttrExternal);

	if( !Database->insertTableInfo(-1, attTable))
	{
		stringstream err;
		err << "Could not make the table external:"
			<< Database->errorMessage() << endl;
		delete this;
		error( StreamToChar(err) );
	}
/*
	TeTable attTable(TableName, attList, id, link, atttype);

	if( !Layer()->createAttributeTable(attTable) )
	{
		stringstream err;
		err << "Could not create the table:"
			<< Database->errorMessage() << endl;
		error( StreamToChar(err) );
	}*/
}

}

