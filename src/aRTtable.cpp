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

#include <TeSTEFunctionsDB.h>

#include "aRTtable.h"
#include "aRTraster.h"
#include "SEXPutils.h"
#include "TeUtils.h"

#include <TeGeometryAlgorithms.h>
#include <TeOverlay.h>
#include <TeCellAlgorithms.h>

using namespace std;
using namespace TeOVERLAY;


bool TeFillCellCategoryCountPercentageRasterOperation2(TeDatabase* db,
													  string raster_layername,
													  string cell_layername,
													  const string& cell_tablename,
													  const string attrName,
													  TeTimeInterval t)
{

	map<string, string> classesMap;
	vector< string > atts;
	string value;

	TeLayer* input_layer = new TeLayer (raster_layername);
	if (!db->loadLayer (input_layer))
	{
		cout << "could not load" << endl;
		 db->close();
		 return false;
	}

		cout << "loaded" << endl;
	atts.push_back(attrName);

	vector<double> values;
	bool found;
	unsigned int i;
	TeRaster* raster = input_layer->raster();
	TeRaster::iterator rasterItBegin = raster->begin();
	TeRaster::iterator rasterItEnd   = raster->end();

	TeTable table(cell_tablename);
	db->loadTableInfo(table);

	TeLayer* cell_layer = new TeLayer (cell_layername);
	if (!db->loadLayer (cell_layer))
	{
		cout << "could not load" << endl;
		 db->close();
		 return false;
	}
		cout << "loaded" << endl;

	// Initialize cell set
	TeCellSet cells;
	if (!TeFillCellInitLoad (cell_layer, cell_tablename, cells)) return false;
	cout << "initload"<< endl;
	// Initialize object set to store cell properties
	TeSTElementSet cellObjSet (cell_layer);

	// looking for the unique values in all the raster

	int count = 0;
	while(rasterItBegin != rasterItEnd)
	{
		found = false;
		for(i = 0; i < values.size(); ++i)
			if(values[i] == (*rasterItBegin)[0])
				found = true;
		if(!found) 
			values.push_back((*rasterItBegin)[0]);
		++rasterItBegin;

	}
	cout << "size: " << values.size() << endl;

	// Process
	TePropertyVector result;
	map<double, int> counter;

	// set the propertyvector of the cell object set
	for(i = 0; i < values.size(); i++)
	{
		stringstream str;

		str << values[i];
        string value_ =  str.str();
        for (unsigned j = 0; j < value_.size(); j++)
            if(value_[j] == '.' || value_[j] == '+')
                value_[j] = '_';

        TeProperty prop;
        prop.attr_.rep_.name_ = attrName + value_;
        prop.attr_.rep_.type_ = TeREAL;
        prop.attr_.rep_.numChar_ = 48;
		prop.attr_.rep_.decimals_ = 10;

        cellObjSet.addProperty(prop.attr_);
	}

	cout << "here" << endl;
	TeCellSet::iterator cell_it = cells.begin();
	vector<string> vprop;

	while (cell_it != cells.end())
	{
		double total = 0.0;
		TePolygon p = polygonFromBox((*cell_it).box());
		TeRaster::iteratorPoly rasterItBegin(raster);
		rasterItBegin = raster->begin(p, TeBBoxPixelInters, 0);
		TeRaster::iteratorPoly rasterItEnd(raster);
		rasterItEnd = raster->end(p, TeBBoxPixelInters, 0);

		for(i = 0; i < values.size(); i++)
			counter[values[i]] = 0;

		while(rasterItBegin != rasterItEnd)
		{
			double pixel = (*rasterItBegin);
			counter[pixel]++;
			++rasterItBegin;
			total++;
		}		

		//add the attribute values in the object set
		vprop.clear();
		for(i = 0; i < values.size(); i++)
		{
			stringstream str;
			if (total > 0)
				str << counter[values[i]] / total;
			else
				str << "0.0";

			vprop.push_back(str.str());
		}

		TeFillCellInitSTO ((*cell_it), vprop, cellObjSet, table.tableType(), t);
		cell_it++;

	} 

	// Update DB
	if (!TeUpdateDBFromSet (&cellObjSet, cell_tablename))
		return  false;
	return  true;
}




