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

#include "aRTtheme.h"
#include "aRTdb.h"
#include "SEXPutils.h"

#include <TeQuerierParams.h>
#include <TeSTElementSet.h>

#include <sstream>

extern "C"
{

aRTtheme::aRTtheme(TeDatabase* database, string name)
{
	Database  = database;
	ThemeName = name;
}

aRTtheme::~aRTtheme()
{
//	PrintSilent("Deleting aRTtheme \"%s\" from memory\n", ThemeName.c_str());
	Clear();
}

aRTcomponent* aRTtheme::OpenQuerier(SEXP options)
{
	string chronon = GET_STRING_ELEMENT(options, "chronon");
	string stat    = GET_STRING_ELEMENT(options, "stat");
	string table   = GET_STRING_ELEMENT(options, "table");
	string att     = GET_STRING_ELEMENT(options, "att");
	string geom    = GET_STRING_ELEMENT(options, "geom");
	bool loadatt   = GET_BOOL_ELEMENT  (options, "loadatt");

	TeGeomRep geomrep;
	if     (geom == "points")   geomrep = TePOINTS;
	else if(geom == "lines")    geomrep = TeLINES;
	else if(geom == "polygons") geomrep = TePOLYGONS;
	else if(geom == "cells")    geomrep = TeCELLS;
	else                        geomrep = TeGEOMETRYNONE;

	TeQuerierParams params(geomrep != TeGEOMETRYNONE, loadatt); // it can select attributes here too, but we will
	                                        // let the user do it when he is creating the theme
	vector<string> vec;

	TeAttributeList attlist = Theme()->sqlAttList ();

	for(int i = 0; i < attlist.size(); i++)
		vec.push_back(attlist[i].rep_.name_);

	params.setLoadAttrs (vec);

    TeChronon techronon = TeUtils::GetTeChronon(chronon);
//    TeStatisticType testat = GetTeStatisticType(stat);
	
//	if(table == "")
		params.setParams(Theme(), techronon);
//	else
//		params.setParams(Theme(), techronon, table, att, testat);
	
    aRTquerier* p = new aRTquerier(params, geomrep);
	return p;
}

enum aRTthemeSenum
{
    aRTthemeSClass = 0,
    aRTthemeSName,
	aRTthemeSLayer,
	aRTthemeSView,
	aRTthemeSPoints,
	aRTthemeSPolygons,
	aRTthemeSLines,
	aRTthemeSCells,
    aRTthemeSTables,
    aRTthemeSAR,
//    aRTthemeSTR,
//    aRTthemeSSR,
    aRTthemeSSum
};

TeGeomRep aRTtheme::GeomRep()
{
	if(Theme()->layer()->nGeometries(TePOINTS))   { return TePOINTS;   }
    if(Theme()->layer()->nGeometries(TeLINES))    { return TeLINES;    }
    if(Theme()->layer()->nGeometries(TePOLYGONS)) { return TePOLYGONS; }
    if(Theme()->layer()->nGeometries(TeCELLS))    { return TeCELLS;    }

    error("Theme does not have a valid geometry\n");
	return TeGEOMETRYNONE;
}

SEXP aRTtheme::Summary()
{
	SEXP info, names;

    info = allocVector(VECSXP, aRTthemeSSum); // List containing the connection attributes
    names = allocVector(STRSXP, aRTthemeSSum); // Names Vector

	string layername = Theme()->layer()? Theme()->layer()->name(): "NOT";
	
	STRING_LIST_ITEM(info, names, aRTthemeSClass, "class",          Class());
    STRING_LIST_ITEM(info, names, aRTthemeSName,  "theme",          ThemeName);
	STRING_LIST_ITEM(info, names, aRTthemeSLayer, "layer",          layername);
	STRING_LIST_ITEM(info, names, aRTthemeSView,  "view",           Theme()->layer()->database()->viewMap()[Theme()->view()]->name());
	STRING_LIST_ITEM(info, names, aRTthemeSAR,    "attribute_rest", Theme()->attributeRest());
  //  STRING_LIST_ITEM(info, names, aRTthemeSTR,    "temporal_rest",  Theme()->temporalRest());
  //  STRING_LIST_ITEM(info, names, aRTthemeSSR,    "spatial_rest",   Theme()->spatialRest());

    SET_STRING_ELT(names, aRTthemeSTables,  mkChar("tables"));

	string y = "yes";
	string n = "no";
	
	STRING_LIST_ITEM(info, names, aRTthemeSPoints,   "points",   (Theme()->layer()->nGeometries(TePOINTS)  ? y:n) );
	STRING_LIST_ITEM(info, names, aRTthemeSLines,    "lines",    (Theme()->layer()->nGeometries(TeLINES)   ? y:n) );
	STRING_LIST_ITEM(info, names, aRTthemeSPolygons, "polygons", (Theme()->layer()->nGeometries(TePOLYGONS)? y:n) );
	STRING_LIST_ITEM(info, names, aRTthemeSCells,    "cells",    (Theme()->layer()->nGeometries(TeCELLS)   ? y:n) );

    TeAttrTableVector attrs;
    TeAttributeList attrlist;
    SEXP s_attrs, s_names, s_attr_list;

	Theme()->getAttTables(attrs);

    s_attrs = allocVector(VECSXP, attrs.size());
    s_names = allocVector(STRSXP, attrs.size());

    for(unsigned i = 0; i < attrs.size(); i++)
	{
        SET_STRING_ELT(s_names, i, mkChar(attrs[i].name().c_str()));
        attrlist = attrs[i].attributeList();
        s_attr_list = allocVector(STRSXP, attrlist.size());

        for(unsigned j = 0; j < attrlist.size(); j++)
        {
            SET_STRING_ELT(s_attr_list, j, mkChar(attrlist[j].rep_.name_.c_str()));
        }
        SET_VECTOR_ELT(s_attrs, i, s_attr_list);
    }
    setAttrib(s_attrs, R_NamesSymbol, s_names);

    SET_VECTOR_ELT(info, aRTthemeSTables, s_attrs);
    setAttrib(info, R_NamesSymbol, names); // Set the names of the list

    return info;
}

SEXP aRTtheme::Print()
{
    stringstream s;

    TeAttrTableVector attrs;
    TeAttributeList  attrlist;
    Theme()->getAttTables (attrs);

	s <<  artOBJECT("aRTtheme") << "\n\n";

	s << "Theme: \""     << ThemeName;
	if(Theme()->layer())
	{
		s << "\"\nLayer: \"" << Theme()->layer()->name()
		  << "\"\nView: \""  <<	Theme()->layer()->database()->viewMap()[Theme()->view()]->name() << "\"" << endl;
	}

	int q;
	q = Theme()->layer()->nGeometries(TePOLYGONS); if(q) { s << "Number of polygons: " << q << endl; }
	q = Theme()->layer()->nGeometries(TeLINES);    if(q) { s << "Number of lines: "    << q << endl; }
	q = Theme()->layer()->nGeometries(TePOINTS);   if(q) { s << "Number of points: "   << q << endl; }
	q = Theme()->layer()->nGeometries(TeCELLS);    if(q) { s << "Number of cells: "    << q << endl; }


	if(attrs.size() == 1)
	{
		s  << "Table: \"" <<  attrs[0].name() <<  "\"\nAttributes: ";

    	attrlist = attrs[0].attributeList();
	
		int length = 0;
    	for(unsigned j = 0; j < attrlist.size(); j++)
		{
            if(length > 39)
			{
				s << "\n            ";
				length = 0;
			}
			s <<  "\"" << attrlist[j].rep_.name_ << "\"";
			length += 4 + attrlist[j].rep_.name_.size();
			if(j + 1 < attrlist.size()) s << ", ";
        }
        s << "\n";
	}
	else
	{
		s  << "Tables: " << attrs.size();

	    for(unsigned i = 0; i < attrs.size(); i++)
		{
			s <<  "\n         \"" <<  attrs[i].name() <<  "\"\n         Attributes: ";
	    	attrlist = attrs[i].attributeList();
	
			int length = 0;
	    	for(unsigned j = 0; j < attrlist.size(); j++)
			{
	            if(length > 30)
				{
					s << "\n                     ";
					length = 0;
				}
				s <<  "\"" << attrlist[j].rep_.name_ << "\"";
				length += 4 + attrlist[j].rep_.name_.size();
				if(j + 1 < attrlist.size()) s << ", ";
	        }
	        s << "\n";
	    }
	}

	if(Theme()->attributeRest() != "")
	{
		s << "\nAttribute Restriction: \""  << Theme()->attributeRest()
	  	  << "\"\n";
	}
//	  << "\"\nTemporal Restriction: \"" << Theme()->temporalRest()
//	  << "\"\nSpatial Restriction: \""  << Theme()->spatialRest()

    Rprintf( StreamToChar(s) );
	return RNULL;
}

SEXP aRTtheme::GetBox()        { return SEXPutils::GetBoxTeBox( Theme()->boxRestriction() ); }

SEXP aRTtheme::GetRaster()
{ 
//	if( !(Theme()->layer()->geomRep() & TeRASTER) ) error("Theme does not have raster");
//	TeRaster* raster = Theme()da ->layer()->raster();

//	TeRDecoder* decoder = new TeRDecoder( raster->decoder() );
//	return decoder->data(); // R_NilValue;
	return R_NilValue;
}

SEXP aRTtheme::GetVisual()
{
	PrintSilent("Not implemented yet.");
	//cout << "getVisual" << endl;
	return R_NilValue;//Theme()->get
}

SEXP aRTtheme::SetVisible(SEXP options)
{
//	bool points    = GET_BOOL_ELEMENT(options, "points");
//	bool lines     = GET_BOOL_ELEMENT(options, "lines");
//	bool polygons  = GET_BOOL_ELEMENT(options, "polyons");
//	bool check     = GET_BOOL_ELEMENT(options, "check");

//	bool vis = SEXP_TO_INTEGER(visible);
//	Theme()->visibility(check);
	
//	Database->updateTheme(Theme());
	return RNULL;
}

SEXP aRTtheme::SetVisual(SEXP options)
{
	SEXP svisual = GetListElement    (options, "visual");
	SEXP satt    = GetListElement    (options, "attribute");
	string mode  = GET_STRING_ELEMENT(options, "mode");
	
	TeGeomRep geomrep = TePOINTS;

	     if( Theme()->layer()->hasGeometry(TePOLYGONS) ) geomrep = TePOLYGONS;
	else if( Theme()->layer()->hasGeometry(TeLINES) )    geomrep = TeLINES;
	else if( Theme()->layer()->hasGeometry(TePOINTS) )   geomrep = TePOINTS;
	else if( Theme()->layer()->hasGeometry(TeRASTER) )   geomrep = TePOLYGONS;
	else if( Theme()->layer()->hasGeometry(TeCELLS) )    geomrep = TePOLYGONS;

	SEXP scolor  = GetListElement(svisual, "color");
	vector<TeColor> colors = SEXPtoColors(scolor);
	TeVisual *visual = GetTeVisual(svisual, geomrep);

	if(colors.size() == 1) // no slicing
	{
	    Theme()->setVisualDefault (visual, geomrep);
		Database->updateTheme( Theme() );
		return RNULL;
	}		

	Theme()->resetGrouping(); // clear any grouping previously created

    TeGrouping grouping;// = new TeGrouping; // defines a particular way of grouping objects
	if(satt != R_NilValue)
	{	
    	TeAttributeRep rep;
    	rep.name_ = CHAR(STRING_ELT(satt, 0));
    	grouping.groupAttribute_ = rep;
	}

	grouping.groupPrecision_ = 4; // TODO: always 4?

	switch(mode[0])
	{
		case 'e': grouping.groupMode_ = TeEqualSteps;    break;
		case 'q': grouping.groupMode_ = TeQuantil;       break;
		case 's': grouping.groupMode_ = TeStdDeviation;  break;
		case 'u': grouping.groupMode_ = TeUniqueValue;   break;
		case 'r': grouping.groupMode_ = TeRasterSlicing; break;
		default:
			error("Invalid grouping mode\n");
	}
	
    grouping.groupNumSlices_ = colors.size();

	if (!Theme()->buildGrouping (grouping)) // generate groups based on grouping definition
    {
        stringstream str;
		str << "Error generating groups: " << Database->errorMessage() << endl;
        Database->close();
		error(str.str().c_str());
    }

	for(unsigned i = 0; i != colors.size(); i++)
	{
		visual->color(colors[i]);
		Theme()->setGroupingVisual(i+1, visual, geomrep);
	}

	// SAME AS LegendWindow::updateDatabase()
    if (!Theme()->saveGrouping() )
    {
		stringstream str;
        str << "Error materializing grouping: " << Database->errorMessage() << endl;
		error(str.str().c_str());
    }

    // Save legend in the collection table
    if(Theme()->grouping().groupMode_ != TeRasterSlicing)
    {
        if(!Theme()->saveLegendInCollection()) return false;
    }

	return RNULL;
}

}

