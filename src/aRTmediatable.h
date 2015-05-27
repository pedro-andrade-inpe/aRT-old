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

#ifndef ART_MEDIATABLE_H_
#define ART_MEDIATABLE_H_

#include "aRTtable.h"

extern "C" {

// a terraLib media table is represented in the database by two tables
// Here, we intend to make the user think it is just one, always two
// columns (id and url)
		
class aRTmediatable : public aRTtable
{
	public:
		aRTmediatable(TeDatabase* db, string layer, string tablename);

		void Create(SEXP data);
		
		SEXP CreateRelation(SEXP data) { error("Can't do it in a media table\n"); return RNULL; }
		SEXP CreateColumn  (SEXP data) { error("Can't do it in a media table\n"); return RNULL; }
		SEXP UpdateColumns (SEXP data) { error("Can't do it in a media table\n"); return RNULL; }
		
		SEXP AddRows       (SEXP data);
		SEXP GetData();
		
		void AddUrl(string id_, string site);
		SEXP Summary();
		SEXP Print(); 
};

}

#endif
