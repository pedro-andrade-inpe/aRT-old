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

#include "aRTlayer.h"

#include "aRTstatictable.h"
#include "aRTmediatable.h"
#include "aRTeventtable.h"
#include "aRTdynattrtable.h"
#include "aRTgeometry.h"
#include "aRTraster.h"

#include "SEXPbasic.h"
#include "SEXPutils.h"
#include "TeUtils.h"

#include <TeDatabaseUtils.h>
#include <TeProjection.h>
#include <TeQuerierParams.h>
#include <TeImportRaster.h>
#include <TeDriverSHPDBF.h>
#include <TeCellAlgorithms.h>
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>

#include <stdio.h>
#include <iostream>

using namespace std;

aRTlayer::aRTlayer(TeDatabase* database, string layername)
{
	Database = database;
	LayerName = layername;
}

aRTlayer::~aRTlayer()
{
//	PrintSilent("Deleting aRTlayer \"%s\" from memory\n", LayerName.c_str());
	Clear();
}

TeGeomRep aRTlayer::GeomRep()
{
	if( Layer()->hasGeometry(TePOLYGONS) ) return TePOLYGONS;
	if( Layer()->hasGeometry(TeLINES)    ) return TeLINES;
	if( Layer()->hasGeometry(TePOINTS)   ) return TePOINTS;
	if( Layer()->hasGeometry(TeCELLS)    ) return TeCELLS;
	if( Layer()->hasGeometry(TeRASTER) 	 ) return TeRASTER;

	error("Layer does not have a valid geometry\n");
	return TeGEOMETRYNONE;
}

SEXP aRTlayer::GetProj()
{
	string sproj = TeGetSProjFromTeProjection(Layer()->projection());

	SEXP resp;

	resp  = allocVector(STRSXP, 1);
	SET_STRING_ELT( resp, 0, mkChar(sproj.c_str()) );
	return resp;
}

SEXP aRTlayer::GetBox()
{
	TeRepresentation* rep;
	rep = Layer()->getRepresentation( GeomRep() );
	
	if(rep) return SEXPutils::GetBoxTeBox(rep->box_);
	return R_NilValue;
}

SEXP aRTlayer::GetNearestNeighbors(SEXP options, aRTlayer* alayer)
{
	string objId = GET_STRING_ELEMENT (options, aRTstrId);
	int numRes   = GET_INTEGER_ELEMENT(options, "quantity");
	SEXP result;	
	TeGeomRep visRep = GeomRep();
	TeGeomRep actRep = alayer->GeomRep();

	string actGeomTable;
	string visGeomTable;
	visGeomTable = Layer()->tableName(visRep);
	actGeomTable = alayer->Layer()->tableName(actRep);

	string visCollTable;
	TeKeys visIdsOut;

	PrintSilent("Under development");

//	TeGetWithinDistance(actGeomTable, actRep, [[coord]], visIdsOut, Database, 10000, actCollTable);
/*
	Database->nearestNeighbors(actGeomTable,
							   actRep,
							   objId,
							   visGeomTable,
							   visCollTable,
							   visRep,
							   visIdsOut,
							   numRes);
*/
	
    PROTECT(result = allocVector(STRSXP, visIdsOut.size()));

    for(unsigned i = 0; i < visIdsOut.size(); i++)
	{
        SET_STRING_ELT(result, i, mkChar(visIdsOut[i].c_str()));
	}

    UNPROTECT(1);
    return result;
}

SEXP aRTlayer::GetDistance(SEXP ids, aRTlayer* alayer)
{
	SEXP result;
	double distance;
	TeGeomRep visRep = GeomRep();
	TeGeomRep actRep = alayer->GeomRep();
	string actGeomTable;
	string visGeomTable;
	visGeomTable = Layer()->tableName(visRep);
	actGeomTable = alayer->Layer()->tableName(actRep);

	string objId1 = CHAR(STRING_ELT(ids,0));
	string objId2 = CHAR(STRING_ELT(ids,1));

	TeKeys keys;
	keys.push_back(objId1);
	keys.push_back(objId2);
	PrintSilent("Calculating distance between %s and %s ... ", objId1.c_str(), objId2.c_str());


//calculateDistance (const string &actGeomTable, TeGeomRep actRep, const string &objId1, const string &visGeomTable, TeGeomRep visRep, const string &objId2, double &distance)


	bool right;
	right = Database->calculateDistance(actGeomTable,
					                    actRep,
										keys,//objId1,
										//visGeomTable,
										//visRep,
										//objId2,
										distance);
   
	if(!right) error("Could not calculate the distance\n");
	PROTECT(result = allocVector(REALSXP, 1));
    REAL(result)[0] = distance;     	
	UNPROTECT(1);
	PrintSilentYes;
	return result;
}

