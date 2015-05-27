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

#ifndef ART_GEOMETRY_H_
#define ART_GEOMETRY_H_

#include "SEXPutils.h"
#include "aRTcomponent.h"

#include <TeMultiGeometry.h>

extern "C" {

// stores a TerraLib _unique_ geometry: points, lines _or_ polygons
// 
// aRTgeometry specification:
// We use only one TeMultiGeometry, instead of a vector. Objects with more
// than one geometry are stored in consecultive positions. So be careful! 
// 
// If we compare the data structures we have
//
// sp                      TerraLib
// ~~~~~~~~~~~~~~~~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Polygon/Line            TeLine2D
// Polygons/Lines          vector<(TePolygon/TeLine)> => Te(Polygon/Line)Set
// Spatial(Polygons/Lines) vector<(TePolygon/TeLine)> => Te(Polygon/Line)Set
// SpatialPointsDataFrame  TePointSet (using only ID as attribute)
//
// TePolygon stores only one polygon with a hole
	
static SEXP PolygonDef;
static SEXP PolygonsDef;
static SEXP LineDef;
static SEXP LinesDef;

class aRTgeometry : public aRTcomponent
{
	public:
		aRTgeometry(SEXP spdata);
		aRTgeometry(TeMultiGeometry);

		SEXP GetData();

		SEXP GetPoints  () { return GetPointsTePointSet    (MultiGeom.points_);   }
		SEXP GetLines   () { return GetLinesTeLineSet      (MultiGeom.lines_);    }
		SEXP GetPolygons() { return GetPolygonsTePolygonSet(MultiGeom.polygons_); }
		SEXP GetCells   () { return GetPolygonsTeCellSet   (MultiGeom.cells_);    }
		aRTcomponent* GetClip    (aRTgeometry* refpols);

		bool AddGeometry(TeMultiGeometry& geom); 
		
		TePointSet&   GetTePointSet  () { return MultiGeom.points_;   }
		TePolygonSet& GetTePolygonSet() { return MultiGeom.polygons_; }
		TeLineSet&    GetTeLineSet   () { return MultiGeom.lines_;    }
		static void InitSpDefs(SEXP defs); // this function must be called before any conversion
		
		SEXP Summary();
		SEXP Print(); 
		string Class()          { return "aRTgeometry"; }
		bool IsValid()          { return true;          }
		SEXP List()             { return RNULL;         }
		aRTcomponent* Open(SEXP){ return NULL;          }

		// TeGeometryAlgorithms.h
		SEXP Simplify(SEXP thresholds);
		SEXP RemoveDuplicated();
		SEXP GetPixels(SEXP data);

	private:
		TeMultiGeometry MultiGeom;
		TeRaster* Raster;
		
		TePointSet   GetTePointSet  (SEXP sps);
		TePolygonSet GetTePolygonSet(SEXP sps);
		TeLineSet    GetTeLineSet   (SEXP sps);
		
		SEXP GetPointsTePointSet    (TePointSet& ps);
		SEXP GetPolygonsTePolygonSet(TePolygonSet& polset);
		SEXP GetPolygonsTeCellSet   (TeCellSet& cellset);
		SEXP GetLinesTeLineSet      (TeLineSet& lineset);
		
		SEXP GetPolygonsTePolygonSetWithoutIslands(TePolygonSet& polset);
		SEXP GetPolygonsTePolygonSetWithIslands   (TePolygonSet& polset, unsigned quant);
};

}

#endif

