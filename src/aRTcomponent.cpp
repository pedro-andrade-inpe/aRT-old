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

#include <iostream>
#include <TeVersion.h>
//#include <TeUnixPlatformDefines.h>
#include "aRTcomponent.h"

using namespace std;

bool aRTcomponent::Silent = false;

#define S___(X) #X
#define aaa abc

#define S_(X) #X
#define STRINGIZE(X) S_(X) 

const char* TerraLibVersion()
{
	// atualizar na proxima versao para ver se eles corrigiram!
//	return TeDBVERSION.c_str();
	//cout << TePLATFORM << endl << TePLATFORMCODE_LINUX << endl;// << HAVE_CONFIG_H << endl;
	//cout << TE_USE_MYSQL << endl;
	//return TE_PACKAGE_VERSION; //TERRALIB_VERSION;
	return TERRALIB_VERSION;
}

aRTcomponentLeaf::aRTcomponentLeaf() : aRTcomponent()
{
	Parent = NULL;
	Valid  = true;
}

void aRTcomponentLeaf::Clear()
{
	if(Parent && Valid) Parent->RemoveChild(this); // Valid: 3 hours of headache
	SetInvalid();
}

void aRTcomponentRoot::RemoveChild(aRTcomponentLeaf* child)
{
    list<aRTcomponentLeaf*>::iterator it;

	for(it = Children.begin(); *it != child; it++);

	Children.erase(it);
}

void aRTcomponentRoot::AddChild(aRTcomponentLeaf* child)
{
    child->Parent = this;
    Children.push_back(child);
}

void aRTcomponentLeaf::SetInvalid()
{
    Valid = false;
}

void aRTcomponentRoot::Clear()
{
	list<aRTcomponentLeaf*>::iterator it = Children.begin();
	
	while(it != Children.end())
	{
		(*it)->SetInvalid();
		it++;
	}	
}

