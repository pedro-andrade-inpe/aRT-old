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

#ifndef ART_LAYER_H_
#define ART_LAYER_H_

#include "aRTcomponent.h"
#include "aRTtheme.h"
#include "aRTdb.h"
#include "aRTquerier.h"
#include "aRTtable.h"
#include "aRTdlcommon.h"

#include "SEXPutils.h"
#include "TeDecoderSEXP.h"

#include <TeLayer.h>

#include <sstream>

extern "C" {

using namespace SEXPutils;
	
// If we do this: 
// class aRTlayer : public aRTdlcommon, public aRTcomponentLeaf
// the destructor is called when a function from aRTdlcommon is called!!!
// The same occours if we convert a pointer to aRTdlcommon in main.cpp.
class aRTlayer : public aRTcomponentLeaf, public aRTdlcommon
{
	public:
		aRTlayer(TeDatabase* database, string layername);
		aRTlayer(TeLayer*);
		~aRTlayer();

		aRTcomponent* CreateTheme      (SEXP data);
		aRTcomponent* CreateTable      (SEXP data);
		aRTcomponent* OpenTheme        (SEXP data);
		aRTcomponent* OpenQuerier      (SEXP data);
		aRTcomponent* OpenTable        (SEXP data);
		aRTcomponent* GetOperation     (SEXP options);
		aRTcomponent* GetSetOperation  (SEXP options);
		aRTcomponent* GetRaster        (SEXP options);
		aRTcomponent* CreateLayerCells (SEXP options);

		SEXP AddPoints  (SEXP points);
		SEXP AddPolygons(SEXP polset);
		SEXP AddLines   (SEXP lineset);
		SEXP AddShape   (SEXP shapeinfo);
		SEXP AddRaster  (SEXP raster);

		SEXP GetBox();
		SEXP GetID();
		SEXP GetProj();
		SEXP GetMetric(SEXP options);

		SEXP GetRelation         (SEXP options, aRTlayer* layer);
		SEXP GetDistance         (SEXP options, aRTlayer* layer);
		SEXP GetNearestNeighbors (SEXP options, aRTlayer* layer);

		TeGeomRep GeomRep();
		
		SEXP Summary();
		SEXP Print(); 
		SEXP List() { return R_NilValue; }
		SEXP ShowTables();
		string Class() { return "aRTlayer"; }
		bool IsValid();
		TeLayer* Layer() { return TeUtils::GetLayerFromDatabase(Database, LayerName); }
		string LayerName;
	private:
};

}

#endif

