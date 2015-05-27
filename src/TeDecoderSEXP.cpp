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

#include "TeDecoderSEXP.h"
#include "SEXPbasic.h"

#include <stdio.h>

extern "C"
{

TeDecoderSEXP::TeDecoderSEXP(SEXP value, TeRasterParams& params)
    : TeRBasic(value), TeDecoder(params)
{
	data(value);
	params_.status_ = TeRasterParams::TeReadyToRead;
}

TeDecoderSEXP::TeDecoderSEXP(TeDecoder* other)
{
	params_ = other->params();
	params_.decoderIdentifier_ = "R";
	SEXP raster, colnames, x, y, z;

	PROTECT(colnames = allocVector(STRSXP, 3));
	PROTECT(raster = allocVector(VECSXP, 3));
	PROTECT(x = allocVector(REALSXP, other->params().ncols_));
	PROTECT(y = allocVector(REALSXP, other->params().nlines_));
    PROTECT(z = allocMatrix(REALSXP, other->params().ncols_, other->params().nlines_));
	
	const int tamanho = other->params().ncols_;
	double value;
	// build _z_
	for(int i = 0; i < other->params().nlines_; i++)
	{
		for(int j = 0; j < other->params().ncols_; j++)
		{
			other->getElement(j,i,value);
			if(value == params_.dummy_[0]) value = NA_REAL;
			REAL(z)[(params_.nlines_ - 1 - i) * tamanho + j] = value;
		}
	}

	// build _x_ 
	double x1 = other->params().boundingBox().x1();
	double x2 = other->params().boundingBox().x2();
	double grow = (x2 - x1)/(other->params().ncols_ - 1);
	for(int i = 0; i < other->params().ncols_; i++)
		REAL(x)[i] = x1 + i * grow;
	
	// build _y_ (x and y can have different lengths)
	double y1 = other->params().boundingBox().y1();
	double y2 = other->params().boundingBox().y2();
    grow = (y2 - y1)/(other->params().nlines_ - 1);
	for(int i = 0; i < other->params().nlines_; i++)
		REAL(y)[i] = y1 + i * grow;
	
    SET_VECTOR_ELT(raster, 0, x);
    SET_VECTOR_ELT(raster, 1, y);
    SET_VECTOR_ELT(raster, 2, z);
	
    SET_STRING_ELT( colnames, 0, mkChar("x") );
    SET_STRING_ELT( colnames, 1, mkChar("y") );
    SET_STRING_ELT( colnames, 2, mkChar("z") );
	
	setAttrib(raster, R_NamesSymbol, colnames);	
	data(raster);
	UNPROTECT(5);
}

bool TeDecoderSEXP::setElement(int col, int lin, double val, int band)
{
	// never used
    return false;
}

bool TeDecoderSEXP::getElement(int col, int lin, double& val, int band)
{
	if(band) return false;
	
	SEXP z = GetListElement(data(), "z");
	val = REAL(z)[(params_.nlines_ - 1 - lin)*params_.ncols_ + col];
	
    if( isnan(val) || ISNA(val) ) val = params_.dummy_[0];

	return true;
}

void TeDecoderSEXP::init()
{
    if( data() == RNULL ) params_.status_ = TeRasterParams::TeNotReady;

    params_.vmin_.resize(1);
    params_.vmax_.resize(1);

    double val;
    getElement(1, 1, val, 0);
    bool found_valid = false;
    params_.setDummy(-1);

    for(int i = 0; i < params_.nlines_; i++)
    {
        for(int j = 0; j < params_.ncols_; j++)
        {
            getElement(j,i,val,0);
            if(val != params_.dummy_[0])
            {
              if(val > params_.vmax_[0] || !found_valid) params_.vmax_[0] = val;
              if(val < params_.vmin_[0] || !found_valid) params_.vmin_[0] = val;
			  found_valid = true;
            }
        }
    }
    params_.useDummy_ = true;
}

bool TeDecoderSEXP::clear()
{
	return true;
}

}