SEXP aRTlayer::GetRelation(SEXP options, aRTlayer* alayer)
{
	string relation = GET_STRING_ELEMENT(options, "relation");
	SEXP ids        = GetListElement(options, aRTstrId);

	if(Database != alayer->Database)
		error("The layers must be in the same database\n");

	TeGeomRep visRep = GeomRep();
	TeGeomRep actRep = alayer->GeomRep();
	string actGeomTable = alayer->Layer()->tableName(actRep);
	string visGeomTable = Layer()->tableName(visRep);
	int relate = TeUtils::GetTeRelation(relation);
	TeKeys actIdsIn, visIdsOut;
	
	if(relate == TeUNDEFINEDREL)
		error("Relation %s does not exist\n", relation.c_str());

	if(ids == R_NilValue)
	{
    	string sqlstr = "select object_id from " + actGeomTable;
	    TeDatabasePortal* portal = alayer->Database->getPortal();
		portal->query(sqlstr);

	    while( portal->fetchRow() )                              
	    {                                                        
	        string id = portal->getData(0);                             
	        actIdsIn.push_back(id);
		}
	}	
	else
	{
		actIdsIn = SEXPutils::SEXPtoKeys(ids);
	}

	bool right;

	PrintSilent("Calculating a spatial relation on layer \'%s\' ... ", LayerName.c_str());
	right = Database->spatialRelation (actGeomTable, actRep, actIdsIn,
							           visGeomTable, visRep, visIdsOut, relate);

	PrintSilentYes;
	if(!right) return RNULL; // error("Could not execute the spatial relation\n");

    return SEXPutils::GetKeys(visIdsOut);
}

aRTcomponent* aRTlayer::GetSetOperation(SEXP options)
{
	string metric = GET_STRING_ELEMENT(options, "operation");
	SEXP ids      = GetListElement(options, aRTstrId);

    TeGeometryVect geomv;
	TePolygonSet ps;
	bool g = false;
	
	TeGeomRep actRep = GeomRep();
	string actTable = Layer()->tableName(actRep);
	TeKeys keys = SEXPutils::SEXPtoKeys(ids);
	
	switch(metric[0])
	{
		case 'u': g = Database->geomUnion       (actTable, actRep, keys,             geomv); break;
		case 'i': g = Database->geomIntersection(actTable, actRep, keys,             geomv); break;
		case 'd': g = Database->geomDifference  (actTable, actRep, keys[0], keys[1], geomv); break;
		case 'x': g = Database->geomXOr         (actTable, actRep, keys[0], keys[1], geomv); break;
		default:  error("Invalid set operation: %s\n", metric.c_str());
	}

	if(!g) error("Operation could not be done\n");

    TeGeometryVect::iterator it = geomv.begin();
    while ( it!= geomv.end() )
    {
        TeGeometry* geom = (*it);

		// "spatialQuery.ui.h" line 1550
		if (dynamic_cast<TePolygonSet*> (geom))
        {
			TePolygonSet polySet, *polSet;
            polSet = (TePolygonSet*) geom;
			TeMultiGeometry mg;
			mg.polygons_ = *polSet;
			aRTgeometry* result = new aRTgeometry(mg);
			return result; // TODO: check if it can have more than one result.
			               // Does the loop always execute once?? I think so.
		}
		else error("Type returned not implemented\n");

		it++;
	}

	return NULL;	
}

