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

#ifndef TE_DECODER_SEXP_H_
#define TE_DECODER_SEXP_H_

#include "TeRBasic.h"
#include <TeDecoder.h>
#include <TeDecoderDatabase.h>

extern "C"
{

class TeDecoderSEXP : public TeRBasic, public TeDecoder
{
	public:
		TeDecoderSEXP(SEXP value, TeRasterParams& params);
		TeDecoderSEXP(TeDecoder* other);
		TeDecoderSEXP(TeRasterParams& params) { /* implementar */ }
		bool setElement(int col, int lin, double val, int band=0 );
		bool getElement(int col, int lin, double& val, int band=0 );
		void init();
		bool clear();
};

}

#endif
