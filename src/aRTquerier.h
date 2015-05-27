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

#ifndef ART_QUERIER_H_
#define ART_QUERIER_H_

#include "aRTcomponent.h"
#include "aRTlayer.h"
#include "SEXPutils.h"

#include <TeQuerier.h>

extern "C" {

class aRTquerier : public aRTcomponentLeaf
{
	public:
		aRTquerier(TeQuerierParams& params, TeGeomRep);
		~aRTquerier();

		virtual SEXP GetData(SEXP options);
		SEXP NextFrame();
		
		aRTcomponent* Open(SEXP) { return NULL; }
		SEXP Summary();
		SEXP Print(); 
		SEXP List() { return R_NilValue; }
		string Class() { return "aRTquerier"; }
		bool IsValid();
	private:
		int ActualFrame;
		int ActualElem;
		TeQuerier* Querier;
		TeGeomRep GeomRep; // only one type of geometry is used in the querier.
};

}

#endif

