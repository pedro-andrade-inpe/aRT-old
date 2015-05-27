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

#ifndef ART_THEME_H_
#define ART_THEME_H_

#include "aRTcomponent.h"
#include "TeUtils.h"

#include <TeTheme.h>

extern "C" {

// aRTthemes can't be populated
// but we can select data and build temporal series
class aRTtheme: public aRTcomponentLeaf
{
	public:
		aRTtheme(TeDatabase* database, string name);
		~aRTtheme();

		SEXP SetVisible(SEXP visible);
		
		SEXP GetBox();
		SEXP GetRaster();
		SEXP GetVisual();

		SEXP SetVisual(SEXP visual);

		aRTcomponent* OpenQuerier(SEXP data);

		SEXP List() { return R_NilValue; }
		SEXP Summary();
		SEXP Print();
		TeGeomRep GeomRep();

		string Class() { return "aRTtheme"; }
		TeTheme* Theme() { return TeUtils::GetThemeFromDatabase(Database, ThemeName); }
	private:
		TeDatabase* Database;
		string ThemeName;
};

}

#endif

