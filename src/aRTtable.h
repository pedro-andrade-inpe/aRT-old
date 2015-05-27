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

#ifndef ART_TABLE_H_
#define ART_TABLE_H_

#include "aRTcomponent.h"
#include "aRTgeometry.h"

#include "TeUtils.h"

#include <TeTable.h>

class aRTlayer;

#include "aRTlayer.h"


extern "C" {

class aRTtable : public aRTcomponentLeaf
{
	public:
		aRTtable(TeDatabase* db, string layer, string tablename);
		~aRTtable();

		void CreateLinkIds(string idname);

		virtual SEXP CreateRelation(SEXP data);
		virtual SEXP CreateColumn  (SEXP data);
		virtual SEXP AddRows       (SEXP data);
		virtual SEXP UpdateColumns (SEXP data);
	
		virtual void Create(SEXP data) = 0; // create the table in the database
		
		virtual SEXP GetData();
		
		aRTcomponent* Open(SEXP) { return NULL; }
		virtual SEXP Summary();
		virtual SEXP Print(); 
		SEXP List() { return R_NilValue; }
		string Class() { return "aRTtable"; }
		bool IsValid();
		string Name() { return TableName; }
		TeLayer* Layer() { return TeUtils::GetLayerFromDatabase(Database, LayerName); }

		TeGeomRep StringToGeomRep(string geom);

		SEXP CreateAndFillNonSpatial(SEXP data, aRTtheme* other);
		SEXP CreateAndFillFromRaster(SEXP data, aRTlayer* other);
		SEXP CreateAndFillFromGeom  (SEXP data, aRTtheme* other);
	protected:
		// FIXME: gambiarra para fazer funcionar com versao 3.2.1 no mysql, pois da erro nesta funcao
		bool MYupdateTable(TeTable &table);

		string TableName;
		string LayerName;
		TeDatabase* Database;
};

}

#endif