SEXP aRTlayer::GetID()
{
	TeDatabasePortal* portal;
    TeAttributeList attList;
    TeAttribute at;
    at.rep_.name_ = "tempCol";
    at.rep_.type_ = TeSTRING;
    at.rep_.numChar_ = 100;
    at.rep_.isAutoNumber_ = false;
    at.rep_.isPrimaryKey_ = false;
    attList.push_back(at);
	SEXP ids;
	int i = 0;

    if(Database->tableExist("tempTable"))
        if(Database->execute("DROP TABLE tempTable") == false)
            error("Fail to drop the temporary table");

    if(Database->createTable("tempTable", attList) == false)
        error("Fail to create the temporary table");

    string popule, geo;
    if(Layer()->hasGeometry(TePOLYGONS))
    {
        geo = Layer()->tableName(TePOLYGONS);
        popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
        if(Database->execute(popule) == false)
            error("Fail to drop the temporary table");
    }
    if(Layer()->hasGeometry(TePOINTS))
    {
        geo = Layer()->tableName(TePOINTS);
        popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
        if(Layer()->database()->execute(popule) == false)
            error("Fail to drop the temporary table");
    }
    if(Layer()->hasGeometry(TeLINES))
    {
        geo = Layer()->tableName(TeLINES);
        popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
        if(Layer()->database()->execute(popule) == false)
            error("Fail to drop the temporary table");
    }

	string query = "select DISTINCT object_id from tempTable";
	portal = Database->getPortal();
	portal->query(query);
	PROTECT(ids = allocVector(STRSXP, portal->numRows()));
    while( portal->fetchRow() )
    {
		string value = portal->getData(0);
        SET_STRING_ELT(ids, i, mkChar(value.c_str()));
		i++;
	}
	return ids;
}

SEXP aRTlayer::GetMetric(SEXP options)
{
	string metric = GET_STRING_ELEMENT(options, "metric");
	SEXP ids      = GetListElement    (options, aRTstrId);

	SEXP result, colnames, other;
	TeGeomRep actRep = GeomRep();
	string actGeomTable;
	TeKeys keys;
	TePolygonSet ps;
 	TePointSet cs;
	TeBox box;
	double value;
	string id;
	int i = 0;
	const bool using_id = (ids != R_NilValue);
	TeDatabasePortal* portal;

	actGeomTable = Layer()->tableName(actRep);
	
	if(!using_id)
	{
		string sqlstr = "select DISTINCT object_id from " + actGeomTable;
		portal = Database->getPortal();
		portal->query(sqlstr);
		PROTECT(ids = allocVector(STRSXP, portal->numRows()));
	    while( portal->fetchRow() )
	    {
			string value = portal->getData(0);
	        SET_STRING_ELT(ids, i, mkChar(value.c_str()));
			i++;
		}
	}
	int size = LENGTH(ids);

	PROTECT(result   = allocVector(VECSXP, 2));
	PROTECT(colnames = allocVector(STRSXP, 2));

#define METRIC_FUNCTION(type_, metric_call_, set_call_, attrib_name_) \
    PROTECT(other = allocVector(type_, size));                        \
    for (i = 0; i < size; i++)                                        \
    {                                                                 \
        keys.clear();                                                 \
        keys.push_back(CHAR(STRING_ELT(ids, i)));                     \
        Database->metric_call_;                                       \
        set_call_;                                                    \
    }                                                                 \
    SET_STRING_ELT(colnames, 0, mkChar(aRTstrId));                    \
    SET_STRING_ELT(colnames, 1, mkChar(attrib_name_));                \
                                                                      \
    setAttrib(result, R_NamesSymbol, colnames);                       \
                                                                      \
	SET_VECTOR_ELT(result, 0, ids);                                   \
    SET_VECTOR_ELT(result, 1, other);                                 \
    UNPROTECT(4 - using_id);                                          \
	PrintSilentYes;                                                   \
    return result;

	PrintSilent("Calculating metric on layer \'%s\' ... ",LayerName.c_str());
	switch(metric[0])
	{
		case 'a':
			METRIC_FUNCTION(REALSXP,
							calculateArea(actGeomTable, actRep, keys, value),
							REAL(other)[i] = value,
							"area")
		case 'l':
			METRIC_FUNCTION(REALSXP,
							calculateLength(actGeomTable, actRep, keys, value),
							REAL(other)[i] = value,
							"length")
/*		case 'd':
			id = CHAR(GET_STRING_ELT(GET_STRING_ELT(options, "origin",0)));
			METRIC_FUNCTION(REALSXP,
                            calculateDistance(actGeomTable, actRep, id, actGeomTable, actRep, keys, distance);
							REAL(other)[i] = distance;
							"distance")
			break;
		case 'w':
			break;*/
		default:
			error("Invalid metric: %s\n", metric.c_str());
	}
	
	return R_NilValue;
}

