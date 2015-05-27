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

#include "TeUtils.h"

namespace TeUtils
{

TeViewNode* GetThemeFromView(TeView* view, string theme_name)
{
    vector<TeViewNode*> themeVector = view->themes();

    for (unsigned i = 0; i < themeVector.size(); ++i)
        if (themeVector[i]->name() == theme_name)
            return themeVector[i];

    return NULL;
}

TeView* GetViewFromDatabase(TeDatabase* database, string view_name)
{
    TeViewMap& view_map = database -> viewMap();

    for (TeViewMap::iterator it = view_map.begin(); it != view_map.end(); ++it)
        if( it -> second -> name() == view_name )
            return it->second;

    return NULL;
}

TeTheme* GetThemeFromLayer(TeLayer* layer, string theme_name)
{
    return GetThemeFromDatabase(layer->database(), theme_name);
}

TeLayer* GetLayerFromDatabase(TeDatabase* database, string layer_name)
{
    TeLayerMap& layer_map = database -> layerMap();
    TeLayerMap::iterator it;

    for (it = layer_map.begin(); it != layer_map.end(); ++it)
        if(it-> second -> name() == layer_name)
            return it->second;

    return NULL;
}

TeTheme* GetThemeFromDatabase(TeDatabase* database, string theme_name)
{
    TeThemeMap& theme_map = database -> themeMap();
    TeThemeMap::iterator it;

    for (it = theme_map.begin(); it != theme_map.end(); ++it)
        if(it-> second -> name() == theme_name)
            return (TeTheme*) it->second;

    return NULL;
}

string AttrDataType(TeAttrDataType value)
{
    switch (value)
    {
        case TeREAL:      return "numeric";
        case TeINT:       return "integer";
        case TeDATETIME:  return "date";
        case TeBLOB:      return "blob";
        case TeOBJECT:    return "object";
        case TeSTRING:
        case TeCHARACTER: return "character";
        case TeUNKNOWN:
        default:          return "<unknown>";
    }
}

string AttrTableType(TeAttrTableType value)
{
    switch(value)
    {
        case TeAttrStatic:       return "static";
        case TeAttrMedia:        return "media";
        case TeAttrExternal:     return "external";
        case TeAttrEvent:        return "event";
        case TeFixedGeomDynAttr: return "dynattr";
        case TeDynGeomDynAttr:   return "alldyn";
        case TeGeomAttrLinkTime: return "dyngeom";
        case TeGeocodingData:
        default:                 return "<unrecognized type>";
    }
}


TeChronon GetTeChronon(string value)
{
    switch(value[0])
    {
        case 'y': return TeYEAR;
        case 's': return TeSECOND;
        case 'c': return TeSEASON;
        case 'f': return TeSECONDOFMINUTE;
        case 'v': return TeWEEKOFYEAR;
        case 'm':
            switch(value[2])
            {
                case 'i': return value.size() == 6? TeMINUTE : TeMINUTEOFHOUR;
                case 'o': return value.size() == 5? TeMONTH  : TeMONTHOFYEAR;
            }
        case 'h': return value.size() == 4? TeHOUR : TeHOUROFDAY;
        case 'd':
            if(value.size() == 3) return TeDAY;
            switch(value[5])
            {
                case 'w': return TeDAYOFWEEK;
                case 'm': return TeDAYOFMONTH;
                case 'y': return TeDAYOFYEAR;
            }
        case 'n': 
        default:  return TeNOCHRONON;
    }
}


TeStatisticType GetTeStatisticType(string value) 
{
    switch(value[0])
    {
        case 'a': return TeAMPLITUDE;
        case 'c': return TeCOUNT;
        case 'k': return value[1] == 'e'? TeKERNEL : TeKURTOSIS;
        case 's':
            switch(value[1])
            {
                case 'u': return TeSUM;
                case 't': return TeSTANDARDDEVIATION;
                case 'k': return TeSKEWNESS;
            }
        case 'm':
            switch(value[1])
            {
                case 'a': return TeMAXVALUE;
                case 'i': return TeMINVALUE;
                case 'e': return value[2] == 'a'? TeMEAN : TeMEDIAN;
                case 'o': return TeMODE;
            }
        case 'v':
            switch(value[4])
            {
                case 'd': return TeVALIDCOUNT;
                case 'o': return TeVARCOEFF;
                case 'a': return TeVARIANCE;
            }
        case 'n':
        default:  return TeNOSTATISTIC;
    }
}

int GetTeRelation(string relation)
{
    switch(relation[0])
    {
        case 'c':
            switch(relation[2])
            {
                case 'n': return TeCONTAINS;
                case 'o': return TeCROSSES;
                case 'v': return relation[5] == 's'? TeCOVERS: TeCOVEREDBY;
            }
            break;
        case 'o': return TeOVERLAPS;
        case 'd': return TeDISJOINT;
        case 't': return TeTOUCHES;
        case 'w': return TeWITHIN;
        case 'i': return TeINTERSECTS;
        case 'e': return TeEQUALS;
	}
	return TeUNDEFINEDREL;
}

int GetTeBoxPixel(string relation)
{
	switch(relation[0])
	{
		case 'o': return TeBoxPixelOut;        // disjoint
		case 'n': return TeBBoxPixelNotInters; // not intercepts
		case 'i': return TeBBoxPixelInters;    // intercepts
		case 'w': return TeBoxPixelIn;         // within
	}
	return 0;
}

}

