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

#include "aRTraster.h"

#include <TeInitRasterDecoders.h>
#include <sstream>

aRTraster::aRTraster(TeLayer* layer)
{
	Raster = layer->raster();
}

aRTraster::aRTraster(SEXP rasterdata)
{
	TeInitRasterDecoders();
    TeRasterParams params;

    SEXP x   = GetListElement(rasterdata, "x"  );
	SEXP y   = GetListElement(rasterdata, "y"  );
    SEXP dim = GetListElement(rasterdata, "dim");

	// TODO: verificar se eh necessario a projecao aqui mesmo,
	// ou entao pegar a propria projecao do layer
	TeProjection* proj = new TeNoProjection();

    params.nBands(1);
    params.projection(proj);
    params.nlines_ = INTEGER(dim)[1];
    params.ncols_ = INTEGER(dim)[0];

	params.boundingBoxLinesColumns(REAL(x)[0],
				        		   REAL(y)[0],
								   REAL(x)[1],
								   REAL(y)[1],
								   INTEGER(dim)[1],
								   INTEGER(dim)[0]);
    params.setDataType(TeDOUBLE);
    params.setPhotometric(TeRasterParams::TeMultiBand);
    params.decoderIdentifier_ = "MEM";
    params.mode_ = 'c';

	TeDecoderSEXP* decoder = new TeDecoderSEXP(rasterdata, params);
	Raster = new TeRaster();
    Raster->setDecoder(decoder);

	PrintSilent("Initializing the raster ... ");
    if(!Raster->init())
	{
		PrintSilentNo;
		error("Could not initialize the raster\n");
	}
	PrintSilentYes;
}

SEXP aRTraster::Summary()
{
	return RNULL;
}

SEXP aRTraster::Print()
{
	stringstream s;

	 s << artOBJECT("aRTraster") << "\n\n";
	 
	 s <<  endl;
	 Rprintf( StreamToChar(s) );
	 return RNULL;
}

SEXP aRTraster::GetData()
{
    TeDecoderSEXP* decoder = new TeDecoderSEXP( Raster->decoder() );
    return decoder->data();
}

SEXP aRTraster::GetPixels(SEXP options, aRTgeometry* geom)
{
	TePointSet ps = geom->GetTePointSet();
	if(ps.size())
	{
		SEXP values, ids, result, colnames;
		
		PROTECT(values   = allocVector(REALSXP, ps.size()));
		PROTECT(ids      = allocVector(STRSXP,  ps.size()));
		PROTECT(result   = allocVector(VECSXP, 2));
		PROTECT(colnames = allocVector(STRSXP, 2));
				
		for(unsigned i = 0; i < ps.size(); i++)
		{
			//cout << "(" << ps[i].location().x() << "  " << ps[i].location().y() << ")  ";
			TeCoord2D coord = Raster->coord2Index(ps[i].elem());
			//cout << coord.x_ << "  " << coord.y_ << endl;
			double value;
			Raster->decoder()->getElement((int)coord.x_,(int)coord.y_,value);
//			cout << ps[i].objectId();
//			cout << value << endl;
			REAL(values)[i]=value;
			SET_STRING_ELT(ids, i, mkChar(ps[i].objectId().c_str()));
			//	push_back();
		}
	    SET_STRING_ELT(colnames, 0, mkChar(aRTstrId));                    
	    SET_STRING_ELT(colnames, 1, mkChar("values"));                
	                                                                     
	    setAttrib(result, R_NamesSymbol, colnames);                       
	                                                                     
		SET_VECTOR_ELT(result, 0, ids);                                   
	    SET_VECTOR_ELT(result, 1, values);                                 
		
		UNPROTECT(4);
		return result;
	}

	TePolygonSet polset = geom->GetTePolygonSet();
	if(polset.size())
	{
		//cout << Raster->nBands() << endl;
        //TePolygonSet intersect;
        //intersect = TeOverlay(box_ps, objGeom, TeINTERSECTION);

        //double raster_intersect_area = 0.0;
        //double intersec_area = TeGeometryArea (intersect);

        TePolygonSet::iterator psIt = polset.begin();
        vector<int> values;
        while (psIt != polset.end())
        {
            TeRaster::iteratorPoly raster_it(Raster);
            TeRaster::iteratorPoly raster_it_end (Raster);
            raster_it = Raster->begin ((*psIt), TeBoxPixelIn);
            raster_it_end = Raster->end ((*psIt), TeBoxPixelIn);
			while     (raster_it != raster_it_end)
            {
                int val = (int)(raster_it.operator*(0));  // OPERADOR * POR BANDA. FAZENDO COM BANDA 0
				values.push_back(val);
                ++raster_it;
            }
            ++psIt;
        }
		
		SEXP result;
		
		PROTECT(result   = allocVector(INTSXP, values.size()));

		for (int i = 0; i < values.size(); i++)
		{
			INTEGER(result)[i]=values[i];
		}

		UNPROTECT(1);

		return result;

	}
	// para pixels relacionados com poligonos:
	//iteratorPoly 	begin (TePolygon &poly, TeStrategicIterator st, int band=0)
	//int GetTeBoxPixel(string relation)
	return RNULL;
}

