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

#include "SEXPbasic.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>

#include "aRTcomponent.h"

using namespace std;

extern "C"{

SEXP GetListElement(SEXP list, const char *str) {
  SEXP names;
  SEXP elmt = (SEXP) NULL;

  PROTECT(names = getAttrib(list, R_NamesSymbol));
  for (int i = 0; i <= Rf_length(names)-1; i++) {
    const char* tempStr = CHAR(STRING_ELT(names, i));
    if (strcmp(tempStr, str) == 0) {
      elmt = VECTOR_ELT(list, i);
      break;
    }
  } 
  UNPROTECT(1);
  return elmt;
}

SEXP AsDataFrame(SEXP list, SEXP rownames)
{
	SEXP classname = allocVector(STRSXP, 1);
	SET_STRING_ELT( classname, 0, mkChar("data.frame") );
	
	SET_CLASS(list, classname);
	setAttrib(list, R_RowNamesSymbol, rownames);

	return list;
}

SEXP Resize(SEXP value, int newlength)
{
	value = Rf_lengthgets(value, newlength);
	UNPROTECT(1);
	PROTECT(value);
	return value;
}

SEXP CharToSEXP(const char* vchar)
{
	SEXP value = allocVector(STRSXP, 1);         
    SET_STRING_ELT(value, 0, mkChar(vchar));
	return value;
}

SEXP getObjHandle(SEXP obj)
{
    SEXP sxpHandle = getAttrib( obj, mkString("pointer") );
    if( isNull(sxpHandle) ) error("Object is NULL");
    return sxpHandle;
}

void* getObj(SEXP sxpHandle)
{
    void* pointer = R_ExternalPtrAddr(sxpHandle);
    if(!pointer) error("External pointer is NULL");
    return pointer;
}

int GetPos(SEXP names, const char* value_)
{
	string value = value_;
	for(int i = 0; i < LENGTH(names);i++)
	{
		if(value == CHAR(STRING_ELT(names, i)))
			return i;
	}
	return -1;
}

char* GetStringItemPos(SEXP value, int pos)
{
	stringstream s;
	switch( TYPEOF(value) )
   	{
		case REALSXP: s << REAL(value)[pos];            break;
       	case INTSXP:  s << INTEGER(value)[pos];         break;
       	case STRSXP:  s << CHAR(STRING_ELT(value,pos)); break;
        default: PrintSilent("IDs type unrecognized");
	}
	return (char*)(s.str().c_str());//StreamToChar(s);//string str = s.str();
//	return (str.c_str();
}

const char* GetStringFactorPos(SEXP factor, int pos) 
{                                                   
	int factorvalue = INTEGER(factor)[pos] - 1; // reference here starts with 1. 
	SEXP attrib = getAttrib(factor,R_LevelsSymbol); 
    return CHAR(STRING_ELT(attrib,factorvalue));
}

/*
void printAttrib(SEXP e)
{
	SEXP rn=Rf_GetRowNames(e);
	if(rn != R_NilValue)
	{
		PrintSilent("Row names:\n");
		printSEXP(rn);
	}

	SEXP cn=getAttrib(e, R_NamesSymbol);
	{
		PrintSilent("Col names:\n");
		printSEXP(cn);
	}
}*/

void printSEXP(SEXP e)
{
    int i = 0;

    switch( TYPEOF(e) )
    {
        case NILSXP:
            PrintSilent("NULL value\n");
            return;

        case LANGSXP:
            PrintSilent("language construct\n");
            return;

        case REALSXP:
            if (LENGTH(e) > 1)
            {
                PrintSilent("Vector of real variables: ");
                while( i < LENGTH(e) )
                {
                    PrintSilent("%f",REAL(e)[i]);
                    if ( !(REAL(e)[i] < 0) && !(REAL(e)[i] >= 0) ) PrintSilent("__NAN__");
                    if (i < LENGTH(e) - 1) PrintSilent(", ");
                    if (dumpLimit && i > dumpLimit) { PrintSilent("..."); break; }
                    i++;
                }
                PrintSilent("\n");
            }
            else PrintSilent( "Real variable %f\n", *REAL(e) );
            return;
        case EXPRSXP:
            PrintSilent( "Vector of %d expressions:\n", LENGTH(e) );
            while( i < LENGTH(e) )
            {
                if (dumpLimit && i > dumpLimit){ PrintSilent("...");  break; }
                printSEXP( VECTOR_ELT(e,i) );
                i++;
            }
            return;

         case INTSXP:
            PrintSilent( "Vector of %d integers:\n", LENGTH(e) );

            while( i < LENGTH(e) )
            {
                if (dumpLimit && i > dumpLimit) { PrintSilent("..."); break; }
                PrintSilent("%d",INTEGER(e)[i]);
                if (i < LENGTH(e) - 1) PrintSilent(", ");
                i++;
            };
            PrintSilent("\n");
            return;

        case VECSXP:
            PrintSilent( "Vector of %d fields:\n", LENGTH(e) );
            while( i < LENGTH(e) )
            {
                if (dumpLimit && i>dumpLimit) { PrintSilent("..."); break; };
                printSEXP(VECTOR_ELT(e,i));
                i++;
            }
            return;

        case STRSXP:
            PrintSilent( "String vector of length %d:\n", LENGTH(e) );
            while( i < LENGTH(e) )
            {
                if (dumpLimit && i > dumpLimit) { PrintSilent("..."); break; };
                printSEXP(VECTOR_ELT(e,i)); i++;
            }
            return;
	   case CHARSXP:
            PrintSilent( "scalar string: \"%s\"\n",(char*) STRING_PTR(e) );
            return;

        case SYMSXP:
            PrintSilent( "Symbol, name: "); printSEXP(PRINTNAME(e) );
            return;

        default:
            PrintSilent( "Unknown type: %d\n", TYPEOF(e) );
    }
};

}

