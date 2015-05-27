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

#include "aRTstatictable.h"

extern "C" {

void aRTstatictable::Create(SEXP options)
{
	string id   = GET_STRING_ELEMENT  (options, aRTstrId);
	int length  = GET_INTEGER_ELEMENT (options, "length");
	bool genids = GET_BOOL_ELEMENT    (options, "generateids");

	TeAttributeList attList;
	TeAttribute at;
			
    at.rep_.type_         = TeSTRING;
    at.rep_.numChar_      = length;
    at.rep_.name_         = id;
    at.rep_.isPrimaryKey_ = true;

    attList.push_back(at);

	TeTable attTable(TableName, attList, id, id, TeAttrStatic);

	PrintSilent("Creating static table \'%s\' on layer \'%s\' ... ", TableName.c_str(), LayerName.c_str());
	if( !Layer()->createAttributeTable(attTable) )
	{
		stringstream err;
		err << "Could not create the table:"
			<< Database->errorMessage() << endl;
		error( StreamToChar(err) );
	}
	PrintSilentYes;

	if(genids) CreateLinkIds(id);
}

}

