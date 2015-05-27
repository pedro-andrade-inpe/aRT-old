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

#include "aRTgeometry.h"

using namespace std;
using namespace SEXPutils;

#include <sstream>
#include <TeOverlay.h>
#include <TeSpatialOperations.h>

extern "C" {

// TODO: melhorar este construtor, talvez usando um
// ponteiro para nao usar o construtor de copia
// Como nao esta lento, isto fica para o furuto.
// Sera que o compilador esta fazendo esta otimizacao?

aRTgeometry::aRTgeometry(TeMultiGeometry mg) : MultiGeom(mg)
{
	Raster = NULL;
}

aRTgeometry::aRTgeometry(SEXP sdata)
{
	string classname = SEXP_TO_STRING( getAttrib(sdata, R_ClassSymbol) );

    if(classname == "SpatialPointsDataFrame")
	{
		PrintSilent("Converting points to TerraLib format ... ");
		MultiGeom.setGeometry( GetTePointSet(sdata) );
	}
	else if(classname == "SpatialPolygons" || classname == "SpatialPolygonsDataFrame")
	{
		PrintSilent("Converting polygons to TerraLib format ... ");
		MultiGeom.setGeometry( GetTePolygonSet(sdata) );
	}
	else if(classname == "SpatialLines" || classname == "SpatialLinesDataFrame")
	{
		PrintSilent("Converting lines to TerraLib format ... ");
		MultiGeom.setGeometry( GetTeLineSet(sdata) );
	}
	else error("Invalid class\n");
	PrintSilentYes;
}

SEXP aRTgeometry::GetData()
{
	if( MultiGeom.hasPoints()   ) return GetPoints();
	if( MultiGeom.hasPolygons() ) return GetPolygons();
	if( MultiGeom.hasLines()    ) return GetLines();
	if( MultiGeom.hasCells()    ) return GetCells();

	return R_NilValue;
}

void aRTgeometry::InitSpDefs(SEXP defs)
{
	PolygonDef  = GetListElement(defs, "polygon" );
	PolygonsDef = GetListElement(defs, "polygons");
	LineDef     = GetListElement(defs, "line"    );
	LinesDef    = GetListElement(defs, "lines"   );
}

// this is the order to appear inside R. S is from Summary
enum aRTgeometrySenum
{
	aRTgeometrySClass = 0,
	aRTgeometrySType,
	aRTgeometrySObjects,
	aRTgeometrySSize
};

SEXP aRTgeometry::Summary()
{
	SEXP info, names;
	int size;
	string type;

	info    = allocVector(VECSXP, aRTgeometrySSize); 
    names   = allocVector(STRSXP, aRTgeometrySSize); 

	STRING_LIST_ITEM(info, names, aRTgeometrySClass, "class", Class());

	     if(MultiGeom.hasPoints())	 { size = MultiGeom.points_.size();	  type = "points";   }
	else if(MultiGeom.hasPolygons()) { size = MultiGeom.polygons_.size(); type = "polygons"; }
	else if(MultiGeom.hasLines())    { size = MultiGeom.lines_.size();    type = "lines";    }
	else if(MultiGeom.hasCells())    { size = MultiGeom.cells_.size();    type = "cells";    }
    else                             { size = 0;                          type = "none";     }
	
	INTEGER_LIST_ITEM(info, names, aRTgeometrySObjects, "objects", size);
    STRING_LIST_ITEM (info, names, aRTgeometrySType,    "type",    type);

	setAttrib(info, R_NamesSymbol, names); 

    return info;
}

SEXP aRTgeometry::Print()
{
	stringstream s;

	 s << artOBJECT("aRTgeometry") << "\n\n";

	      if(MultiGeom.hasPoints())   s << MultiGeom.points_.size()   << " points";
	 else if(MultiGeom.hasPolygons()) s << MultiGeom.polygons_.size() << " polygons";
	 else if(MultiGeom.hasLines())    s << MultiGeom.lines_.size()    << " lines";
	 else if(MultiGeom.hasCells())    s << MultiGeom.cells_.size()    << " cells";
     else                             s << "Empty!";

	 s << endl << endl;
	 Rprintf( StreamToChar(s) );
	 return RNULL;
}

aRTcomponent* aRTgeometry::GetClip(aRTgeometry* refpols)
// clip works with only polygons and lines, with a reference being always a polygon set
{
	TeMultiGeometry mg;

	// if we have polygons, do it with polygons
	if(GetTePolygonSet().size())
	{
		TePolygonSet result;
		TePolygonSet built_result;

		for(int i = 0; i < GetTePolygonSet().size(); i++)
		{
			TePolygonSet input;
			input.add(GetTePolygonSet()[i]);
	
			bool ok = TeOVERLAY::TeIntersection (input, refpols->GetTePolygonSet(), result);
			if(!ok) error("Error while executing the intersection.");
			
			if(result.size())
			{
				for(int j = 0; j < result.size(); j++)
				{
					result[j].objectId(GetTePolygonSet()[i].objectId());
					built_result.add(result[j]);
				}
				result.clear();
			}
		}

		mg.polygons_ = built_result;
	}
	else if(GetTeLineSet().size())
	{
		TeMultiGeometry result;
		TeLineSet built_result;

		for(int i = 0; i < GetTeLineSet().size(); i++)
		{
			TeLineSet input;
			input.add(GetTeLineSet()[i]);

			result = TeOVERLAY::TeIntersection (input, refpols->GetTePolygonSet());

			if(result.lines_.size())
			{
				for(int j = 0; j < result.lines_.size(); j++)
				{
					result.lines_[j].objectId(GetTeLineSet()[i].objectId());
					built_result.add(result.lines_[j]);
				}
				result.clear();
			}
		}

		mg.lines_ = built_result;
	}
	else
	{
		error("Not a valid geometry");
	}

	aRTgeometry* ret = new aRTgeometry(mg);
	return ret;
}


// CONVERSORS __TO__ TERRALIB OBJECTS
TePointSet aRTgeometry::GetTePointSet(SEXP sps)
{
	TePointSet ps;
	
	SEXP data = R_do_slot( sps, CharToSEXP("data")   );
	SEXP geom = R_do_slot( sps, CharToSEXP("coords") );

	SEXP rownames;
/*
	rownames = getAttrib(geom, R_RowNamesSymbol);

	const int nsize = LENGTH(rownames);
	cout << "nsize: " << nsize << endl;
	for(int i = 0; i < nsize; i++)
	{
		cout << CHAR(STRING_ELT(rownames, i)) << endl;
	}
*/
	SEXP ids  = GetListElement(data, "ID");
	const int size = LENGTH(ids);

 	if( getAttrib(ids,R_LevelsSymbol) != RNULL )
	{
		for(int i = 0; i < size; i++)
		{
			TePoint point(REAL(geom)[i], REAL(geom)[i+size] );
			point.objectId( GetStringFactorPos(ids, i) );
			ps.add(point);
		}
	}
	else
	{	
		for(int i = 0; i < size; i++)
		{ // TODO: testar se eh string ou integer ou outro antes de inserir o dado?
			TePoint point(REAL(geom)[i], REAL(geom)[i+size] );
			point.objectId( CHAR(STRING_ELT(ids, i)) );
			ps.add(point);
		}
	}
	
	return ps;
}

TePolygonSet aRTgeometry::GetTePolygonSet(SEXP sps)
{
    TePolygonSet ps;

    SEXP geom = R_do_slot( sps, CharToSEXP("polygons") );

	const int size = LENGTH(geom);

    for(int i = 0; i < size; i++)
    {
		SEXP spols = R_do_slot( VECTOR_ELT(geom, i), CharToSEXP("Polygons") );
		SEXP ID    = R_do_slot( VECTOR_ELT(geom, i), CharToSEXP("ID") );

       	const int spols_size = LENGTH(spols);
		TePolygon pol;
		for(int j = 0; j < spols_size; j++)
		{
			SEXP spol = R_do_slot( VECTOR_ELT(spols, j), CharToSEXP("coords") );
			SEXP hole = R_do_slot( VECTOR_ELT(spols, j), CharToSEXP("hole") );

			bool hooooooooooooooooole = LOGICAL(hole)[0];
			const int spol_size = LENGTH(spol)/2;
			TeLine2D line;
			for(int k = 0; k < spol_size; k++)
			{
				TeCoord2D coord(REAL(spol)[k], REAL(spol)[k+spol_size] );
				line.add(coord);
			}

			// if the current polygon is not a hole, and there is another polygon with a 
			// hole in the list, so we insert the list in the polygon set and start
			// to build another polygon. 
			if(!hooooooooooooooooole && !pol.empty())
			{
				pol.objectId( CHAR(STRING_ELT(ID, 0)) );
				ps.add(pol);
				pol = TePolygon();
			}

            TeLinearRing lr(line);
		    pol.add(lr);

        }
		if(!pol.empty())
		{
			pol.objectId( CHAR(STRING_ELT(ID, 0)) );
			ps.add(pol);
		}
    }
	return ps;
}

TeLineSet aRTgeometry::GetTeLineSet(SEXP sls)
{
	TeLineSet ls;

    SEXP geom = R_do_slot( sls, CharToSEXP("lines") );

	const int size = LENGTH(geom);

    for(int i = 0; i < size; i++)
    {
		SEXP slines = R_do_slot( VECTOR_ELT(geom, i), CharToSEXP("Lines") );
		SEXP ID     = R_do_slot( VECTOR_ELT(geom, i), CharToSEXP("ID") );

       	const int slines_size = LENGTH(slines);
		for(int j = 0; j < slines_size; j++)
		{
			SEXP sline = R_do_slot( VECTOR_ELT(slines, j), CharToSEXP("coords") );
			const int sline_size = LENGTH(sline)/2;
			TeLine2D line;

			for(int k = 0; k < sline_size; k++)
			{
				TeCoord2D coord(REAL(sline)[k], REAL(sline)[k+sline_size] );
				line.add(coord);
			}

			line.objectId( CHAR(STRING_ELT(ID, 0)) );
			ls.add(line);
        }
    }
	return ls;
}

// CONVERSORS _TO_ SEXP 
SEXP aRTgeometry::GetLinesTeLineSet(TeLineSet& lineset)
{
	SEXP result, lines, line, mat, line_list, ID;
	int unprotect_ = 0;
	unsigned quant = 1;

    // count the number of lines. [i] and [i+1] may have the same ID
    for(unsigned i = 1; i < lineset.size(); i++)
	{
		if(lineset[i-1].geomId() != lineset[i].geomId()) quant++;
	}

	// >>
	if( quant != lineset.size() ) error("not implemented yet");

	PROTECT( result = allocVector(VECSXP, quant) );
	unprotect_ ++;

	// all lines
	for(unsigned i = 0; i < quant; i++)
	{
		// >> implementacao sem repeticao de ID;
		// each lines has one line_list
		PROTECT( lines = R_do_new_object(LinesDef) );
		PROTECT( line_list = allocVector(VECSXP, 1) ); // check if there are more than one
		PROTECT( ID        = allocVector(STRSXP, 1) );
		unprotect_ += 3;

		// >> just one line

		// esta parte vai continuar a mesma porque cada lineset[i] tem apenas uma linha
		PROTECT( line = R_do_new_object(LineDef) );
		PROTECT( mat = allocMatrix(REALSXP, lineset[i].size(), 2) );
		unprotect_ += 2;

		for(unsigned j = 0; j < lineset[i].size(); j++)
		{
			REAL(mat)[j] = lineset[i][j].x();
			REAL(mat)[j+lineset[i].size()] = lineset[i][j].y();
		}

		R_do_slot_assign( line, mkChar("coords"), mat);

		SET_VECTOR_ELT(line_list, 0, line);
		SET_STRING_ELT( ID, 0, mkChar(lineset[i].objectId().c_str()) );

		R_do_slot_assign( lines, mkChar("Lines"), line_list);
		R_do_slot_assign( lines, mkChar("ID"),    ID);

		SET_VECTOR_ELT(result, i, lines);
	}
	
	UNPROTECT(unprotect_);
	return result;
}

SEXP aRTgeometry::GetPolygonsTeCellSet(TeCellSet& cellset)
{
	SEXP result, pols, pol, mat, pol_list, ID, ringdir, hole, plotorder;

	PROTECT( result = allocVector(VECSXP, cellset.size()) );

	for(unsigned i = 0; i < cellset.size(); i++)
	{
		PROTECT( pols      = R_do_new_object(PolygonsDef) );
		PROTECT( pol       = R_do_new_object(PolygonDef) );
		PROTECT( mat       = allocMatrix(REALSXP, 5, 2) );
		PROTECT( pol_list  = allocVector(VECSXP, 1) );
		PROTECT( plotorder = allocVector(INTSXP, 1) );
		PROTECT( ID        = allocVector(STRSXP, 1) );
		PROTECT( hole      = allocVector(LGLSXP, 1) );
		PROTECT( ringdir   = allocVector(INTSXP, 1) );
		
		TeBox b = cellset[i].box();
		REAL(mat)[0] = b.x1();
		REAL(mat)[1] = b.x2();
		REAL(mat)[2] = b.x2();
		REAL(mat)[3] = b.x1();
		REAL(mat)[4] = b.x1();

		REAL(mat)[5] = b.y1();
		REAL(mat)[6] = b.y1();
		REAL(mat)[7] = b.y2();
		REAL(mat)[8] = b.y2();
		REAL(mat)[9] = b.y1();

		INTEGER(ringdir)  [0] = 1;
		INTEGER(plotorder)[0] = 1;
		LOGICAL(hole)     [0] = 0;
			
		R_do_slot_assign( pol, mkChar("coords"),  mat);
		R_do_slot_assign( pol, mkChar("hole"),    hole);
		R_do_slot_assign( pol, mkChar("ringDir"), ringdir);

		SET_VECTOR_ELT(pol_list, 0, pol);
		SET_STRING_ELT( ID, 0, mkChar(cellset[i].objectId().c_str()) );

		R_do_slot_assign( pols, mkChar("Polygons"),  pol_list);
		R_do_slot_assign( pols, mkChar("ID"),        ID);
		R_do_slot_assign( pols, mkChar("plotOrder"), plotorder);

		SET_VECTOR_ELT(result, i, pols);
	}
	
	UNPROTECT(8 * cellset.size() + 1);
	return result;
}


SEXP aRTgeometry::GetPolygonsTePolygonSet(TePolygonSet& polset)
{
	unsigned quant = 1;

	for(unsigned i = 1; i < polset.size(); i++)
		if( polset[i].objectId() != polset[i-1].objectId() )
			quant++;

	if( quant == polset.size() ) return GetPolygonsTePolygonSetWithoutIslands(polset);
	else                         return GetPolygonsTePolygonSetWithIslands(polset, quant);
}

// without islands can be optimized
SEXP aRTgeometry::GetPolygonsTePolygonSetWithoutIslands(TePolygonSet& polset)
{
    SEXP result, pols, pol, mat, pol_list, ID, ringdir, hole, plotorder;
    int unprotect_ = 0;

    PROTECT( result = allocVector(VECSXP, polset.size()) );
    unprotect_ ++;

    for(unsigned i = 0; i < polset.size(); i++)
    {
        PROTECT( pols      = R_do_new_object(PolygonsDef) );
        PROTECT( pol_list  = allocVector(VECSXP, polset[i].size()) );
        PROTECT( plotorder = allocVector(INTSXP, polset[i].size()) );
        PROTECT( ID        = allocVector(STRSXP, 1) );
        unprotect_ += 4;

        for(unsigned j = 0; j < polset[i].size(); j++)
        {
            PROTECT( pol     = R_do_new_object(PolygonDef) );
            PROTECT( mat     = allocMatrix(REALSXP, polset[i][j].size(), 2) );
            PROTECT( hole    = allocVector(LGLSXP, 1) );
            PROTECT( ringdir = allocVector(INTSXP, 1) );
            unprotect_ += 4;

            for(unsigned k = 0; k < polset[i][j].size(); k++)
            {
                REAL(mat)[k] = polset[i][j][k].x();
                REAL(mat)[k+polset[i][j].size()] = polset[i][j][k].y();
            }

            if(!j) { INTEGER(ringdir)[0] = 1; LOGICAL(hole)[0] = 0; }
            else   { INTEGER(ringdir)[0] = -1;  LOGICAL(hole)[0] = 1; }

            R_do_slot_assign( pol, mkChar("coords"),  mat);
            R_do_slot_assign( pol, mkChar("hole"),    hole);
            R_do_slot_assign( pol, mkChar("ringDir"), ringdir);

            SET_VECTOR_ELT(pol_list, j, pol);
            INTEGER(plotorder)[j] = j+1;
        }

        SET_STRING_ELT( ID, 0, mkChar(polset[i].objectId().c_str()) );

        R_do_slot_assign( pols, mkChar("Polygons"),  pol_list);
        R_do_slot_assign( pols, mkChar("ID"),        ID);
        R_do_slot_assign( pols, mkChar("plotOrder"), plotorder);

        SET_VECTOR_ELT(result, i, pols);
    }

    UNPROTECT(unprotect_);
    return result;
}

SEXP aRTgeometry::GetPolygonsTePolygonSetWithIslands(TePolygonSet& polset, unsigned quant)
{
	SEXP result, pols, pol, mat, pol_list, ID, ringdir, hole, plotorder;
	int unprotect_ = 0;
	unsigned i, it;
	
	PROTECT( result = allocVector(VECSXP, quant) );
	unprotect_ ++;

	// check how many positions each polygon uses
	unsigned each[quant];
	for(i = 0; i < quant; i++) each[i] = 0;

	i = 0;
	for(it = 1; it < polset.size(); it++)
	{
		each[i] += polset[it-1].size();

		if( polset[it].objectId() != polset[it-1].objectId() ) i++;
	}
    each[i] += polset[it-1].size();
	
	it = 0;
	for(i = 0; i < quant; i++) // for each object (not each polygon)
	{
		PROTECT( pols      = R_do_new_object(PolygonsDef) );
		PROTECT( pol_list  = allocVector(VECSXP, each[i]) );
		PROTECT( plotorder = allocVector(INTSXP, each[i]) );
		PROTECT( ID        = allocVector(STRSXP, 1      ) );
		unprotect_ += 4;

		SET_STRING_ELT( ID, 0, mkChar(polset[it].objectId().c_str()) );
		
		for(unsigned eachit = 0; eachit < each[i];) // for each polygon of the object (islands/holes)
		{
			for(unsigned j = 0; j < polset[it].size(); j++) // polset[i] has a polygon (and its holes)
			{
				PROTECT( pol     = R_do_new_object(PolygonDef) );
				PROTECT( mat     = allocMatrix(REALSXP, polset[it][j].size(), 2) );
				PROTECT( hole    = allocVector(LGLSXP, 1) );
				PROTECT( ringdir = allocVector(INTSXP, 1) );
				unprotect_ += 4;
			
				for(unsigned k = 0; k < polset[it][j].size(); k++) // each point of the polygon
				{
					REAL(mat)[k] = polset[it][j][k].x();
					REAL(mat)[k+polset[it][j].size()] = polset[it][j][k].y();
				}
	
				// alguma coisa do tipo...
				if(!j) { INTEGER(ringdir)[0] = -1; LOGICAL(hole)[0] = 0; }
				else   { INTEGER(ringdir)[0] = 1;  LOGICAL(hole)[0] = 1; }
				
				R_do_slot_assign( pol, mkChar("coords"),  mat);
				R_do_slot_assign( pol, mkChar("hole"),    hole);
				R_do_slot_assign( pol, mkChar("ringDir"), ringdir);
	
				SET_VECTOR_ELT(pol_list, eachit, pol);
				INTEGER(plotorder)[eachit] = eachit+1;
				eachit++;
			}
			it++;
		}
		
		R_do_slot_assign( pols, mkChar("Polygons"),  pol_list);
		R_do_slot_assign( pols, mkChar("ID"),        ID);
		R_do_slot_assign( pols, mkChar("plotOrder"), plotorder);

		SET_VECTOR_ELT(result, i, pols);
	}
	
	UNPROTECT(unprotect_);
	return result;
}

SEXP aRTgeometry::GetPointsTePointSet(TePointSet& ps)
{
	const int size = ps.size();
	SEXP result, mat, colnames, ID;
	
	PROTECT( result   = allocVector(VECSXP, 2) );
	PROTECT( colnames = allocVector(STRSXP, 2) );
    PROTECT( ID       = allocVector(STRSXP, size) );
    PROTECT( mat      = allocMatrix(REALSXP, size, 2) );

	SET_STRING_ELT( colnames, 0, mkChar("mat") );
	SET_STRING_ELT( colnames, 1, mkChar("ID")  );

	for(int j = 0; j < size; j++)
	{
		REAL(mat)[j]      = ps[j].location().x();
		REAL(mat)[j+size] = ps[j].location().y();

		SET_STRING_ELT( ID, j, mkChar( ps[j].objectId().c_str() ) );
    }

	SET_VECTOR_ELT( result, 0, mat );
    SET_VECTOR_ELT( result, 1, ID  );

    setAttrib(result, R_NamesSymbol, colnames);

	UNPROTECT(4);
	return result;
}

// TeGeometryAlgorithms.h
// TODO: deve haver um jeito melhor de chamar este tipo de funcao
// sem necessitar da implementacao de cada uma delas separadamente
SEXP aRTgeometry::Simplify(SEXP data)
{
	double snap    = GET_REAL_ELEMENT(data, "snap");
	double maxdist = GET_REAL_ELEMENT(data, "maxdist");
	
	TeLineSet ls = MultiGeom.getLines();
	if(ls.size() > 0)
	{
		PrintSilent("Simplifying %d lines ... ", ls.size());
		for(unsigned i = 0; i < ls.size(); i++)
		{
			TeLine2D l2d = ls[i];
			if(!TeLineSimplify(l2d, snap, maxdist))
				error("Couldn't simplify the line\n");
		}
		PrintSilentYes;
	}

	TePolygonSet ps = MultiGeom.getPolygons();
	if(ps.size() > 0)
	{
		PrintSilent("Simplifying %d polygons ... ", ps.size());

		for(unsigned i = 0; i < ps.size(); i++)
		{
			for(unsigned j = 0; j < ps[i].size(); j++)
			{
				TeLine2D l2d = ps[i][j];
				if(!TeLineSimplify(l2d, snap, maxdist))
					error("Couldn't simplify the polygon\n");
			}
		}
		PrintSilentYes;
	}

	return RNULL;
}

// TeGeometryAlgorithms.h
// TODO: deve haver um jeito melhor de chamar este tipo de funcao
// sem necessitar da implementacao de cada uma delas separadamente
SEXP aRTgeometry::RemoveDuplicated()
{
	TeLineSet ls = MultiGeom.getLines();
	if(ls.size() > 0)
	{
		PrintSilent("Removing duplicated coords in %d lines ... ", ls.size());
		for(unsigned i = 0; i < ls.size(); i++)
		{
			TeLine2D l2d = ls[i];
		    TeRemoveDuplicatedCoordinates(l2d);
		}
		PrintSilentYes;
	}

	TePolygonSet ps = MultiGeom.getPolygons();
	if(ps.size() > 0)
	{
		PrintSilent("Removing duplicated coords in %d polygons ... ", ps.size());

		for(unsigned i = 0; i < ps.size(); i++)
		{
			for(unsigned j = 0; j < ps[i].size(); j++)
			{
				TeLine2D l2d = ps[i][j];
		    	TeRemoveDuplicatedCoordinates(l2d);
			}
		}
		PrintSilentYes;
	}

	return RNULL;
}

}