extern "C"
{

// TEMPORARY
class TeAverageWeightByRestrictedAreaStrategy : public TeComputeSpatialStrategy
{
	public:
		TeAverageWeightByRestrictedAreaStrategy(TeTheme*, TeRaster*, string& attrName, vector<int> validRasterValues, int band);
		string compute(TeBox);

	private:
		TeRaster* raster_;
		string attrName_;
		map<int, int> classesSelection_; 
		int band_;
		int validRasterValue_;
		int count;
};

TeAverageWeightByRestrictedAreaStrategy::TeAverageWeightByRestrictedAreaStrategy(TeTheme* theme, TeRaster* raster, string& attrName, vector<int> validRasterValues, int band) : TeComputeSpatialStrategy(theme, TePOLYGONS)
{
	count = 0;
	raster_ = raster;
	attrName_ = attrName;
	band_ = band;

	for(unsigned i = 0; i < validRasterValues.size(); i++)
		classesSelection_[validRasterValues[i]] = 1;
}

string TeAverageWeightByRestrictedAreaStrategy::compute (TeBox box)
{
	if( ! (count%50))
		PrintSilent("%d\n", count);
	count++;

	double weigh_val = 0.0;

	if (!raster_) return Te2String (weigh_val);

	const double raster_elem_area = raster_->params().resx_*raster_->params().resy_;

	TeSTElementSet stes(theme);

	vector<string> attrNames;
	attrNames.push_back (attrName_);      
	if (!TeSTOSetBuildDB (&stes, true, false, attrNames)) return Te2String(weigh_val);

	// convert input box to a polygonset
	TePolygonSet box_ps;
	TePolygon pol = polygonFromBox(box);
	box_ps.add(pol);

	double areaBox = TeGeometryArea(box);
	if (! (areaBox > 0.0)) return Te2String (weigh_val);

	TeSTElementSet::iterator it = stes.begin();
	while (it != stes.end())
 	{
		string val;
		if ((*it).getPropertyValue (attrName_, val))
		{
			double num_val = atof (val.c_str());
			TePolygonSet objGeom;
			if ((*it).getGeometry(objGeom))
			{
				double geomArea = TeGeometryArea (objGeom);
				if (geomArea > 0.0)
				{
					TePolygonSet intersect;

					if(TeOverlay(box_ps, objGeom, intersect, TeINTERSECTION))
					{
						double raster_intersect_area = 0.0;
						double intersec_area = TeGeometryArea (intersect);
	
						TePolygonSet::iterator psIt = intersect.begin();
						while (psIt != intersect.end())
						{
							TeRaster::iteratorPoly raster_it(raster_);
							TeRaster::iteratorPoly raster_it_end(raster_);
							raster_it = raster_->begin ((*psIt), TeBoxPixelIn);
							raster_it_end = raster_->end ((*psIt), TeBoxPixelIn);
							while (raster_it != raster_it_end)
							{
								int val = (int)(raster_it.operator*(band_));
								if (classesSelection_[val] == 1)
									raster_intersect_area +=  raster_elem_area;
								++raster_it;
							}
							++psIt;
						}
						weigh_val += num_val*(raster_intersect_area/areaBox); // avarege considering cell area
					}
				}
			}
		}
		++it;
	}
	return Te2String (weigh_val);
}
// end -- TEMPORARY


aRTtable::aRTtable(TeDatabase* database, string layername, string tablename)
{
	TableName = tablename;
	LayerName = layername;
	Database  = database;
}

aRTtable::~aRTtable()
{
	Clear();
}
	
// this is the order to appear inside R. S is from Summary
enum aRTtableSenum
{
	aRTtableSClass = 0,
	aRTtableSName,
	aRTtableSType,
	aRTtableSRows,
	aRTtableSColumns,
	aRTtableSKeys,
	aRTtableSSize
};

SEXP aRTtable::Summary()
{
    SEXP info, names, skeys, scolumns;

	TeTable table;
	vector<string> stratts;

	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);
	table.primaryKeys(stratts);
	TeAttributeList attlist = table.attributeList();

	info     = allocVector(VECSXP, aRTtableSSize);  // List containing the connection attributes
    names    = allocVector(STRSXP, aRTtableSSize);  // Names Vector
	skeys    = allocVector(STRSXP, stratts.size()); // keys vector
	scolumns = allocVector(STRSXP, attlist.size());

	for(unsigned i = 0; i < attlist.size();i++)
		SET_STRING_ELT(scolumns, i, mkChar(attlist[i].rep_.name_.c_str()));
	
	for(unsigned i = 0; i != stratts.size(); i++)
		SET_STRING_ELT(skeys, i, mkChar(stratts[i].c_str()));
	
	STRING_LIST_ITEM (info, names, aRTtableSClass,   "class",   Class());
	STRING_LIST_ITEM (info, names, aRTtableSName,    "name",    TableName);
	STRING_LIST_ITEM (info, names, aRTtableSType,    "type",    TeUtils::AttrTableType(table.tableType()));
	INTEGER_LIST_ITEM(info, names, aRTtableSRows,    "rows",    table.size());
	SEXP_LIST_ITEM   (info, names, aRTtableSKeys,    "keys",    skeys);
	SEXP_LIST_ITEM   (info, names, aRTtableSColumns, "columns", scolumns);
	
    setAttrib(info, R_NamesSymbol, names); // Set the names of the list
	
	return info;
}

