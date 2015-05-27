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

#include "aRTeventtable.h"

extern "C" {

void aRTeventtable::Create(SEXP options)
{
	string id       = GET_STRING_ELEMENT  (options, aRTstrId);
	int length      = GET_INTEGER_ELEMENT (options, "length");
	bool genids     = GET_BOOL_ELEMENT    (options, "generateids");
	string itimeatt = GET_STRING_ELEMENT  (options, "itimeatt");
	string ftimeatt = GET_STRING_ELEMENT  (options, "ftimeatt");

	TeAttributeList attList;
	TeAttribute at;
			
    at.rep_.type_         = TeSTRING;
    at.rep_.numChar_      = length;
    at.rep_.name_         = id;
    at.rep_.isPrimaryKey_ = true;

    attList.push_back(at);

	at.rep_.type_ = TeDATETIME;
	at.rep_.name_ = itimeatt;

	/* // these changes are not saved in the database. it must be done when we will insert data.
	at.dateTimeFormat_ = "YYYYsMMsDDsHHsmmsSS";
	at.dateChronon_ = TeSECOND;
	at.dateSeparator_ = "-";
	at.timeSeparator_ = ":";
	*/

	attList.push_back(at);

	if(itimeatt != ftimeatt)
	{
		at.rep_.name_ = ftimeatt;
		attList.push_back(at);
	}
	
	PrintSilent("Creating event table \'%s\' on layer \'%s\' ... ", TableName.c_str(), LayerName.c_str());
	TeTable attTable(TableName, attList, id, id, TeAttrEvent);

	attTable.attInitialTime(itimeatt);
    attTable.attFinalTime(ftimeatt);
							
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