aRTcomponent* aRTlayer::GetOperation(SEXP options)
{
	string metric = GET_STRING_ELEMENT(options, "operation");
	SEXP ids      = GetListElement    (options, aRTstrId);

	TeGeomRep actRep = GeomRep();
	string actGeomTable;
	TeKeys keys;
	TePolygonSet ps;
	TeMultiGeometry mg;
 	TePointSet cs;
	double value;
	int unprotect_ = 0;
	string id;
	int i = 0;
	const bool using_id = (ids != R_NilValue);
	TeDatabasePortal* portal;
	aRTgeometry* result;

	actGeomTable = Layer()->tableName(actRep);
	
	if(!using_id)
	{
		string sqlstr = "select DISTINCT object_id from " + actGeomTable;
		portal = Database->getPortal();
		portal->query(sqlstr);
		PROTECT(ids = allocVector(STRSXP, portal->numRows()));
		unprotect_++;
	    while( portal->fetchRow() )
	    {
			string value = portal->getData(0);
	        SET_STRING_ELT(ids, i, mkChar(value.c_str()));
			i++;
		}
	}
	int size = LENGTH(ids);
	unsigned it = 0;

#define OPERATION_FUNCTION(metric_call_, set_)               \
    for (i = 0; i < size; i++)                               \
    {                                                        \
        keys.clear();                                        \
        keys.push_back(CHAR(STRING_ELT(ids, i)));            \
        Database->metric_call_;                              \
		for(it = 0; it < set_.size(); it++)                  \
			if(set_[it].objectId() == "")                    \
				set_[it].objectId(CHAR(STRING_ELT(ids, i))); \
    }                                                        \
	PrintSilentYes;                                          \
    break;
// TODO: the second for in the macro above is terrible. (but it works)
	
	PrintSilent("Calculating operation '%s' on layer \'%s\' ... ", metric.c_str(), LayerName.c_str());
	switch(metric[0])
	{
		case 'c':
			OPERATION_FUNCTION(centroid(actGeomTable, actRep, cs, keys), cs)
		case 'b':
			value = GET_REAL_ELEMENT(options, "dist");
			OPERATION_FUNCTION(buffer(actGeomTable, actRep, keys, ps, value), ps)
		case 'h':
			OPERATION_FUNCTION(convexHull(actGeomTable, actRep, keys, ps), ps)
		default:
			error("Invalid operation: %s\n", metric.c_str());
	}

	mg.points_ = cs;
	mg.polygons_ = ps;
	result = new aRTgeometry(mg);
	UNPROTECT(unprotect_);
	return result;
}

aRTcomponent* aRTlayer::GetRaster(SEXP options)
{
    if( !(Layer()->geomRep() & TeRASTER) ) error("Layer does not have raster data");

	return new aRTraster( Layer() );
}

SEXP aRTlayer::AddShape(SEXP options)
{
	string tablename = GET_STRING_ELEMENT  (options, "tablename");
	string filename  = GET_STRING_ELEMENT  (options, "filename");
	string id        = GET_STRING_ELEMENT  (options, aRTstrId);
	int length       = GET_INTEGER_ELEMENT (options, "length");

	PrintSilent("Importing shape \'%s\' to layer \'%s\' ... ",
				filename.c_str(),
				LayerName.c_str());
	
	if( !TeImportShape(Layer(), filename, tablename, id, length) )
		error("Shape could not be imported\n");
	PrintSilentYes;
	return RNULL;
}