SEXP aRTtable::Print()
{
	stringstream s;

	if (!IsValid()) error("Invalid object!");

    TeTable table;
	table.name(TableName);
	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);
	s << artOBJECT("aRTtable")  << "\n\n"
	  << "Table: \"" << TableName << "\"" << endl
	  << "Type: " << TeUtils::AttrTableType(table.tableType()) << endl;
	if(LayerName != "")
		s << "Layer: \"" << LayerName << "\"" << endl;
		
	s << "Rows: " << table.size() << endl
	  << "Attributes: " << endl; 

	vector<string> stratts;
	
	TeAttributeList attlist = table.attributeList();
	
	for(unsigned i = 0; i < attlist.size();i++)
	{
		s << "    " << attlist[i].rep_.name_ << ": "
		  << TeUtils::AttrDataType(attlist[i].rep_.type_);

		if(attlist[i].rep_.type_ == TeSTRING)
			s << "[" << attlist[i].rep_.numChar_ << "]";
		
		if(attlist[i].rep_.isPrimaryKey_) s << " (key)";

		s << endl;
	}

	vector<string> pks;
	table.primaryKeys(pks);
/*	s << "Primary keys: ";
	for(int i = 0; i < pks.size(); i++)
		s << pks[i] << "   ";
	s << endl;
*/

	Rprintf( StreamToChar(s) );
	return RNULL;
}

// addRows needs all attributes, and calculates the order according
// to terralib attributes
SEXP aRTtable::AddRows(SEXP data)
{
	TeTable table(TableName);

	Database->loadTableInfo(table);

	SEXP colnames = GetListElement     (data, "colnames");
	SEXP matrix   = GetListElement     (data, "matrix");
	int lrows     = GET_INTEGER_ELEMENT(data, "length");

	unsigned lcols = Rf_length(colnames);
	
	TeAttributeList attlist = table.attributeList();
	int order[attlist.size()];

	PrintSilent("Converting %d attributes to TerraLib format ... ", lcols);
	for(unsigned i = 0; i != attlist.size(); i++)
	{
		order[i] = -2;
		
		if(attlist[i].rep_.type_==TeDATETIME)
		{
			attlist[i].dateTimeFormat_ = "YYYYsMMsDDsHHsmmsSS";
			attlist[i].dateSeparator_  = "-";
			attlist[i].timeSeparator_  = ":";
			attlist[i].dateChronon_    = TeSECOND;
			attlist[i].indicatorPM_    = "";
			attlist[i].indicatorAM_    = "";
		}
		
		// check for table attribute in colnames
		for(unsigned j = 0; j < lcols; j++)
		{
			// TODO: check if NA!!

			string value = CHAR(STRING_ELT(colnames,j));

			if( attlist[i].rep_.name_ == value)
				order[i] = j;
		}

		if(order[i] == -2)
			error("Attribute \'%s\' not found\n", attlist[i].rep_.name_.c_str());
	}
	// changing datetime format (the format is not stored in the database)
	table.setAttributeList(attlist);

	if(attlist.size() != lcols)
		error("There are more attributes in data.frame than in the table\n");
	PrintSilentYes;

	PrintSilent("Converting %d rows to TerraLib format ... ", lrows);
	for(int i = 0; i != lrows; i++)
	{
		TeTableRow row;

		for(unsigned j = 0; j != attlist.size(); j++)
		{
			string value = CHAR(STRING_ELT(matrix, i+order[j]*lrows));
	
			if(attlist[j].rep_.type_ == TeINT  and value == "NA") value = "";
			if(attlist[j].rep_.type_ == TeREAL and value == "NA") value = "";

			row.push_back( value );
		}
		table.add(row);
	}	
	PrintSilentYes;
	
	PrintSilent("Inserting data into table \'%s\' ... ", TableName.c_str());
	if( !Database->insertTable (table) )
		error("Could not insert data in the table\n");
	PrintSilentYes;
	return RNULL;
}

