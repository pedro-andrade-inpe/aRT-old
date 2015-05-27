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

#include "aRTdb.h" 
#include "aRTexternaltable.h"
#include <TeImportRaster.h>

extern "C"
{

aRTdb::aRTdb(TeDatabase* database, bool load)
{
	Database = database;

	if(!load) return; // TerraView integration. _database_ is already loaded.

//	Reload();
}

SEXP aRTdb::Reload(SEXP data)
{
	views = GET_STRING_ELEMENT(data, "user");

	if (views == "") views = Database -> user();

	PrintSilent("Loading layer set from database \'%s\' ... ", Database->databaseName().c_str());
	if( !Database -> loadLayerSet() )
	{
		delete this;
		PrintSilentNo;
		error("Could not load the layer set!\n");
	}
	PrintSilentYes;
	PrintSilent("Loading view set \'%s\' from database \'%s\' ... ", views.c_str(), Database->databaseName().c_str());
	if( !Database -> loadViewSet(views) )
	{
		delete this;
		PrintSilentNo;
		error("Could not load the view set!\n");
	}

	Database->loadProjectSet();

	PrintSilentYes;
	return RNULL;
}

aRTdb::~aRTdb()
{
	//FIXME
	PrintSilent("Removing aRTdb \'%s\' from memory ... ", Database->databaseName().c_str());
	//delete Database;
	Clear();
	PrintSilentYes;
}

SEXP aRTdb::ShowViews()
{
	SEXP vectorViews;

	TeViewMap& view_map = Database -> viewMap();
	TeView* view;
	TeViewMap::iterator it;
	
	vectorViews = view_map.size() > 0 ? allocVector(STRSXP, view_map.size()) : R_NilValue;
	
	int i = 0;
	for (it = view_map.begin(); it != view_map.end(); ++it)
	{
		view = it -> second;
		SET_STRING_ELT(vectorViews, i, mkChar(view -> name().c_str()));
		i++;
    }

	return vectorViews;
}

SEXP aRTdb::ShowThemes()
{
	SEXP vectorThemes;

	TeThemeMap& theme_map = Database -> themeMap();
    TeAbstractTheme* theme;
	TeThemeMap::iterator it;
	vectorThemes = theme_map.size() > 0 ? allocVector(STRSXP, theme_map.size()) : R_NilValue;
	int i = 0;
	for (it = theme_map.begin(); it != theme_map.end(); ++it)
	{
		theme = it -> second;
		SET_STRING_ELT(vectorThemes, i, mkChar(theme -> name().c_str()));
		i++;
	}

	return vectorThemes;
}

SEXP aRTdb::CreateView(string view_name, TeProjection* proj)
{	
	PrintSilent("Checking for view \'%s\' in database \'%s\' ... ",
				view_name.c_str(),
				Database->databaseName().c_str());
	
	if( TeUtils::GetViewFromDatabase(Database, view_name) )
	{
		PrintSilentYes;
		return RNULL;
	}	
	PrintSilentNo;

	PrintSilent("Creating view \'%s\' ... ", view_name.c_str());

	if(!proj) proj = new TeNoProjection();

	TeView* view = new TeView(view_name, views);
	view -> projection(proj);
	PrintSilentYes;
	PrintSilent("Inserting view \'%s\' in database \'%s\' ... ",
				view_name.c_str(),
				Database->databaseName().c_str());

	if ( !Database -> insertView(view) )
	{
		stringstream stream;
		stream << "The view \'" <<  view -> name()
			   << "\' could not be inserted into the database!\nError: "
			   << Database -> errorMessage() << "\n";
		error(StreamToChar(stream));
	}
	PrintSilentYes;
	return RNULL;
}

SEXP aRTdb::List()
{
	SEXP vectorLayers;

	TeLayerMap& layer_map = Database -> layerMap();
	TeLayer* layer;
	TeLayerMap::iterator it;
	
	vectorLayers = layer_map.size() > 0 ? allocVector(STRSXP, layer_map.size()) : R_NilValue;
	
	int i = 0;
	for (it = layer_map.begin(); it != layer_map.end(); ++it)
	{
		layer = it -> second;
		SET_STRING_ELT(vectorLayers, i, mkChar(layer -> name().c_str()));
		i++;
    }

	return vectorLayers;
}

SEXP aRTdb::DeleteLayer(SEXP data)
{
	string layer_name = GET_STRING_ELEMENT(data, "layername");

	PrintSilent("Checking for layer \'%s\' ... ", layer_name.c_str());
    TeLayer* te_layer = TeUtils::GetLayerFromDatabase(Database, layer_name);

    if (!te_layer)
	{
		PrintSilentNo;
        error("Layer does not exist\n");
	}
	PrintSilentYes;

	int layerid = te_layer->id();

	PrintSilent("Removing layer \'%s\' from database \'%s\' ... ",
				layer_name.c_str(),
				Database->databaseName().c_str());
	if( Database->deleteLayer(layerid) ) { PrintSilentYes; }
	else                                 { error("Layer could not be removed\n"); }

	return RNULL;
}

SEXP aRTdb::DeleteView(SEXP data)
{
	string view_name = GET_STRING_ELEMENT(data, "viewname");

	PrintSilent("Checking for view \'%s\' ... ", view_name.c_str());
    TeView* te_view = TeUtils::GetViewFromDatabase(Database, view_name);
	
    if (!te_view)
	{
		PrintSilentNo;
        warning("View does not exist\n");
		return RNULL;
	}
	
	PrintSilentYes;
	
    int viewid = te_view->id();

    if( Database->deleteView(viewid) ) { PrintSilentYes; }
	else                               { error("View could not be removed\n"); }

	return RNULL;
}

SEXP aRTdb::DeleteTheme(SEXP data)
{
	string theme_name = GET_STRING_ELEMENT(data, "themename");

	PrintSilent("Checking for theme \'%s\' ... ", theme_name.c_str());
	TeTheme* te_theme = TeUtils::GetThemeFromDatabase(Database, theme_name);

	if (!te_theme)
	{
		PrintSilentNo;
		warning("Theme does not exist\n");
		return RNULL;
	}
	PrintSilentYes;
	
	int themeid = te_theme->id();

	PrintSilent("Deleting theme \'%s\' ... ", theme_name.c_str());
	
	if( !Database->deleteTheme(themeid) )
	{
		PrintSilentNo;
		error("Theme could not be removed\n");
	}
	
	PrintSilentYes;
	return RNULL;
}

aRTcomponent* aRTdb::CreateLayer(SEXP data)
{
    TeLayer* layer;
	TeProjection* proj;

	string layer_name = GET_STRING_ELEMENT(data, "layername"); 
	string projection = GET_STRING_ELEMENT(data, "projection");
	
	PrintSilent("Checking for layer \'%s\' ... ", layer_name.c_str());
	layer = TeUtils::GetLayerFromDatabase(Database, layer_name);
	if(layer)
	{
		PrintSilentYes;
		error("Trying to create an existent layer.\n");
    }

	PrintSilentNo;
	PrintSilent("Building projection to layer \'%s\' ... ", layer_name.c_str());
	proj = TeGetTeProjectionFromSProj(projection);
	PrintSilentYes;

	PrintSilent("Creating layer \'%s\' ... ", layer_name.c_str());

	// also insert the layer in the Database list in memory
   	layer = new TeLayer(layer_name, Database, proj);

	if (layer -> id() <= 0)
	{
		string error_ = Database -> errorMessage();
		delete proj;
		delete layer;
		error(error_.c_str());
   	}

	aRTlayer* child = new aRTlayer(Database, layer_name);
	AddChild(child);
	PrintSilentYes;
	return child;
}

aRTcomponent* aRTdb::OpenLayer(SEXP data)
{
    TeLayer* layer;
	
	string layer_name = GET_STRING_ELEMENT(data, "layername"); 
	
	layer = TeUtils::GetLayerFromDatabase(Database, layer_name);
	if(layer)
	{
		PrintSilent("Opening layer \'%s\' ... ", layer_name.c_str());
	    aRTlayer* child = new aRTlayer(Database, layer_name);
	    AddChild(child);
		PrintSilentYes;
		return child;
    }

	error("Layer \'%s\' does not exist.", layer_name.c_str());
	return NULL;			 
}

aRTcomponent* aRTdb::CreateTable(SEXP options)
{
	string table_name = GET_STRING_ELEMENT  (options, "tablename");

	aRTtable* child = new aRTexternaltable(Database, "", table_name);
	PrintSilent("Checking for table \'%s\' ... ", table_name.c_str());
	if( Database->tableExist(table_name) )
	{
		PrintSilentYes;
		error("Trying to create an existent table\n");
	}

	PrintSilentNo;
	PrintSilent("Creating external table \'%s\' ... ", table_name.c_str());
	child->Create(options);
	PrintSilentYes;

	AddChild(child);
	return child;
}

aRTcomponent* aRTdb::OpenTable(SEXP options)
{
	string table_name = GET_STRING_ELEMENT  (options, "tablename");

	aRTtable* child = new aRTexternaltable(Database, "", table_name);
	PrintSilent("Checking for table \'%s\' ... ", table_name.c_str());
	if( Database->tableExist(table_name) )
	{
		PrintSilentYes;
		AddChild(child);
		return child;
	}

	PrintSilentNo;
	error("Table \'%s\' does not exist\n", table_name.c_str());
	return NULL;
}

// only open for themes. they have to be created from layers
aRTcomponent* aRTdb::OpenTheme(SEXP data)
{
	string theme_name = GET_STRING_ELEMENT(data, "themename"); 

	PrintSilent("Looking for theme \'%s\' ... ", theme_name.c_str());
	if( !TeUtils::GetThemeFromDatabase(Database, theme_name) )
		error("Theme \'%s\' does not exist. You can create it only from layers\n", theme_name.c_str());

	PrintSilentYes;
	PrintSilent("Opening theme \'%s\' ... ", theme_name.c_str());
	TeUtils::GetThemeFromDatabase(Database, theme_name)->loadThemeTables();
	PrintSilentYes;
	aRTtheme* child = new aRTtheme(Database, theme_name);
   	AddChild(child);
   	return child;
}

enum aRTSenum
{
    aRTdbSClass = 0,
    aRTdbSDatabase,
	aRTdbSLayers,
	aRTdbSViews,
	aRTdbSThemes,
    aRTdbSSum
};

SEXP aRTdb::Summary()
{
    SEXP info, names;

    info = allocVector(VECSXP, aRTdbSSum); // List containing the connection attributes
    names = allocVector(STRSXP, aRTdbSSum); // Names Vector

    STRING_LIST_ITEM(info, names, aRTdbSClass,     "class",    Class());
	STRING_LIST_ITEM(info, names, aRTdbSDatabase,  "database", Database->databaseName());

	SEXP_LIST_ITEM(info, names, aRTdbSThemes, "themes", ShowThemes());
    SEXP_LIST_ITEM(info, names, aRTdbSViews,  "views",  ShowViews());
    SEXP_LIST_ITEM(info, names, aRTdbSLayers, "layers", List());
	
    setAttrib(info, R_NamesSymbol, names); // Set the names of the list

    return info;
}

SEXP aRTdb::ShowTables()
{
	SEXP vectorTables;
	TeAttrTableVector atts;
	TeAttributeList attrList;
	
	Database -> getAttrTables(atts, TeAttrExternal);
	
	// TODO: it returns false when the database does not have any table
	//if ( !Database -> getAttrTables(atts, TeAttrExternal) )
	//	error("Error loading tables from database!");

	if( !atts.size() ) return R_NilValue;
	
	vectorTables = allocVector(STRSXP, atts.size());

	for (unsigned i = 0; i < atts.size(); i++)
		SET_STRING_ELT(vectorTables, i, mkChar(atts[i].name().c_str()));

	return vectorTables;
}

SEXP aRTdb::Print()
{
	stringstream s;

	if(!Database) error("Invalid object");

	TeAttrTableVector atts;
	Database->getAttrTables(atts, TeAttrExternal);

	s << artOBJECT("aRTdb") << "\n\n"
	  << "Database: \"" << Database->databaseName() << "\"" << endl
	  << "Layers: ";

    TeLayerMap& layer_map = Database -> layerMap();
    TeLayer* layer;
    TeLayerMap::iterator lit;

    if( layer_map.size() > 0 )
	{
		s << endl;
	    for (lit = layer_map.begin(); lit != layer_map.end(); ++lit)
	    {
	        layer = lit -> second;
	        s << "    \"" << layer -> name() << "\"" << endl;
	    }
	}
	else s << "(none)" << endl;
 
	s << "Themes: ";

	TeThemeMap& theme_map = Database -> themeMap();
	TeAbstractTheme* theme;
	TeThemeMap::iterator tit;
	if(theme_map.size() > 0)
	{
		s << endl;
		for (tit = theme_map.begin(); tit != theme_map.end(); ++tit)
		{
			theme = tit -> second;
			s << "    \"" << theme -> name() << "\"" << endl;
		}
	}
	else s << "(none)" << endl;

	s << "External tables: ";
    if( atts.size() )
	{
		s << endl;
		for(unsigned i = 0; i != atts.size(); i++)
			s << "    \"" << atts[i].name() << "\"" << endl;
	}
	else s << "(none)" << endl;
			
	s << endl;
	Rprintf( StreamToChar(s) );
	return RNULL;
}

aRTcomponent* aRTdb::ImportRaster(SEXP options)
{
	string filename  = GET_STRING_ELEMENT(options, "file");
	string layername = GET_STRING_ELEMENT(options, "lname");
	double dummy     = GET_REAL_ELEMENT  (options, "dummy");

	TeRaster raster(filename.c_str());
	bool ok;

	aRTlayer* child = new aRTlayer(Database, layername);
    AddChild(child);
	
	PrintSilent("Importing raster \'%s\' to layer \'%s\' ... ",
				filename.c_str(),
				layername.c_str());

	ok =  TeImportRaster (child->Layer(),
                          &raster,
       					  raster.params().ncols_,
   						  raster.params().nlines_,
                          TeRasterParams::TeNoCompression,
                          "", // object id
                          dummy,
                          true); // usedummy

	if(!ok)
	{
        PrintSilentNo;
        error("Could not import the raster");
	}

	PrintSilentYes;
    return child;
}


aRTcomponent* aRTdb::ImportMif(SEXP options)
{
	string filename  = GET_STRING_ELEMENT  (options, "file");
	string layername = GET_STRING_ELEMENT  (options, "lname");

	PrintSilent("Importing mid/mif \'%s\' to layer \'%s\' ... ",
				filename.c_str(),
				layername.c_str());

//	if( !TeImportMIF(filename, Database, layername) )
		error("File could not be imported\n");

	PrintSilentYes;
	
	aRTlayer* child = new aRTlayer(Database, layername);
    AddChild(child);
    return child;
}

}

