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

// SEXPutils implements functions that manipulate TerraLib data

#ifndef SEXP_UTILS_H_
#define SEXP_UTILS_H_

#include "SEXPbasic.h"

#include <TeVisual.h>
#include <TeBox.h>
#include <TeDatabase.h>

extern "C" {

namespace SEXPutils
{
	using namespace std;

	vector<TeColor> SEXPtoColors(SEXP colors);
	SEXP GetBoxTeBox(TeBox box);
	TeVisual* GetTeVisual(SEXP sxp, TeGeomRep geomrep);

	TeKeys SEXPtoKeys(SEXP data); // a vector of string
	SEXP GetKeys(TeKeys keys);
}

}

#endif