SEXP aRTtable::CreateColumn(SEXP data)
{
	TeAttributeRep atrep;
	
    int size            = GET_INTEGER_ELEMENT(data, "length"    );
    string type         = GET_STRING_ELEMENT (data, "type"      );
	atrep.name_         = GET_STRING_ELEMENT (data, "colname"   );
    atrep.decimals_     = GET_INTEGER_ELEMENT(data, "decimals"  );
	atrep.isPrimaryKey_ = GET_BOOL_ELEMENT   (data, "key"       );
	atrep.isAutoNumber_ = GET_BOOL_ELEMENT   (data, "autonumber");

	TeTable table(TableName);

	if( !Database->loadTableInfo(table) )
        error("Could not load the table info!\n");

	TeAttribute att;
	PrintSilent("Checking for column \'%s\' in table \'%s\' ... ", atrep.name_.c_str(), TableName.c_str());
	if( Database->columnExist(TableName, atrep.name_, att))
		error("Column \'%s\' already exists in \'%s\'\n", atrep.name_.c_str(), TableName.c_str());
	PrintSilentNo;
	
    switch( type[0] )
    {
        case 'i': case 'd': // integer
            atrep.type_ = TeINT;
            break;
		case 'f': case 'c': case 's': // factor, character
            atrep.type_ = TeSTRING;
            atrep.numChar_ = size;
            break;
        case 'n': case 'r': // numeric
            atrep.type_ = TeREAL;
            break;
        default:
            error("Invalid attribute type: %s\n", type.c_str());
	}

	PrintSilent("Creating column \'%s\' in table \'%s\' ... ", atrep.name_.c_str(), TableName.c_str());
	Database->addColumn(TableName, atrep);
	PrintSilentYes;
	return RNULL;
}