SEXP aRTlayer::AddPoints(SEXP sps)
{
	aRTgeometry* points = (aRTgeometry*) getObj(sps);

	PrintSilent("Adding %d points to layer \'%s\' ... ", points->GetTePointSet().size(), LayerName.c_str());
	if( !Layer()->addPoints(points->GetTePointSet()) ) error("Points could not be added\n");
	PrintSilentYes;

	PrintSilent("Reloading tables of layer \'%s\' ... ", LayerName.c_str());
	Layer()->loadLayerTables();
	PrintSilentYes;
	return RNULL;
}

SEXP aRTlayer::AddPolygons(SEXP polset)
{
	aRTgeometry* polygons = (aRTgeometry*) getObj(polset);

	PrintSilent("Adding %d polygons to layer \'%s\' ... ", polygons->GetTePolygonSet().size(), LayerName.c_str());
    if( !Layer()->addPolygons(polygons->GetTePolygonSet()) ) error("Polygons could not be added\n");
	PrintSilentYes;

	PrintSilent("Reloading tables of layer \'%s\' ... ", LayerName.c_str());
	Layer()->loadLayerTables();
	PrintSilentYes;
	return RNULL;
}

SEXP aRTlayer::AddLines(SEXP lineset)
{
	aRTgeometry* lines = (aRTgeometry*) getObj(lineset);

	PrintSilent("Adding %d lines to layer \'%s\' ... ", lines->GetTeLineSet().size(), LayerName.c_str());
    if( !Layer()->addLines(lines->GetTeLineSet()) ) error("Lines could not be added\n");
	PrintSilentYes;

	PrintSilent("Reloading tables of layer \'%s\' ... ", LayerName.c_str());
	Layer()->loadLayerTables();
	PrintSilentYes;
	return RNULL;
}

SEXP aRTlayer::AddRaster(SEXP rasterdata)
{
	aRTraster* artraster = (aRTraster*) getObj(rasterdata);
	TeRaster* raster = artraster->GetTeRaster();

	PrintSilent("Adding raster data to layer \'%s\' ... ", LayerName.c_str());
	bool ok;
	ok =  TeImportRaster (Layer(),
                          raster,
       					  raster->params().ncols_,
   						  raster->params().nlines_,
                          TeRasterParams::TeNoCompression,
                          "", // object id
                          raster->params().dummy_[0],
                          true); // usedummy

	if(!ok)
	{
        PrintSilentNo;
        error("Could not import the raster");
	}

	PrintSilentYes;
	PrintSilent("Reloading tables of layer \'%s\' ... ", LayerName.c_str());
	Layer()->loadLayerTables();
	PrintSilentYes;
	return RNULL;
}

// this is the order to appear inside R. S is from Summary
enum aRTlayerSenum
{
	aRTlayerSClass = 0,
	aRTlayerSLayer,
	aRTlayerSDatabase,
	aRTlayerSPolygons,
	aRTlayerSLines,
	aRTlayerSPoints,
	aRTlayerSCells,
	aRTlayerSRaster,
	aRTlayerSProjection,
	aRTlayerSDatum,
	aRTlayerSLongitude,
	aRTlayerSLatitude,
	aRTlayerSSize
};
					   
SEXP aRTlayer::Summary()
{
    SEXP info, names;

    info    = allocVector(VECSXP, aRTlayerSSize);  // List containing the connection attributes
    names   = allocVector(STRSXP, aRTlayerSSize);  // Names Vector

    string raster = "no";
    if( Layer()->geomRep() & TeRASTER) raster = "yes";
		
    STRING_LIST_ITEM(info, names, aRTlayerSRaster,     "raster",     raster);
    STRING_LIST_ITEM(info, names, aRTlayerSClass,      "class",      Class());
    STRING_LIST_ITEM(info, names, aRTlayerSProjection, "projection", Layer()->projection()->name());
    STRING_LIST_ITEM(info, names, aRTlayerSDatabase,   "database",   Database->databaseName());
    STRING_LIST_ITEM(info, names, aRTlayerSLayer,      "layer",      LayerName);
    STRING_LIST_ITEM(info, names, aRTlayerSDatum,      "datum",      Layer()->projection()->datum().name());
		
    DOUBLE_LIST_ITEM(info, names, aRTlayerSLongitude,  "longitude",  Layer()->projection()->lon0());
    DOUBLE_LIST_ITEM(info, names, aRTlayerSLatitude,   "latitude",   Layer()->projection()->lat0());
    DOUBLE_LIST_ITEM(info, names, aRTlayerSPolygons,   "polygons",   Layer()->nGeometries(TePOLYGONS));
    DOUBLE_LIST_ITEM(info, names, aRTlayerSPoints,     "points",     Layer()->nGeometries(TePOINTS));
    DOUBLE_LIST_ITEM(info, names, aRTlayerSLines,      "lines",      Layer()->nGeometries(TeLINES));
    DOUBLE_LIST_ITEM(info, names, aRTlayerSCells,      "cells",      Layer()->nGeometries(TeCELLS));

    setAttrib(info, R_NamesSymbol, names); // Set the names of the list

    return info;
}

