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

#include "aRTquerier.h"
#include "aRTgeometry.h"
#include "SEXPutils.h"

#include <TeQuerierParams.h>
#include <sstream>

using namespace std;

aRTquerier::aRTquerier(TeQuerierParams& params, TeGeomRep geomrep)
{
	PrintSilent("Creating querier ... ");
	GeomRep = geomrep;
	Querier = new TeQuerier(params);
	if(! Querier->getNumTimeFrames() ) Querier->loadInstances();

	ActualElem = 0;
	ActualFrame = 0;
    PrintSilentYes;
}

SEXP aRTquerier::GetData(SEXP options)
{
	int quant = GET_INTEGER_ELEMENT(options, "quantity");

	if(quant > Querier->numElemInstances() - ActualElem || quant <= 0)
		quant = Querier->numElemInstances() - ActualElem;

	PrintSilent("Reading %d element(s) from %d/%d ... ", quant, ActualElem, Querier->numElemInstances());

	if(quant == 0)
	{
		PrintSilentYes;
		return R_NilValue;
	}

	ActualElem += quant;

	TeSTInstance sti;
	TeMultiGeometry mg, mgresult;
	int i;
	unsigned j;
	TePropertyVector propvec;
	SEXP colnames = RNULL, sexpatts = RNULL, rownames = RNULL, sexpgeom = RNULL;
	SEXP* each_column = NULL;
	int unprotect_ = 0;

    if( !Querier->fetchInstance(sti) )
	{
		PrintSilentNo;
		warning("Could not fetch the instance\n");
		return R_NilValue;
	}
	PrintSilentYes;

	// attributes allocation and fill colnames
	unsigned attsize = sti.getPropertyVector().size();
	if(attsize)
	{
    	sexpatts = PROTECT( allocVector( VECSXP, attsize ) );
    	colnames = PROTECT( allocVector( STRSXP, attsize ) );
    	rownames = PROTECT( allocVector( STRSXP, quant   ) );
		each_column = new SEXP[attsize];		
		unprotect_ += 3;
		propvec = sti.getPropertyVector();

		for(j = 0; j < attsize; j++)
		{
	        switch(propvec[j].attr_.rep_.type_)
	        {
	            case TeREAL: each_column[j] = PROTECT( allocVector(REALSXP, quant) ); break;
	            case TeINT:  each_column[j] = PROTECT( allocVector(INTSXP,  quant) ); break;
	            default:     each_column[j] = PROTECT( allocVector(STRSXP,  quant) ); break;
	        }
	        SET_STRING_ELT( colnames, j, mkChar(propvec[j].attr_.rep_.name_.c_str()) );
		}
		unprotect_ += attsize;
		setAttrib(sexpatts, R_NamesSymbol, colnames);
	}

	for(i = 0; i < quant;) // each instance
	{
		// attributes
		if(attsize)
		{
	        propvec = sti.getPropertyVector();
	        for(j = 0; j < attsize; j++)
	        {
				const char* val = propvec[j].value_.c_str();
            	switch(propvec[j].attr_.rep_.type_)
	        	{
	            	case TeREAL: REAL          (each_column[j])[i] = atof(val);  break;
	            	case TeINT:  INTEGER       (each_column[j])[i] = atoi(val);  break;
	            	default:     SET_STRING_ELT(each_column[j], i, mkChar(val)); break;
	        	}
			}
		}

		// geometries. Simple, eh?
		mgresult = sti.geometries();
		
		i++; // IMPORTANT! the iterator increases here
	
		// rownames must be here to start it from 1.
		if(attsize)
		{
			stringstream str;
			str << i;
			SET_STRING_ELT(rownames, i-1, mkChar(str.str().c_str()));
		}

		if(i < quant) Querier->fetchInstance(sti);
	}

	if(attsize)
	{
	    for(j = 0; j < attsize; j++) SET_VECTOR_ELT(sexpatts, j, each_column[j]);

		sexpatts = PROTECT(AsDataFrame(sexpatts, rownames));
		unprotect_++;
		
		if(GeomRep == TeGEOMETRYNONE)
		{
			UNPROTECT(unprotect_);
			return sexpatts;
		}
	}

	if(GeomRep != TeGEOMETRYNONE)
	{
		// TODO: apagar as outras geometrias do mg, deixando apenas a selecionada pelo usuario
		aRTgeometry* pointer = new aRTgeometry(mgresult);
		sexpgeom = PROTECT(R_MakeExternalPtr((void*) pointer, mkChar("aRT"), R_NilValue));
		unprotect_++;
		
		if(!attsize)
		{
			UNPROTECT(unprotect_);
			return sexpgeom; 
		}
	}

   	SEXP result  = PROTECT(allocVector( VECSXP, 2 ));
    colnames = PROTECT(allocVector( STRSXP, 2 ));
	unprotect_+=2;

	SET_STRING_ELT(colnames, 0, mkChar("geometry"));
	SET_STRING_ELT(colnames, 1, mkChar("attributes"));

	SET_VECTOR_ELT(result, 0, sexpgeom);
	SET_VECTOR_ELT(result, 1, sexpatts);
	
	setAttrib(result, R_NamesSymbol, colnames);
	
	UNPROTECT(unprotect_);
	return result;
}

