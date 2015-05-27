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

#ifndef SEXP_BASIC_H_
#define SEXP_BASIC_H_

// fazer alguma coisa aqui to tipo enum + lista de string para melhorar as 
// consultas
#define aRTstrId      "ID"
#define aRTstrPoint   "points"
#define aRTstrPolygon "polygons"
#define aRTstrLine    "lines"

extern "C"{

#ifndef R_NO_REMAP
#define R_NO_REMAP // all R macros MUST begin with Rf_ (cause of length(x))
#endif

#include <Rinternals.h>
//#include <Rdefines.h>
// redefines Free, and then doesn't work on Windows:
/* In file included ... from aRTtheme.cpp:20:
 * c:/MinGW/bin/../lib/gcc/mingw32/3.4.2/../../../../include/objidl.h:497:error: e
 * xpected `)' before '(' token
 * In file included ... from aRTtheme.cpp:20:
 * c:/MinGW/bin/../lib/gcc/mingw32/3.4.2/../../../../include/oaidl.h:765: error: ex
 * pected `)' before '(' token
 */

// macros from Rdefines
#define SET_CLASS(x, n)     setAttrib(x, R_ClassSymbol, n)

#define AS_CHARACTER(x)     coerceVector(x,STRSXP)
#define AS_NUMERIC(x)       coerceVector(x,REALSXP)

#define NUMERIC_POINTER(x)  REAL(x)

// macros from Rinternals (R_NO_REMAP avoids declaring them)
#define install      Rf_install
#define coerceVector Rf_coerceVector
#define isNull       Rf_isNull
#define mkString     Rf_mkString
#define allocMatrix  Rf_allocMatrix
#define mkChar       Rf_mkChar
#define allocVector  Rf_allocVector
#define protect      Rf_protect
#define unprotect    Rf_unprotect
#define setAttrib    Rf_setAttrib
#define getAttrib    Rf_getAttrib
#define error        Rf_error
#define warning      Rf_warning
//#define length      Rf_length // NEVER DO IT! THERE ARE SOME CONFLICTS

#define RNULL R_NilValue

// just one position SEXP data
#define SEXP_TO_STRING(value)  CHAR(STRING_ELT(value,0))
#define SEXP_TO_INTEGER(value) INTEGER(value)[0]
#define SEXP_TO_REAL(value)    REAL(value)[0]

#define GET_STRING_ELEMENT(data_, name_)  SEXP_TO_STRING (GetListElement(data_, name_))
#define GET_INTEGER_ELEMENT(data_, name_) SEXP_TO_INTEGER(GetListElement(data_, name_))
#define GET_REAL_ELEMENT(data_, name_)    SEXP_TO_REAL   (GetListElement(data_, name_))
#define GET_BOOL_ELEMENT(data_, name_)    SEXP_TO_INTEGER(GetListElement(data_, name_))

SEXP GetListElement(SEXP list, const char *str);

SEXP AsDataFrame(SEXP list, SEXP rownames);

SEXP CharToSEXP(const char* vchar);

SEXP getObjHandle(SEXP obj);
void* getObj(SEXP sxpHandle);

int GetPos(SEXP names, const char* value);
char* GetStringItemPos(SEXP value, int pos);

const char* GetStringFactorPos(SEXP factor, int pos);

#define dumpLimit 3

//  void printAttrib(SEXP e);
void printSEXP(SEXP e);

}

#endif

