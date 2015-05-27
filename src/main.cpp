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

/* In this file, we intent to convert some pointers to C++
 * objects and to call their functions. 
 * 
 * All objects in R contains only the pointer to TerraLib data.
 * This code only converts SEXPs to Te* and Te* to SEXPs, 
 * checks if the data is valid, and calls the functions.
 *
 * R side .aRTcall calls functions implemented here using the macros
 * R side .Call calls the other functions
 */

#include "aRTcomponent.h"
#include "aRTconn.h"
#include "aRTdb.h"
#include "aRTlayer.h"
#include "aRTtheme.h"
#include "aRTdlcommon.h"
#include "aRTgeometry.h"
#include "aRTraster.h"
#include "SEXPutils.h"
#include "SEXPbasic.h"

extern "C" {

// converts a SEXP SexpValue_ to a Type_* and puts it into Pointer_
#define SEXP_TO_ARTP(SexpValue_, Type_, Pointer_) \
  Pointer_ = (Type_*) getObj(SexpValue_);

#define ARTP_TO_SEXP(Pointer_, SexpValue_)                                      \
  if (Pointer_ == NULL) SexpValue_ = R_NilValue;                                \
  else                                                                          \
  {                                                                             \
    SexpValue_ = R_MakeExternalPtr((void*) Pointer_, mkChar("aRT"),R_NilValue); \
  }

// check if a SEXP Pointer_ is a valid aRTcomponent*. If not, returns Return_.
// This macro must be inside a function.
#define CHECK_VALID(SPointer_)                              \
{                                                           \
    aRTcomponent* art_component_;                           \
    SEXP_TO_ARTP(SPointer_, aRTcomponent, art_component_);  \
    if( !art_component_ -> IsValid() ) {                    \
        error("Invalid object");                            \
    }                                                       \
}

// declares a function called FuncionName, that receives a SEXP Pointer
// as argument. It converts the Pointer to a Type*, calls the function 
// Call and then prints its returned value. The function can have 0 or
// 1 argument, and there is one marco for both.

#define SEXP_FUNC_0(FunctionName_, Class_, Call_) \
SEXP FunctionName_(SEXP Pointer) {                \
    CHECK_VALID(Pointer);                         \
    Class_* obj;                                  \
    SEXP_TO_ARTP(Pointer, Class_, obj)            \
    return obj -> Call_();                        \
}

#define SEXP_FUNC_1(FunctionName_, Class_, Call_) \
SEXP FunctionName_(SEXP Pointer, SEXP arg_) {     \
    CHECK_VALID(Pointer);                         \
    Class_* obj;                                  \
    SEXP_TO_ARTP(Pointer, Class_, obj);           \
    return obj->Call_(arg_);                      \
}

// a function that gets two pointers as args
#define SEXP_FUNCTION_2P(FunctionName_, Type_, OtherType_, Call_)    \
SEXP FunctionName_(SEXP SPointer, SEXP SOptions, SEXP SOtherPointer) \
{                                                                    \
	CHECK_VALID(SPointer);                                           \
	CHECK_VALID(SOtherPointer);                                      \
                                                                     \
	Type_* obj;                                                      \
	OtherType_* otherobj;                                            \
                                                                     \
	SEXP_TO_ARTP(SPointer, Type_, obj);                              \
	SEXP_TO_ARTP(SOtherPointer, OtherType_, otherobj);               \
                                                                     \
	return obj->Call_(SOptions, otherobj);                           \
}

// Declaring some functions using the above macros
	
//          FUNCTION NAME        TYPE          CALL
//          ==================== ============= ================
SEXP_FUNC_0(cppPrint,            aRTcomponent, Print           )
SEXP_FUNC_0(cppList,             aRTcomponent, List            ) // showDbs, ...
SEXP_FUNC_0(cppSummary,          aRTcomponent, Summary         )
SEXP_FUNC_0(cppShowThemes,       aRTdb,        ShowThemes      )
SEXP_FUNC_0(cppShowViews,        aRTdb,        ShowViews       )
SEXP_FUNC_0(cppDShowTables,      aRTdb,        ShowTables      )
SEXP_FUNC_0(cppGetBox,           aRTlayer,     GetBox          )
SEXP_FUNC_0(cppGetID,            aRTlayer,     GetID           )
SEXP_FUNC_0(cppLShowTables,      aRTlayer,     ShowTables      )
SEXP_FUNC_0(cppGetProj,          aRTlayer,     GetProj         )
SEXP_FUNC_0(cppNextFrame,        aRTquerier,   NextFrame       )
SEXP_FUNC_0(cppGetTableData,     aRTtable,     GetData         )
SEXP_FUNC_0(cppGetVisual,        aRTtheme,     GetVisual       )
SEXP_FUNC_0(cppGetGeo,           aRTgeometry,  GetData         )
SEXP_FUNC_0(cppGetRast,          aRTraster,    GetData         )
		
SEXP_FUNC_1(cppSetVisual,        aRTtheme,     SetVisual       )
SEXP_FUNC_1(cppAddPoints,        aRTlayer,     AddPoints       )
SEXP_FUNC_1(cppAddPolygons,      aRTlayer,     AddPolygons     )
SEXP_FUNC_1(cppAddLines,         aRTlayer,     AddLines        )
SEXP_FUNC_1(cppAddShape,         aRTlayer,     AddShape        )
SEXP_FUNC_1(cppCreateViewFromDb, aRTdb,        CreateView      )
SEXP_FUNC_1(cppAddRaster,        aRTlayer,     AddRaster       )
SEXP_FUNC_1(cppCreateColumn,     aRTtable,     CreateColumn    )
SEXP_FUNC_1(cppAddRows,          aRTtable,     AddRows         )
SEXP_FUNC_1(cppUpdateColumns,    aRTtable,     UpdateColumns   )
SEXP_FUNC_1(cppAddPermission,    aRTconn,      AddPermission   )
SEXP_FUNC_1(cppDropUser,         aRTconn,      DropUser        )
SEXP_FUNC_1(cppGetPermissions,   aRTconn,      GetPermissions  )
SEXP_FUNC_1(cppCreateRelation,   aRTtable,     CreateRelation  )
SEXP_FUNC_1(cppSetVisible,       aRTtheme,     SetVisible      )
SEXP_FUNC_1(cppGetMetric,        aRTlayer,     GetMetric       )
SEXP_FUNC_1(cppGetQuerierData,   aRTquerier,   GetData         )
SEXP_FUNC_1(cppDeleteLayer,      aRTdb,        DeleteLayer     )
SEXP_FUNC_1(cppDeleteView,       aRTdb,        DeleteView      )
SEXP_FUNC_1(cppDeleteTheme,      aRTdb,        DeleteTheme     )
SEXP_FUNC_1(cppDeleteTable,      aRTdlcommon,  DeleteTable     )
SEXP_FUNC_1(cppDeleteDb,         aRTconn,      DeleteDb        )
SEXP_FUNC_1(cppReload,           aRTdb,        Reload          )

///////////////////////////////////////////////////////////////////////
SEXP_FUNC_1(cppSimplify,         aRTgeometry,  Simplify        )
SEXP_FUNC_0(cppRemoveDuplicated, aRTgeometry,  RemoveDuplicated)
///////////////////////////////////////////////////////////////////////

SEXP_FUNCTION_2P(cppGetDistance,         aRTlayer,   aRTlayer,    GetDistance             )
SEXP_FUNCTION_2P(cppGetRelation,         aRTlayer,   aRTlayer,    GetRelation             )
SEXP_FUNCTION_2P(cppGetNearestNeighbors, aRTlayer,   aRTlayer,    GetNearestNeighbors     )
SEXP_FUNCTION_2P(cppGetPixels,           aRTraster,  aRTgeometry, GetPixels               )
SEXP_FUNCTION_2P(cppFillCellGeom,        aRTtable,   aRTtheme,    CreateAndFillFromGeom   )
SEXP_FUNCTION_2P(cppFillCellNonSpatial,  aRTtable,   aRTtheme,    CreateAndFillNonSpatial )
SEXP_FUNCTION_2P(cppFillCellRaster,      aRTtable,   aRTlayer,    CreateAndFillFromRaster )

SEXP cppPP(SEXP value)
{
	printSEXP(value);
	return R_NilValue;
}

SEXP cppPPslot(SEXP v)
{
	int size = LENGTH(v);
	for(int i = 0; i < size; i++)
		PrintSilent("%d: %s\n", i, GetStringFactorPos(v, i));
	PrintSilent("fim");
	//	return R_do_new_object(v);
	return R_NilValue;
}

SEXP cppRemove(SEXP SPointer)
{
// LET IT REMOVED??!!
//	CHECK_VALID(SPointer); // TODO: nao fazer isso porque pode gerar um erro na tela
//	                       // para os ponteiros cujos pais foram removidos
// core em recife se comentar esta linha
	aRTcomponent* obj;
	SEXP_TO_ARTP(SPointer, aRTcomponent, obj);
//	cout << obj->Class() << endl;
//	obj->Print();
	delete obj;
	return R_NilValue;
}

SEXP cppRaster(SEXP data)
{
	aRTraster* obj = new aRTraster(data);
	SEXP pointer;

	ARTP_TO_SEXP( obj, pointer )
	return pointer;
}


SEXP cppGeometry(SEXP data)
{
	aRTgeometry* obj = new aRTgeometry(data);
	SEXP pointer;

	ARTP_TO_SEXP( obj, pointer )
	return pointer;
}

SEXP cppSilent(SEXP silent)
{
	if(silent != R_NilValue)
	{
		aRTcomponent::Silent = SEXP_TO_INTEGER(silent);
		return silent;
	}

	SEXP sx = allocVector(LGLSXP, 1);
	LOGICAL(sx)[0] = aRTcomponent::Silent;
	return sx;
}


// TODO: it doesnt work on windows. check in package RMySQL.
SEXP cppConfirm()
{
	char c[100];
	int scanvalue;

	do
	{
		Rprintf("It is a dangerous operation. Confirm? [Y/n]: ");
		scanvalue = scanf("%s", c);
	} while(c[0] != 'Y' && c[0] != 'n');

	SEXP ch = allocVector(STRSXP, 1);
	char cc[2];
	cc[0] = c[0];
	cc[1] = '\0';
    SET_STRING_ELT(ch, 0, mkChar(cc));
	return ch;
}

SEXP cppTerraLibVersion()
{
	const char* resp = TerraLibVersion();
	SEXP ch = allocVector(STRSXP, 1);
	SET_STRING_ELT(ch, 0, mkChar(resp));
	return ch;
}
		
// create a new aRT object, open a connection with a SGBD and return its pointer
SEXP cppNewaRT(SEXP user, SEXP password, SEXP port, SEXP host, SEXP dbms)
{
	SEXP pointer;
	// Database authentication parameters
	string strUser       = SEXP_TO_STRING(user);
	string strPasswd     = SEXP_TO_STRING(password);
	string strDbms       = SEXP_TO_STRING(dbms);
	port                 = AS_NUMERIC(port); // vector of double
	unsigned int intPort = (unsigned int)NUMERIC_POINTER(port)[0];
	string strHost       = SEXP_TO_STRING(host);
	aRTconn* art = new aRTconn(strUser, strPasswd, intPort, strHost, strDbms);

	ARTP_TO_SEXP( art, pointer )
	return pointer;
}

#define POINTER_FUNC(FunctionName_, Class_, Call_) \
SEXP FunctionName_(SEXP SPointer, SEXP SData)      \
{                                                  \
	CHECK_VALID(SPointer);                         \
                                                   \
	Class_* obj;                                   \
	aRTcomponent *result;                          \
    SEXP pointer;                                  \
	SEXP_TO_ARTP(SPointer, Class_, obj)            \
	result = obj -> Call_(SData);                  \
	ARTP_TO_SEXP(result, pointer);                 \
	return pointer;                                \
}

POINTER_FUNC(cppConnCreateDb,     aRTconn,  CreateDb         )
POINTER_FUNC(cppConnOpenDb,       aRTconn,  OpenDb           )
POINTER_FUNC(cppDbOpenTheme,      aRTdb,    OpenTheme        )
POINTER_FUNC(cppDbCreateLayer,    aRTdb,    CreateLayer      )
POINTER_FUNC(cppDbOpenLayer,      aRTdb,    OpenLayer        )
POINTER_FUNC(cppLayerOpenTheme,   aRTlayer, OpenTheme        )
POINTER_FUNC(cppLayerCreateTheme, aRTlayer, CreateTheme      )
POINTER_FUNC(cppLayerOpenQuerier, aRTlayer, OpenQuerier      )
POINTER_FUNC(cppThemeOpenQuerier, aRTtheme, OpenQuerier      )

POINTER_FUNC(cppLayerCreateTable, aRTlayer, CreateTable      )
POINTER_FUNC(cppLayerOpenTable,   aRTlayer, OpenTable        )
POINTER_FUNC(cppDbCreateTable,    aRTdb,    CreateTable      )
POINTER_FUNC(cppDbOpenTable,      aRTdb,    OpenTable        )
POINTER_FUNC(cppGetSetOperation,  aRTlayer, GetSetOperation  )
POINTER_FUNC(cppGetOperation,     aRTlayer, GetOperation     )
POINTER_FUNC(cppGetRaster,        aRTlayer, GetRaster        )
POINTER_FUNC(cppCell,             aRTlayer, CreateLayerCells )
POINTER_FUNC(cppImportMif,        aRTdb,    ImportMif        )
POINTER_FUNC(cppImportRaster,     aRTdb,    ImportRaster     )

// POINTER_FUNCTION(cppTable, aRTdlcommon, aRTcomponent, OpenTable)
// see why it doesn't work in aRTlayer.h

// TODO ING
SEXP cppGetClip(SEXP SPointer, SEXP SOtherPointer)
{
	CHECK_VALID(SPointer);

	aRTgeometry* obj;
	aRTgeometry* otherobj;
	aRTcomponent *result;
    SEXP pointer;
	SEXP_TO_ARTP(SPointer, aRTgeometry, obj)
	SEXP_TO_ARTP(SOtherPointer, aRTgeometry, otherobj);
	result = obj->GetClip(otherobj);
	ARTP_TO_SEXP(result, pointer);
	return pointer;
}

SEXP cppInitSpDefs(SEXP pointer)
{
	aRTgeometry::InitSpDefs(pointer);
	return R_NilValue;
}
		
// for TerraView integration:
SEXP cppOpenDbPointer(SEXP pointer)
{
	TeDatabase* db;
	aRTdb* adb;
	SEXP result;
	
	SEXP_TO_ARTP(pointer, TeDatabase, db)
	adb = new aRTdb(db);
	
	ARTP_TO_SEXP(adb, result);
	return result;
	// it must have a destructor that removes only _result_ but not _db_
}

}