// FIXME: gambiarra para fazer funcionar com versao 3.2.1 no mysql, pois da erro nesta funcao
bool aRTtable::MYupdateTable(TeTable &table)
{
    TeAttributeList& att = table.attributeList();
    unsigned int i;
    string uniqueVal;
    bool isUniqueValString = false;

    string uniqueName = table.uniqueName(); // primary key explicitly defined or 
    if (table.uniqueName().empty())         // check in the attribute list
    {
        for (i=0; i<att.size(); ++i)
            if (att[i].rep_.isPrimaryKey_)
            {
                uniqueName = att[i].rep_.name_;
                table.setUniqueName(uniqueName);
                break;
            }
    }

    TeAttributeList::iterator it;
    TeTableRow row;
    unsigned int j;
    for (i = 0; i < table.size(); i++  )
    {
        row = table[i];
        it = att.begin();
        string q;
        j = 1;
        int jj = 0;
        while ( it != att.end() )
        {
            if (uniqueName != (*it).rep_.name_)
            {
                if ((*it).rep_.isAutoNumber_)
                {
                    ++it;
                    j++;
                    jj++;
                    continue;
                }
                q += (*it).rep_.name_ + "=";
                switch ((*it).rep_.type_)
                {
                    case TeSTRING:
                        q += "'"+row[jj]+"'";
                    break;
                    case TeREAL:
                    {
                        std::string value = row[jj];
                        replace(value.begin(), value.end(), ',', '.');
                        q += value;
                    }
                    break;
                    case TeINT:
                        q += row[jj];
                    break;
                    case TeDATETIME:
                    {
                        const string temp_dt = string(row[jj].c_str());
                        TeTime t(temp_dt, (*it).dateChronon_, (*it).dateTimeFormat_, (*it).dateSeparator_, (*it).timeSeparator_, (*it).indicatorPM_);
                        q += Database->getSQLTime(t);
                    }
                    break;
                    case TeCHARACTER:
                        q += "'" + row[jj] + "'";
                    break;
                    case TeBLOB:
                        q += "'" + row[jj] + "'";
                    break;
                    default:
                        q += "'"+row[jj]+"'";
                    break;
                }
                if (j<att.size())
                    q+= ",";
            }
            else
            {
                uniqueVal = row[jj];
                isUniqueValString = ((*it).rep_.type_ == TeSTRING);
            }
            ++it;
            j++;
            jj++;
        }
        if (q.empty())
            continue;

        if (!uniqueName.empty() && !uniqueVal.empty())
        {
            if(isUniqueValString)
                q += " WHERE " + uniqueName + " = '" + uniqueVal +"'";
            else
                q += " WHERE " + uniqueName + " = " + uniqueVal;
        }
        string sql = "UPDATE "+ table.name() + " SET " + q;
        if (!Database->execute(sql))
        {
            return false;
        }
    }
    return true;
}




// update columns doesn't need all atributes, and data
// doesn't need to be in the same order as the database
// table (rows and cols)
SEXP aRTtable::UpdateColumns(SEXP data)
{
	TeTable table(TableName);
	
	Database->loadTableInfo(table);
	SEXP colnames = GetListElement     (data, "colnames");
	SEXP matrix   = GetListElement     (data, "matrix");
	int lrows     = GET_INTEGER_ELEMENT(data, "length");

	int lcols = Rf_length(colnames);
	
	TeAttributeList attlist = table.attributeList();
	TeAttributeList attlist_result;
	
	PrintSilent("Converting %d attributes to TerraLib format ... ", lcols);
	for(int i = 0; i != lcols; i++)
	{
		for(unsigned j = 0; j != attlist.size(); j++)
			if(attlist[j].rep_.name_ == string(CHAR(STRING_ELT(colnames,i))))
				attlist_result.push_back(attlist[j]);
	}

	table.setAttributeList(attlist_result);
	PrintSilentYes;
	
	PrintSilent("Converting %d rows to TerraLib format ... ", lrows);
	
	for(int i = 0; i != lrows; i++)
	{
		TeTableRow row;
		for(int j = 0; j != lcols; j++)
		{
			string value = CHAR(STRING_ELT(matrix, i+j*lrows));
	
			if(attlist[j].rep_.type_ == TeINT  and value == "NA") value = "";
			if(attlist[j].rep_.type_ == TeREAL and value == "NA") value = "";

			row.push_back( value );
		}
		
		table.add(row);
	}
	PrintSilentYes;
	
	PrintSilent("Updating columns of table \'%s\' ... ", TableName.c_str());

	// FIXME: gambiarra para fazer funcionar com versao 3.2.1 no mysql, pois da erro nesta funcao
	if( !MYupdateTable(table) )
	{
		PrintSilentNo;
		PrintSilent("DBError: %s", Database->errorMessage().c_str());
		error("Could not update the table\n");
	}

	PrintSilentYes;
	return RNULL;
}




SEXP aRTtable::CreateRelation(SEXP data)
{
	string fieldName     = GET_STRING_ELEMENT(data, "attr");
	string relatedTable  = GET_STRING_ELEMENT(data, "rtable");
	string relatedField  = GET_STRING_ELEMENT(data, "rattr");

	TeTable table(TableName);
	Database->loadTableInfo(table);

	int relId;
	if( Database->insertRelationInfo (table.id(), fieldName, relatedTable, relatedField, relId) )
		PrintSilent("Relation created successfully\n");
	else
		error("Could not create the relation\n");
	return RNULL;
}