aRTquerier::~aRTquerier()
{
//	PrintSilent("Deleting one aRTquerier object from memory\n");
//	delete Querier;
}
	
// this is the order to appear inside R. S is from Summary
enum aRTquerierSenum
{
	aRTquerierSClass = 0,
	aRTquerierSSlides,
	aRTquerierSSlide,
	aRTquerierSElements,
	aRTquerierSElement,
	aRTquerierSSize
};
					   
SEXP aRTquerier::Summary()
{

    SEXP info, names;

	info    = allocVector(VECSXP, aRTquerierSSize);  // List containing the connection attributes
    names   = allocVector(STRSXP, aRTquerierSSize);  // Names Vector

	STRING_LIST_ITEM(info,  names, aRTquerierSClass,    "class",    Class());
    INTEGER_LIST_ITEM(info, names, aRTquerierSElements, "elements", Querier->numElemInstances());
    INTEGER_LIST_ITEM(info, names, aRTquerierSElement,  "element",  ActualElem);
    INTEGER_LIST_ITEM(info, names, aRTquerierSSlides,   "frames",   Querier->getNumTimeFrames());
    INTEGER_LIST_ITEM(info, names, aRTquerierSSlide,    "frame",    ActualFrame);
	
	setAttrib(info, R_NamesSymbol, names); // Set the names of the list

    return info;
}

SEXP aRTquerier::Print()
{

	stringstream s;

	if (!IsValid()) error("Invalid object!");
		 
	s << artOBJECT("aRTquerier") << "\n\n";

    // all querier has a theme associated, so if it has a layer we will not show the theme
	if( Querier->params().layer() ) { s << "Layer: \"" << Querier->params().layer()->name() << "\"" << endl; }
	else                            { s << "Theme: \"" << Querier->params().theme()->name() << "\"" << endl; }
	
	s << "Retrieves:";
	if( Querier->params().loadGeom() )
	{
		s << " geometry";
		if( Querier->getAttrList().size() ) s << " and";
	}
	if( Querier->getAttrList().size() ) s << " attributes";
	s << endl;
	 
	if( Querier->getNumTimeFrames() )
		s << "Frames: " << ActualFrame << "/" << Querier->getNumTimeFrames() << endl;
	else
		s << "Frames: 0" << endl;

    s << "Elements: " << ActualElem  << "/" << Querier->numElemInstances() << endl;

/*
	TeTemporalSeries ts;
	if( Querier->getTS (ts) )
	{
		s << "Temporal Series:" << "\n\n"
	      << "";
	}
	else
		s << "No temporal serie" << endl;
*/
	Rprintf( StreamToChar(s) );
	return RNULL;
}

SEXP aRTquerier::NextFrame()
{
	if(ActualFrame >= Querier->getNumTimeFrames())
		error("Next slide does not exist\n");
	
	ActualFrame++;
	ActualElem = 0;

	PrintSilent("Loading instances ... ");
	Querier->loadInstances(ActualFrame);
	PrintSilentYes;
	
	SEXP resp;
	PROTECT(resp = allocVector(INTSXP, 1));
	INTEGER(resp)[0] = Querier->numElemInstances ();
	UNPROTECT(1);
	return resp;
}

bool aRTquerier::IsValid()
{
	if(!Querier) Valid = false;

	return Valid;
}