SEXP aRTlayer::Print()
{
	stringstream s;

	if (!IsValid()) error("Invalid object!");
		 
    s << artOBJECT("aRTlayer") << "\n\n"
	  << "Layer: \""           << LayerName                << "\"" << endl
	  << "Database: \""        << Database->databaseName() << "\"" << endl;

	bool emp = true; // empty
	int q; // quantity

	q = Layer()->nGeometries(TePOLYGONS); if(q) { emp = false; s << "Number of polygons: " << q << endl; }
	q = Layer()->nGeometries(TeLINES);    if(q) { emp = false; s << "Number of lines: "    << q << endl; }
	q = Layer()->nGeometries(TePOINTS);   if(q) { emp = false; s << "Number of points: "   << q << endl; }
	q = Layer()->nGeometries(TeCELLS);    if(q) { emp = false; s << "Number of cells: "    << q << endl; }

	if(Layer()->hasGeometry(TeRASTER)) { emp = false; s << "Layer has raster data" << endl; }

	if(emp) s << "Layer is empty" << endl;
	
	if(Layer() -> projection() == NULL)
    {
        s << "Layer has no projection!" << endl;
	   Rprintf( StreamToChar(s) );
       return RNULL;
	}

	s << "Projection Name: \""   << Layer()->projection()->name()         << "\""<< endl
	  << "Projection Datum: \""  << Layer()->projection()->datum().name() << "\""<< endl;
//	  << "Projection Longitude: "<< Layer()->projection()->lon0()                << endl
//	  << "Projection Latitude: " << Layer()->projection()->lat0()                << endl;

	TeAttrTableVector tvector = Layer()->attrTables();

	s << "Tables:";

	if(!tvector.size()) s << " (none) ";
	else for(unsigned i = 0; i < tvector.size(); i++)
	{
		s << "\n    \"" << tvector[i].name() << "\": ";
		s << TeUtils::AttrTableType(tvector[i].tableType());
	}
	s << endl;
	
	Rprintf( StreamToChar(s) );
	return RNULL;
}

SEXP aRTlayer::ShowTables()
{
	SEXP vectorTables;
	TeAttrTableVector atts;
	TeAttributeList attrList;
	
	if (!Layer() -> getAttrTables(atts)) return R_NilValue;;

	vectorTables = allocVector(STRSXP, atts.size());

	for (unsigned i = 0; i < atts.size(); i++)
		SET_STRING_ELT(vectorTables, i, mkChar(atts[i].name().c_str()));

	return vectorTables;
}

aRTcomponent* aRTlayer::CreateTable(SEXP options)
{
	string table_name = GET_STRING_ELEMENT  (options, "tablename");
	string type       = GET_STRING_ELEMENT  (options, "type");

	aRTtable* child = NULL;
	
    switch(type[0])
    {
        case 's': child = new aRTstatictable(Database, LayerName, table_name); break;
        case 'm': child = new aRTmediatable (Database, LayerName, table_name); break;
        case 'e': child = new aRTeventtable (Database, LayerName, table_name); break;
        case 'd':
			switch(type[3])
			{
				case 'a': child = new aRTdynattrtable(Database, LayerName, table_name); break;
				default: error("Type not recognized\n");
// TODO: implement dyngeom* tables
//        		case 'g': atttype = TeDynGeomDynAttr;              break;
			}
			break;
//        case 'a': atttype = TeGeomAttrLinkTime;            break;
        default:  error("Type not recognized\n");
    }

	if( !Database->tableExist(child->Name()) ) // the table name can change as in mediatable
	{
		child->Create(options);
	}
	else error("Trying to create an existent table \'%s\'\n", table_name.c_str());

	Parent->AddChild(child);
	return child;
}