SEXP aRTtable::GetData()
{
    TeTable table;
	table.name(TableName);
	Database->loadTable(TableName, table);
	Database->loadTableInfo(table);

	SEXP result;
	SEXP *each_column;
	SEXP colnames;
	SEXP rownames;
	
	TeAttributeList attlist = table.attributeList();
	
	each_column = new SEXP[attlist.size()];
	colnames    = PROTECT(allocVector( STRSXP, attlist.size() ));
	result      = PROTECT(allocVector( VECSXP, attlist.size() ));
	rownames    = PROTECT(allocVector( STRSXP, table.size() ));
	
	for(unsigned i = 0; i != attlist.size(); i++)
	{
		int type = STRSXP;
		switch(attlist[i].rep_.type_)
		{
			case TeINT:    type = INTSXP;  break;
			case TeREAL:   type = REALSXP; break;
			case TeSTRING:
			case TeDATETIME:
						   type = STRSXP;  break;
			default:
				error("Type of \'%s\' not supported", attlist[i].rep_.name_.c_str());
		}
						 
		each_column[i] = PROTECT(allocVector( type, table.size() ));
		SET_STRING_ELT(colnames, i, mkChar(attlist[i].rep_.name_.c_str()));
	}
	
	for(unsigned i = 0; i < table.size(); i++)
	{
		for(unsigned j = 0; j < attlist.size(); j++)
		{
			string value = table(i,j);
	        switch(attlist[j].rep_.type_)
	        {
	            case TeINT:      INTEGER(each_column[j])[i] = (value == ""? NA_INTEGER : atoi(value.c_str())); break;
	            case TeREAL:     REAL(each_column[j])[i]    = (value == ""? NA_REAL    : atof(value.c_str())); break;
	            case TeSTRING:
				case TeDATETIME: SET_STRING_ELT( each_column[j], i, mkChar(value.c_str()) ); break;
				default: break; // to avoid warning
			}
		}
		
		stringstream str;
		str << i+1; // rownames must start from 1
		SET_STRING_ELT(rownames, i, mkChar(str.str().c_str()));
	}

	setAttrib(result, R_NamesSymbol, colnames);
	
	for(unsigned i = 0; i != attlist.size(); i++)
		SET_VECTOR_ELT(result, i, each_column[i]);

	result = AsDataFrame(result, rownames);
	UNPROTECT(attlist.size()+3);
	delete[] each_column;

	return result;
}

void aRTtable::CreateLinkIds(string idname)
// creates rows with the object's ids
{
	TeAttributeList attList;
	TeAttribute at;
    at.rep_.name_ = "tempCol";
    at.rep_.type_ = TeSTRING;
    at.rep_.numChar_ = 100;
    at.rep_.isAutoNumber_ = false;
    at.rep_.isPrimaryKey_ = false;
    attList.push_back(at);

    PrintSilent("Creating link ids ... ");
	
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

	popule = "INSERT INTO " + TableName + " (" + idname + ") SELECT DISTINCT tempCol FROM tempTable";
    if(Database->execute(popule) == false)
		error("Fail to insert the objects in the table");

    PrintSilentYes;
}

bool aRTtable::IsValid()
{
	return Valid;
}


TeGeomRep GeomRep(TeLayer* Layer)
{
    if( Layer->hasGeometry(TePOLYGONS) ) return TePOLYGONS;
    if( Layer->hasGeometry(TeLINES)    ) return TeLINES;
    if( Layer->hasGeometry(TePOINTS)   ) return TePOINTS;
    if( Layer->hasGeometry(TeRASTER)   ) return TeRASTER;

    error("Layer does not have a valid geometry\n");
    return TeGEOMETRYNONE;
}

TeGeomRep aRTtable::StringToGeomRep(string geom)
{
	TeGeomRep rep;

	switch(geom[2])
	{
		case 'l': rep = TePOLYGONS; break;
		case 'n': rep = TeLINES;    break;
		case 'i': rep = TePOINTS;   break;
	}
	return rep;
}

