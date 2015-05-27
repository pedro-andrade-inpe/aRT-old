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

// this class does not have connection with the other aRT classes,
// because it works with objects directly in memory

#ifndef ART_RASTER_H_
#define ART_RASTER_H_

#include "aRTcomponent.h"
#include "SEXPutils.h"
#include "TeDecoderSEXP.h"
#include "aRTgeometry.h"

#include <TeLayer.h>

extern "C" {

class aRTraster : public aRTcomponent
{
	public:
		aRTraster(SEXP spdata);
		aRTraster(TeLayer* layer);

		SEXP GetData();

		SEXP Summary();
		SEXP Print(); 
		string Class()          { return "aRTraster"; }
		bool IsValid()          { return true;        }
		SEXP List()             { return RNULL;       }
		aRTcomponent* Open(SEXP){ return NULL;        }
		TeRaster* GetTeRaster() { return Raster;      }

		// operations
		SEXP GetPixels(SEXP data, aRTgeometry* geom); 
	private:
		TeRaster* Raster;
};

}

#endif