aRTcomponent* aRTlayer::OpenTable(SEXP options)
{
	string table_name = GET_STRING_ELEMENT  (options, "tablename");
	string type       = GET_STRING_ELEMENT  (options, "type");

	aRTtable* child = NULL;
	
    switch(type[0])
    {
        case 's': child = new aRTstatictable(Database, LayerName, table_name); break;
        case 'm': child = new aRTmediatable (Database, LayerName, table_name); break;
        case 'e': child = new aRTeventtable (Database, LayerName, table_name); break;
        case 'd':
			switch(type[3])
			{
				case 'a': child = new aRTdynattrtable(Database, LayerName, table_name); break;
				default: error("Type not recognized\n");
// TODO: implement dyngeom* tables
//        		case 'g': atttype = TeDynGeomDynAttr;              break;
			}
			break;
//        case 'a': atttype = TeGeomAttrLinkTime;            break;
        default:  error("Type not recognized\n");
    }

	if( !Database->tableExist(child->Name()) ) // the table name can change as in mediatable
	{
		error("Table \'%s\' does not exist\n", table_name.c_str());
	}
	else
	{
		PrintSilent("Opening table \'%s\' from layer \'%s\' ... ",
					table_name.c_str(),
					LayerName.c_str());
		PrintSilentYes;
	}

	Parent->AddChild(child);
	return child;
}

// TODO: let the user choose the geometry!
aRTcomponent* aRTlayer::OpenQuerier(SEXP options)
{
	string geom = GET_STRING_ELEMENT(options, "geom");

	TeQuerierParams params(true, false); 
    params.setParams( Layer() );
	aRTquerier* child = new aRTquerier( params, GeomRep() );
	Parent->AddChild(child);
	return child;
}