SEXP aRTtable::CreateAndFillNonSpatial(SEXP data, aRTtheme* other)
{
	string att    = GET_STRING_ELEMENT(data, "att");
	string newatt = GET_STRING_ELEMENT(data, "newatt");
	string strat  = GET_STRING_ELEMENT(data, "strategy");
	string geom   = GET_STRING_ELEMENT(data, "geometry");
	string itime  = GET_STRING_ELEMENT(data, "itime");
	string ftime  = GET_STRING_ELEMENT(data, "ftime");

 	TeTimeInterval t(itime, ftime, TeSECOND, "YYYYsMMsDDsHHsmmsSS", "-", ":", "");
	bool result;

	PrintSilent("Creating column '%s' with operation '%s' ... ", newatt.c_str(), strat.c_str());

	TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* operation;

	     if (strat == "average")    operation = new TeAverageStrategy           <TeSTElementSet::propertyIterator>;
	else if (strat == "sum")        operation = new TeSumStrategy               <TeSTElementSet::propertyIterator>;
	else if (strat == "percentage")	operation = new TeCategoryPercentageStrategy<TeSTElementSet::propertyIterator>;
	else if (strat == "presence")   operation = new TePresenceStrategy          <TeSTElementSet::propertyIterator>;
	else if (strat == "minimum")    operation = new TeMinimumStrategy           <TeSTElementSet::propertyIterator>;
	else if (strat == "maximum")    operation = new TeMaximumStrategy           <TeSTElementSet::propertyIterator>;
	else if (strat == "majority")   operation = new TeMajorityStrategy          <TeSTElementSet::propertyIterator>;
	else error("wrong call to aRTtable::CreateAndFillNonSpatial");

	result = TeFillCellNonSpatialOperation (other->Theme(),
                                       		StringToGeomRep(geom),
                                       		att,
                                       		operation,
                                       		Layer(),
                                       		TableName,
                                       		newatt,
                                       		t);
	delete operation;		
	
	if(result) { PrintSilentYes; }
	else       { PrintSilentNo; error("Error while creating attribute"); }

	return RNULL;
}

SEXP aRTtable::CreateAndFillFromRaster(SEXP data, aRTlayer* other)
{
	string newatt = GET_STRING_ELEMENT(data, "newatt");
	string strat  = GET_STRING_ELEMENT(data, "strategy");
	string itime  = GET_STRING_ELEMENT(data, "itime");
	string ftime  = GET_STRING_ELEMENT(data, "ftime");
	double dummy_v     = GET_REAL_ELEMENT  (data, "dummy");
	double default_v   = GET_REAL_ELEMENT  (data, "default");

 	TeTimeInterval t(itime, ftime, TeSECOND, "YYYYsMMsDDsHHsmmsSS", "-", ":", "");

	PrintSilent("Creating column '%s' with operation '%s' ... ", newatt.c_str(), strat.c_str());

	bool result = true;

	if (strat == "percentage")
	{
		result = TeFillCellCategoryCountPercentageRasterOperation2(Database,
		                                                          other->LayerName,
		                                                          LayerName,
		                                                          TableName,
		                                                          newatt,
		                                                          t);
	}
	else
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* operation;

		     if (strat == "average")  operation = new TeAverageStrategy <TeBoxRasterIterator>(dummy_v, default_v);
		else if (strat == "maximum")  operation = new TeMaximumStrategy <TeBoxRasterIterator>(dummy_v, default_v);
		else if (strat == "minimum")  operation = new TeMinimumStrategy <TeBoxRasterIterator>(dummy_v, default_v);
		else if (strat == "stdev"  )  operation = new TeSTDevStrategy   <TeBoxRasterIterator>(dummy_v, default_v);
		else if (strat == "sum")      operation = new TeSumStrategy     <TeBoxRasterIterator>(dummy_v, default_v);
		else if (strat == "majority")
		{
			stringstream dummy_s;
			stringstream default_s;

			dummy_s << dummy_v;
			default_s << default_v;

			operation = new TeMajorityStrategy<TeBoxRasterIterator>(TeREAL, StreamToChar(default_s), StreamToChar(dummy_s));
		}
		else error("wrong call to aRTtable::CreateAndFillFromRaster");

		result = TeFillCellNonSpatialRasterOperation (Database,
		                                              other->LayerName,
		                                              operation,
		                                              LayerName,
		                                              TableName,
		                                              newatt,
		                                              t);
		delete operation;
	}

	if(result) { PrintSilentYes; }
	else       { PrintSilentNo; error("Error while creating attribute"); }

	return RNULL;
}

