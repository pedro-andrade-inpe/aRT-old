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

//  aRTconn.h - class aRTconn, implements a "virtual" SGBD connection

#ifndef ART_CONN_H_
#define ART_CONN_H_

#include "aRTcomponent.h"

//#ifndef uint
//#define uint unsigned
//#endif

enum aRTdatabaseType
{
	aRTmySQL,
	aRTpostgres,
	aRTsqlite,
	aRTpostgis
	// ...
};

class aRTconn : public aRTcomponent
{
	public:
		aRTconn(string user,
		        string pass,
		        unsigned Port,
		        string host,
		        string dbms);

		SEXP Summary();
		SEXP Print();
		aRTcomponent* OpenDb  (SEXP data);
		aRTcomponent* CreateDb(SEXP data);
		SEXP DeleteDb(SEXP data);
		SEXP AddPermission  (SEXP data);
		SEXP DropUser       (SEXP data);
		SEXP GetPermissions (SEXP data);

		SEXP List();
        string Class() { return "aRT"; }
	private:
		TeDatabase* NewTeDatabase();

		string User;
		string Password;
		unsigned Port;
		string Host;
		aRTdatabaseType Type;
};

#endif

