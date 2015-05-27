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

#ifndef ART_DLCOMMON_H_
#define ART_DLCOMMON_H_

#include "TeDecoderSEXP.h"
#include "aRTcomponent.h"

#include <TeDatabase.h>

extern "C" {
	
// common functions between database and layer
class aRTdlcommon 
{
	public:
		virtual aRTcomponent* OpenTable(SEXP options) { return NULL; }
		virtual ~aRTdlcommon() { }

		void CreateTable(string Tablename, SEXP attributes, int keyposition);
		void CreateLinkIds(string tablename, string idname);
		SEXP DeleteTable(SEXP tablename);
		
		virtual SEXP ShowTables() = 0;
	protected:
		TeDatabase* Database;
};

}

#endif

