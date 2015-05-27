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

#include "aRTdynattrtable.h"

extern "C" {

void aRTdynattrtable::Create(SEXP options)
{
	string id       = GET_STRING_ELEMENT  (options, aRTstrId);
	string link     = GET_STRING_ELEMENT  (options, "link");
	int length      = GET_INTEGER_ELEMENT (options, "length");
	bool genids     = GET_BOOL_ELEMENT    (options, "generateids");
	string itimeatt = GET_STRING_ELEMENT  (options, "itimeatt");
	string ftimeatt = GET_STRING_ELEMENT  (options, "ftimeatt");

	TeAttributeList attList;
	TeAttributeList keyList;
	TeAttribute at;

//	at.rep_.name_ = "attr_id";
//	at.rep_.type_ = TeSTRING;
//	at.rep_.numChar_ = 48;
//	attList.push_back (at);
//	keyList.push_back(at);

    at.rep_.type_         = TeSTRING;
    at.rep_.numChar_      = length;
    at.rep_.name_         = id;
    at.rep_.isPrimaryKey_ = true;

    attList.push_back(at);
	keyList.push_back(at);

    at.rep_.type_         = TeSTRING;
    at.rep_.numChar_      = length;
    at.rep_.name_         = link;
    at.rep_.isPrimaryKey_ = false;

    attList.push_back(at);

	at.rep_.type_ = TeDATETIME;
	at.rep_.name_ = itimeatt;
    at.rep_.isPrimaryKey_ = false;

 // these changes are not saved in the database. it must be done when we will insert data.
	at.dateTimeFormat_ = "YYYYsMMsDDsHHsmmsSS";
	at.dateChronon_ = TeSECOND;
	at.dateSeparator_ = "-";
	at.timeSeparator_ = ":";

	attList.push_back(at);

	if(itimeatt != ftimeatt)
	{
		at.rep_.name_ = ftimeatt;
		attList.push_back(at);
	}
	
	TeTable attTable(TableName, attList, id, link, TeFixedGeomDynAttr);

	attTable.setLinkName(link);
	attTable.setUniqueName(id);
	attTable.attInitialTime(itimeatt);
    attTable.attFinalTime(ftimeatt);

	PrintSilent("Creating dynamic table \'%s\' on layer \'%s\' ... ", TableName.c_str(), LayerName.c_str());
	if( !Layer()->createAttributeTable(attTable) )
	{
		stringstream err;
		err << "Could not create the table:"
			<< Database->errorMessage() << endl;
		error( StreamToChar(err) );
	}

	PrintSilentYes;


// TODO: TEMPORAL TABLES AS WRITTEN IN ANA PAULA'S CODE

/*
       TeAttribute        attribute;
       TeAttributeList attList;
       TeAttributeList keyList;

       // attr_id
       attribute.rep_.name_ = "attr_id";
       attribute.rep_.type_ = TeSTRING;
       attribute.rep_.numChar_ = 48;
       attribute.rep_.isPrimaryKey_ = true;
       attList.push_back (attribute);
       keyList.push_back(attribute);

       // object_id
       attribute.rep_.name_ = "object_id_";
       attribute.rep_.type_ = TeSTRING;
       attribute.rep_.numChar_ = 48;
       attList.push_back ( attribute );
         //  initial_time
       attribute.rep_.name_ = "initial_time";
        attribute.rep_.type_ = TeDATETIME;         attribute.dateTimeFormat_ =
"YYYYsMMsDDsHHsmmsSS";  // se alguma coisa der errado, é por aqui... checar com karine
       attribute.dateChronon_ = TeSECOND;
       attribute.rep_.numChar_ = 48;
       attList.push_back ( attribute );

       //  final_time
       attribute.rep_.name_ = "final_time";
        attribute.rep_.type_ = TeDATETIME;        attribute.dateTimeFormat_ =
"YYYYsMMsDDsHHsmmsSS";
       attribute.dateChronon_ = TeSECOND;
       attribute.rep_.numChar_ = 48;
       attList.push_back ( attribute );
         // Create table and initialize attributes

       TeTable cells_attTable (TableName);             
	cells_attTable.setAttributeList(attList);
       cells_attTable.setTableType(TeFixedGeomDynAttr);
       cells_attTable.setLinkName("object_id_");
       cells_attTable.setUniqueName("attr_id");
       cells_attTable.attInitialTime ("initial_time");
       cells_attTable.attFinalTime ("final_time");
       cells_attTable.attTimeUnit (TeSECOND);
                 if (!Layer()->createAttributeTable(cells_attTable))
           return;

       TeAttributeList attr;
       Layer()->database ()->getAttributeList (TableName, attr);
*/

	if(genids) CreateLinkIds(id);
}

}

