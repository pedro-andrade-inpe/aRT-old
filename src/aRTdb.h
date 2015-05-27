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

#ifndef ART_DB_H_
#define ART_DB_H_

#include "aRTcomponent.h"
#include "aRTdlcommon.h"
#include "aRTlayer.h"

extern "C"
{

class aRTdb : public aRTcomponentRoot, public aRTdlcommon
{
	public:
		aRTdb(TeDatabase* database, bool load = true);
		~aRTdb();

		aRTcomponent* OpenLayer   (SEXP options);
		aRTcomponent* OpenTable   (SEXP options);
		aRTcomponent* OpenTheme   (SEXP options);
		aRTcomponent* CreateTable (SEXP options);
		aRTcomponent* CreateLayer (SEXP options);
		aRTcomponent* ImportMif   (SEXP options);
		aRTcomponent* ImportRaster(SEXP options);

		SEXP Summary();
		SEXP List();
		SEXP Print();
		SEXP ShowViews();
		SEXP ShowThemes();
		SEXP ShowTables();
		SEXP Reload(SEXP data);

		SEXP CreateView (SEXP data, TeProjection* proj = NULL) { return CreateView(SEXP_TO_STRING(data), proj); }
		SEXP DeleteLayer(SEXP layer_name);
		SEXP DeleteView (SEXP view_name);
		SEXP DeleteTheme(SEXP theme_name);

		string Class() { return "aRTdb"; }
		SEXP CreateView(string str, TeProjection* proj = NULL);
	private:
		string views;
};

}

#endif

