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

/* aRTcomponent.h - Implementation of virtual classes aRTcomponent*,
 * which control the memory dependencies of the objects.
 *
 * Please check if the object IsValid() before using its functions.
 * (It is implemented in main.cpp)
 */

#ifndef ART_COMPONENT_H_
#define ART_COMPONENT_H_

#include <list>
#include <string>

#include <TeDatabase.h>
#include <TeProjection.h>
#include <TeTheme.h>
#include <TeGeometry.h>
#include "SEXPbasic.h"

using namespace std;

const char* TerraLibVersion();

extern "C" {

#define artOBJECT(class_) string("Object of class ") +  class_

// macros to add name into names, value (or call) into list and names into list,
// to be used in Summary.
#define STRING_LIST_ITEM(list, names, position, name, call) \
{                                                           \
    SET_STRING_ELT(names, position, mkChar(name));          \
    SEXP element;                                           \
    element = allocVector(STRSXP, 1);                       \
    SET_STRING_ELT(element, 0, mkChar(call.c_str()));       \
    SET_VECTOR_ELT(list, position, element);                \
}

#define DOUBLE_LIST_ITEM(list, names, position, name, call) \
{                                                           \
    SET_STRING_ELT(names, position, mkChar(name));          \
    SEXP element;                                           \
    element = allocVector(REALSXP, 1);                      \
    REAL(element)[0] = call;                                \
    SET_VECTOR_ELT(list, position, element);                \
}

#define INTEGER_LIST_ITEM(list, names, position, name, call) \
{                                                            \
    SET_STRING_ELT(names, position, mkChar(name));           \
    SEXP element;                                            \
    element = allocVector(INTSXP, 1);                        \
    INTEGER(element)[0] = call;                              \
    SET_VECTOR_ELT(list, position, element);                 \
}

#define SEXP_LIST_ITEM(list, names, position, name, call) \
{                                                         \
	SET_STRING_ELT(names, position, mkChar(name));        \
	SET_VECTOR_ELT(list, position, call);                 \
}

// use PrintSilent instead of Rprintf
#define PrintSilent if(!aRTcomponent::Silent) Rprintf
#define PrintSilentYes { PrintSilent("yes\n"); }
#define PrintSilentNo  { PrintSilent("no\n");  }

#define StreamToChar(x) (char*)(x.str().c_str())

class aRTcomponent
{
	public:
		virtual ~aRTcomponent() { }

		virtual void Clear() { };
		virtual SEXP Print()             = 0;
		virtual SEXP Summary()           = 0;
        virtual string Class()           = 0;
	    virtual SEXP List()              = 0; // List all components that can be opened

		virtual bool IsValid() { return true; }
		
		static bool Silent;
};

class aRTcomponentRoot;

class aRTcomponentLeaf : public aRTcomponent
{
	public:
		aRTcomponentLeaf();
		aRTcomponentRoot* GetParent() { return Parent; }
		void SetInvalid();
		virtual bool IsValid() { return Valid; }
		void Clear();
		aRTcomponentRoot* Parent;
	protected:
		bool Valid;
};

class aRTcomponentRoot : public aRTcomponent
{
	public:
		aRTcomponentRoot() : aRTcomponent() { }
		const int NumberOfChildren() { return Children.size(); }
		void RemoveChild(aRTcomponentLeaf*);
		void AddChild(aRTcomponentLeaf*);
		void Clear();
		void SetInvalid();

	protected:
		list<aRTcomponentLeaf*> Children;
};

}
#endif