aRTcomponent* aRTlayer::CreateTheme(SEXP options)
{
	string theme_name = GET_STRING_ELEMENT(options, "themename");
	string viewname   = GET_STRING_ELEMENT(options, "viewname" );
	SEXP tableNames   = GetListElement    (options, "tables"   );
	string attrest    = GET_STRING_ELEMENT(options, "attrest"  );

	PrintSilent("Checking for theme \'%s\' in layer \'%s\' ... ",
				theme_name.c_str(),
				Database->databaseName().c_str());

	if( TeUtils::GetThemeFromDatabase(Database, theme_name) )
	{
		PrintSilentYes;
		error("Trying to create an existent theme\n");
	}
	PrintSilentNo;

    // try to create a view. no problem if it already exists.
	((aRTdb*)Parent)->CreateView( viewname, Layer()->projection() );

	TeView* view = TeUtils::GetViewFromDatabase(Database, viewname);

	PrintSilent("Creating theme \'%s\' on layer \'%s\' ... ", theme_name.c_str(), LayerName.c_str());
    TeTheme* te_theme = new TeTheme(theme_name, Layer(), 0, view->id());
	te_theme -> visibleRep(Layer()->geomRep());
	te_theme -> visibility(true);
//	if(attrest.size() > 0)
	te_theme->attributeRest(attrest);
	PrintSilentYes;

	// selecting the tables
	PrintSilent("Checking tables of theme \'%s\' ... ", theme_name.c_str());
	vector<string> attrTableNames;
	TeTable aux;
	TeAttrTableVector atts;
	tableNames = AS_CHARACTER(tableNames);

    // check the tables
	for (int i = 0; i < Rf_length(tableNames); i++)
	{
	    string tableName = CHAR(STRING_ELT(tableNames, i));
	    if (!Layer() -> getAttrTablesByName(tableName, aux))
	    {
	        error("Table \"%s\" inexist", tableName.c_str());
	    }
	    else
	    {
	        TeTable table(tableName);
	        Database->loadTableInfo(table);
	        atts.push_back(table);
			te_theme->addThemeTable(table);
	    }
	}
	PrintSilentYes;

	// TODO: set the projection only if it is the first theme of the view. terraview does that
	// check if it really works
//	if( !view->size() ) 
	view -> projection(Layer()->projection());

	view -> add(te_theme);

	TeAppTheme* app_theme = new TeAppTheme(te_theme);
    app_theme->canvasLegUpperLeft(Layer()->box().center());

	if(!insertAppTheme(Database, app_theme))
	{
		stringstream s;
    	s << "Error building theme \'" << theme_name << "\': " << Database->errorMessage() << endl;
    	error( StreamToChar(s) );
	}

	if( Layer()->hasGeometry(TeRASTER) )
	{
//		te_theme->visibleRep(TeRASTER | 0x40000000);
			
		PrintSilent("Saving theme \'%s\' ... ", theme_name.c_str());
/*		if( !te_theme->save() )
		{
			stringstream s;
			s << "Fail to save theme \'" << theme_name << "\': " << Database->errorMessage() << endl;
	        error( StreamToChar(s) );
		}
*/
		PrintSilentYes;
	}
	else // not a raster theme
	{
		PrintSilent("Saving theme \'%s\' ... ", theme_name.c_str());

    	if( !te_theme -> save() )
    	{
    	    stringstream s;
    	    s << "Error saving theme \'" << theme_name << "\': " << Database->errorMessage() << endl;
    	    error( StreamToChar(s) );
    	}
		PrintSilentYes;

    	PrintSilent("Building collection of theme \'%s\' ... ", theme_name.c_str());
    	if( !te_theme -> buildCollection() )
    	{
    	    stringstream s;
    	    s << "Error building theme \'" << theme_name << "\': " << Database->errorMessage() << endl;
    	    error( StreamToChar(s) );
    	}
    	PrintSilentYes;

    	PrintSilent("Generating positions of theme \'%s\' ... ", theme_name.c_str());
		if( !te_theme->generateLabelPositions() )
    	{
    	    stringstream s;
    	    s << "Error generating positions of theme \'" << theme_name << "\': " << Database->errorMessage() << endl;
    	    error( StreamToChar(s) );
    	}
    	PrintSilentYes;
	}

    TeBox tbox = Database->getThemeBox(te_theme);
    te_theme->setThemeBox(tbox);
    Database->updateTheme(te_theme);

	aRTtheme* child = new aRTtheme(Database, theme_name);
	Parent->AddChild(child);
	return child;
}

aRTcomponent* aRTlayer::OpenTheme(SEXP options)
{
	string theme_name = GET_STRING_ELEMENT(options, "themename");

	PrintSilent("Checking for theme \'%s\' in layer \'%s\' ... ",
				theme_name.c_str(),
				Database->databaseName().c_str());

	if( TeUtils::GetThemeFromDatabase(Database, theme_name) )
	{
		PrintSilentYes;
		TeUtils::GetThemeFromDatabase(Database, theme_name)->loadThemeTables();
		aRTtheme* child = new aRTtheme(Database, theme_name);
		Parent->AddChild(child);
		return child;
	}

	PrintSilentNo;
	error("Theme \'%s\' does not exist! If you want to create set create=TRUE\n",
			  theme_name.c_str());
	return NULL;
}

aRTcomponent* aRTlayer::CreateLayerCells(SEXP options)
{
	double rx        = GET_REAL_ELEMENT  (options, "rx");
	double ry        = GET_REAL_ELEMENT  (options, "ry");
	string layername = GET_STRING_ELEMENT(options, "lname");
	bool allbox      = GET_BOOL_ELEMENT  (options, "allbox");

	TeLayer* resp;

	PrintSilent("Creating layer \'%s\' with cells ... ",layername.c_str());
	resp = TeCreateCells(layername, Layer(), rx, ry, Layer()->box(), !allbox);
	if(!resp)
	{
		PrintSilentNo;
		error("Could not create the cells.");
	}
	PrintSilentYes;
	
	aRTlayer* child = new aRTlayer(Database, layername);
    Parent->AddChild(child);
    return child;
}


bool aRTlayer::IsValid()
{
	if(Valid && !Layer())
	{
		Valid = false;
		error("Layer removed from the database!");
	}

	return Valid;
}