SEXP aRTtable::CreateAndFillFromGeom(SEXP data, aRTtheme* other)
{
	string att    = GET_STRING_ELEMENT(data, "att");
	string newatt = GET_STRING_ELEMENT(data, "newatt");
	string strat  = GET_STRING_ELEMENT(data, "strategy");
	string geom   = GET_STRING_ELEMENT(data, "geometry");
	string tname  = GET_STRING_ELEMENT(data, "tname");
	string itime  = GET_STRING_ELEMENT(data, "itime");
	string ftime  = GET_STRING_ELEMENT(data, "ftime");
	double dist   = GET_REAL_ELEMENT  (data, "dist");

 	TeTimeInterval t(itime, ftime, TeSECOND, "YYYYsMMsDDsHHsmmsSS", "-", ":", "");
	
	PrintSilent("Creating column '%s' with operation '%s' ... ", newatt.c_str(), strat.c_str());
	
	bool result = true;

	if (strat == "distance")
	{
		result = TeFillCellDistanceOperation(other->Theme(),
		                                     StringToGeomRep(geom),
		                                     Layer(),
		                                     TableName,
		                                     newatt,
		                                     t);
	}
	else if (strat == "percentage")
	{
		result = TeFillCellCategoryAreaPercentageOperation(other->Theme(),
		                                                   newatt,
		                                                   Layer(),
		                                                   TableName,
		                                                   t);
	}
	else
	{
		TeComputeSpatialStrategy* op;

		     if (strat == "area"  )     op = new TeTotalAreaPercentageStrategy(other->Theme());
		else if (strat == "averagewba") op = new TeAverageWeighByAreaStrategy (other->Theme(), att, TePOLYGONS);
		else if (strat == "length")     op = new TeLineLengthStrategy         (other->Theme());
		else if (strat == "sumwba")     op = new TeSumWeighByAreaStrategy     (other->Theme(), att, TePOLYGONS);
		else if (strat == "count")
		{
			     if (geom == "polygon") op = new TeCountPolygonalObjectsStrategy(other->Theme());
			else if (geom == "line")    op = new TeCountLineObjectsStrategy     (other->Theme());
			else if (geom == "point")   op = new TeCountPointObjectsStrategy    (other->Theme());
			else error("Invalid gometry");
		}
		else if (strat == "xdistance") // distance with a maximum error
		{
			     if (geom == "polygon") op = new TeMinimumDistancePolygonsStrategy(other->Theme(), dist);
			else if (geom == "line")    op = new TeMinimumDistanceLinesStrategy   (other->Theme(), dist);
			else if (geom == "point")   op = new TeMinimumDistancePointsStrategy  (other->Theme());
			else error("Invalid geometry");
		}
		else if (strat == "weighbra")
		{
			int band  = GET_INTEGER_ELEMENT  (data, "band");
			SEXP validValues = GetListElement(data, "validValues");
			SEXP sexpRaster  = GetListElement(data, "raster");

			const int size = LENGTH(validValues);
			vector<int> validRasterValues;

			for(unsigned int i = 0; i < size; i++)
				validRasterValues.push_back(INTEGER(validValues)[i]);

			aRTraster* araster = (aRTraster*) getObj(sexpRaster);

			op = new TeAverageWeightByRestrictedAreaStrategy(other->Theme(), araster->GetTeRaster(), att, validRasterValues, band);
		}
		else error("wrong call to aRTtable::CreateAndFillFromGeom");
		
		result = TeFillCellSpatialOperation (Database,
		                                     op,
		                                     Layer()->name(),
		                                     TableName,
		                                     newatt,
		                                     t);
		delete op;
	}

	if(result) { PrintSilentYes; }
	else       { PrintSilentNo; error("Error while creating attribute"); }

	return RNULL;
}

}

