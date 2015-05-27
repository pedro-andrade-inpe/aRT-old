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

#include "aRTmediatable.h"

#include <TeDatabaseUtils.h>
#include "TeUtils.h"
#include <TeApplicationUtils.h>


extern "C"{

aRTmediatable::aRTmediatable(TeDatabase* db, string layer, string tablename)
		:aRTtable(db,layer,tablename)
{
    TableName = "media_layer_" + Te2String(Layer()->id());
}
	
void aRTmediatable::Create(SEXP data)
{
    string mTable = "media_table_" + Te2String(Layer()->id());

	PrintSilent("Creating media table to layer \'%s\' ... ", LayerName.c_str());
    if( !createLayerMediaTable(Database, Layer()->id(), TableName) )
        error("Fail to create the table to store media names!");

    if( !createMediaTable(Database, mTable) )
        error("Fail to create the media tables!");
	PrintSilentYes;
}

SEXP aRTmediatable::AddRows(SEXP data)
{
    SEXP colnames = GetListElement     (data, "colnames");
    SEXP matrix   = GetListElement     (data, "matrix");
    int lrows     = GET_INTEGER_ELEMENT(data, "length");

    int lcols = Rf_length(colnames);
	int pos_id;
	int pos_url;
	
	if(lcols != 2) error("Invalid number of columns\n");

	if( string(CHAR(STRING_ELT(colnames,0))) == "object_id")
	{
		pos_id = 0;
		pos_url = 1;
	}
	else
	{
		pos_id = 1;
		pos_url = 0;
	}
	
	PrintSilent("Adding %d urls ... ", lrows);
	for(int i = 0; i < lrows; i++)
	{
		AddUrl(CHAR(STRING_ELT(matrix, i + pos_id  * lrows)),
               CHAR(STRING_ELT(matrix, i + pos_url * lrows)));
	}
	PrintSilentYes;
	return RNULL;
}
		
void aRTmediatable::AddUrl(string id_, string site)
{
    string type = "url";

	TeLayer* layer_ = Layer();
    string mTable = "media_table_" + Te2String(layer_->id());

    TeDatabasePortal *portal = Database->getPortal();
    if( !insertMedia(Database, mTable, site, "", type, false) )
    {
		delete portal;
        error("Fail to insert the \"media_blob\" field in the table!");
    }
    if( !insertMediaObjLayer(Database, Layer(), id_, site, mTable) )
    {
		delete portal;
        error("Fail to insert the \"media_name\" field in the table!");
    }

	// the last url will be the default one
    string up = "UPDATE " + TableName + " SET show_default = 1 WHERE object_id = '" + id_ + "'";
    Database->execute(up);
    delete portal;
}

SEXP aRTmediatable::GetData()
{
    TeTable table;
	table.name(TableName);
	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);

	SEXP result;
	SEXP *each_column;
	SEXP colnames;
	SEXP rownames;
	
	TeAttributeList attlist = table.attributeList();
	
	each_column = new SEXP[2];
	colnames    = PROTECT(allocVector( STRSXP, 2 ));
	result      = PROTECT(allocVector( VECSXP, 2 ));
	rownames    = PROTECT(allocVector( STRSXP, table.size() ));

	int pos_id  = 0;
	int pos_url = 0;
	
	for(unsigned i = 0; i != attlist.size(); i++)
	{
		if(attlist[i].rep_.name_ == "object_id")  pos_id  = i;
		if(attlist[i].rep_.name_ == "media_name") pos_url = i;
	}

	each_column[0] = PROTECT(allocVector( STRSXP, table.size() ));
	each_column[1] = PROTECT(allocVector( STRSXP, table.size() ));

	SET_STRING_ELT(colnames, 0, mkChar("object_id"));
	SET_STRING_ELT(colnames, 1, mkChar("media_name"));
	
	for(unsigned i = 0; i < table.size(); i++)
	{
		SET_STRING_ELT( each_column[0], i, mkChar(table(i,pos_id).c_str()) );
		SET_STRING_ELT( each_column[1], i, mkChar(table(i,pos_url).c_str()) );
		
		stringstream str;
		str << i+1; // rownames must start from 1
		SET_STRING_ELT(rownames, i, mkChar(str.str().c_str()));
	}

	setAttrib(result, R_NamesSymbol, colnames);
	
	SET_VECTOR_ELT(result, 0, each_column[0]);
	SET_VECTOR_ELT(result, 1, each_column[1]);

	result = AsDataFrame(result, rownames);
	UNPROTECT(5);
	delete[] each_column;

	return result;
}

// this is the order to appear inside R. S is from Summary
enum aRTtableSenum
{
	aRTtableSClass = 0,
	aRTtableSType,
	aRTtableSRows,
	aRTtableSColumns,
	aRTtableSKeys,
	aRTtableSSize
};

// if you want to know what is really stored in a media table
// just comment this function and using the original from aRTtable
SEXP aRTmediatable::Summary()
{
    SEXP info, names, skeys, scolumns;

	TeTable table;
	vector<string> stratts;

	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);
	table.primaryKeys(stratts);
	TeAttributeList attlist = table.attributeList();

	info     = allocVector(VECSXP, aRTtableSSize);  // List containing the connection attributes
    names    = allocVector(STRSXP, aRTtableSSize);  // Names Vector
	skeys    = allocVector(STRSXP, 2); // keys vector
	scolumns = allocVector(STRSXP, 2);

	SET_STRING_ELT(scolumns, 0, mkChar("object_id"));
	SET_STRING_ELT(scolumns, 1, mkChar("media_name"));
	
	SET_STRING_ELT(skeys, 0, mkChar("object_id"));
	SET_STRING_ELT(skeys, 1, mkChar("media_name"));
	
	STRING_LIST_ITEM (info, names, aRTtableSClass,   "class",   Class());
	STRING_LIST_ITEM (info, names, aRTtableSType,    "type",    TeUtils::AttrTableType(table.tableType()));
	INTEGER_LIST_ITEM(info, names, aRTtableSRows,    "rows",    table.size());
	SEXP_LIST_ITEM   (info, names, aRTtableSKeys,    "keys",    skeys);
	SEXP_LIST_ITEM   (info, names, aRTtableSColumns, "columns", scolumns);
	
    setAttrib(info, R_NamesSymbol, names); // Set the names of the list
	
	return info;
}

SEXP aRTmediatable::Print()
{
	stringstream s;

	if (!IsValid()) error("Invalid object!");

    TeTable table;
	table.name(TableName);
	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);
	s << artOBJECT("aRTtable")  << "\n\n"
	  << "Type: "  << TeUtils::AttrTableType(table.tableType()) << endl
	  << "Layer: " << LayerName                                 << endl
      << "Rows: "  << table.size()                              << endl
	  << "Attributes: "                                         << endl
      << "    object_id: character[50] (key)"                   << endl
      << "    media_name: character[255] (key)"                 << endl;						   

	Rprintf( StreamToChar(s) );
	return RNULL;
}

}

