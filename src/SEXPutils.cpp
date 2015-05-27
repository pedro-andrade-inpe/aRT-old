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

#include "SEXPutils.h"

#include <TeInitRasterDecoders.h>
#include <TeImportRaster.h>
//#include <TeQuerierParams.h>

#include <sstream>

extern "C" {

namespace SEXPutils {

// visual data is a list which each attribute will be
// a data of the TeVisual
TeVisual* GetTeVisual(SEXP visual, TeGeomRep geomrep)
{
	int style   = GET_INTEGER_ELEMENT(visual, "style"  );
	int width   = GET_INTEGER_ELEMENT(visual, "width"  );
	int cwidth  = GET_INTEGER_ELEMENT(visual, "cwidth" );
	int transp  = GET_INTEGER_ELEMENT(visual, "transp" );
	int ctransp = GET_INTEGER_ELEMENT(visual, "ctransp");
	int size    = GET_INTEGER_ELEMENT(visual, "size"   );
	SEXP color  = GetListElement     (visual, "color"  );
	SEXP ccolor = GetListElement     (visual, "ccolor" );
	
	TeVisual *tvisual = new TeVisual(geomrep);

    TeColor tcolor  = SEXPtoColors(color)[0];
    TeColor tccolor = SEXPtoColors(ccolor)[0];
	
	tvisual->style(style);
	tvisual->size(size);
	tvisual->width(width);
	tvisual->contourWidth(cwidth);
	tvisual->transparency(transp);
	tvisual->contourTransparency(ctransp);
	tvisual->color(tcolor); 
	tvisual->contourColor(tccolor);
	
	return tvisual;
}

vector<TeColor> SEXPtoColors(SEXP colors)
{
    vector<TeColor> colorVec;
    int size = LENGTH(colors)/3;
    
    for(int i = 0; i < size; i++)
    {
        TeColor cor(INTEGER(colors)[3*i],
                    INTEGER(colors)[3*i+1], 
                    INTEGER(colors)[3*i+2]);
        colorVec.push_back(cor);
    }
    return colorVec;
}

SEXP GetBoxTeBox(TeBox box){
    SEXP *coords, colnames;
	SEXP result;

    PROTECT(result   = allocVector(VECSXP, 2));
    PROTECT(colnames = allocVector(STRSXP, 2));

    coords = new SEXP[2];

    PROTECT(coords[0] = allocVector(REALSXP,5));
    PROTECT(coords[1] = allocVector(REALSXP,5));

    SET_STRING_ELT( colnames, 0, mkChar("x") );
    SET_STRING_ELT( colnames, 1, mkChar("y") );

    REAL(coords[0])[0] = box.x1();
    REAL(coords[1])[0] = box.y1();

    REAL(coords[0])[1] = box.x2();
    REAL(coords[1])[1] = box.y1();

    REAL(coords[0])[2] = box.x2();
    REAL(coords[1])[2] = box.y2();

    REAL(coords[0])[3] = box.x1();
    REAL(coords[1])[3] = box.y2();

	REAL(coords[0])[4] = box.x1();
    REAL(coords[1])[4] = box.y1();

    SET_VECTOR_ELT(result, 0, coords[0]);
    SET_VECTOR_ELT(result, 1, coords[1]);

    setAttrib(result, R_NamesSymbol, colnames);
    delete[] coords;

    UNPROTECT(4);

    return result;
}              

TeKeys SEXPtoKeys(SEXP data)
{
	TeKeys result;
	for(int i = 0; i < LENGTH(data); i++)
		result.push_back( CHAR(STRING_ELT(data,i)) );

	return result;
}

SEXP GetKeys(TeKeys keys)
{
	SEXP result;
    PROTECT( result = allocVector( STRSXP, keys.size() ) );

    for(unsigned i = 0; i < keys.size(); i++)
	{
        SET_STRING_ELT( result, i, mkChar( keys[i].c_str() ) );
	}

    UNPROTECT(1);
	return result;
}

}

}

