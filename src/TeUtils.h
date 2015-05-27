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

#ifndef ART_TEUTILS_H_
#define ART_TEUTILS_H_

#include <TeDatabase.h>
//#include <TeThemeApplication.h>

namespace TeUtils
{

	TeViewNode* GetThemeFromView    (TeView*,     string theme_name);
	TeView*     GetViewFromDatabase (TeDatabase*, string view_name);
	TeLayer*    GetLayerFromDatabase(TeDatabase*, string layer_name);
	TeTheme*    GetThemeFromDatabase(TeDatabase*, string theme_name);
	TeTheme*    GetThemeFromLayer   (TeLayer*,    string theme_name);
	
	TeChronon       GetTeChronon      (string value);
	TeStatisticType GetTeStatisticType(string value);
	int             GetTeRelation     (string value);

	string AttrDataType(TeAttrDataType value);
	string AttrTableType(TeAttrTableType value);
	
}

#endif

