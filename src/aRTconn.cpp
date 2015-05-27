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

#include "aRTconn.h"
#include "aRTdb.h"
#include "SEXPbasic.h"

#include <TeUpdateDBVersion.h>
#include <TeApplicationUtils.h>

// TODO: change the following line to TeDefines.h when the problem with HAVE_CONFIG_H
// will be solved
#include <TeDefines.h>//UnixPlatformDefines.h> // TE_USE_...

#include <string>
#include <stdlib.h>

#ifndef TE_USE_MYSQL
	#define TE_USE_MYSQL
#endif

#ifdef TE_USE_MYSQL
	#include <TeMySQL.h>
#endif

#ifdef TE_USE_PGIS 
	#include <TePGInterface.h>
	#include <TePGUtils.h>
	#include <TePostgreSQL.h>
	#include <TePostGIS.h>
#endif

extern "C"
{

SEXP aRTconn::DropUser(SEXP data)
{
	string user      = GET_STRING_ELEMENT(data, "user"     );
	bool remote      = GET_BOOL_ELEMENT  (data, "remote"   );
	string host      = GET_STRING_ELEMENT(data, "host"     );

	PrintSilent("Dropping user \'%s\' ... ", user.c_str());
	TeDatabase* db = NewTeDatabase();

	if( !db -> connect(Host, User, Password, "", Port))
		error("Could not connect\n");

	stringstream stream;
	stream << "drop user " << user;

	if(!remote && host == "") stream << "@localhost";
	else if(host != "")       stream << "@" << host;
	else                      stream << "@\'%\'";

	stream << ";";

	string sql = StreamToChar(stream);

//	cout << sql << endl;

	if( !db->execute(sql) )
	{
		string err = db->errorMessage();
		delete db;
		error( err.c_str() );
	}

	delete db;
    PrintSilentYes;
	return RNULL;
}

SEXP aRTconn::AddPermission(SEXP data)  // only works in MySQL!
{
	string user      = GET_STRING_ELEMENT(data, "user"     );
	bool remote      = GET_BOOL_ELEMENT  (data, "remote"   );
	string host      = GET_STRING_ELEMENT(data, "host"     );
	string pass      = GET_STRING_ELEMENT(data, "pass"     );
	string database  = GET_STRING_ELEMENT(data, "database" );
	string privilege = GET_STRING_ELEMENT(data, "privilege");

	PrintSilent("Adding permissions to user \'%s\' ... ", user.c_str());
	TeDatabase* db = NewTeDatabase();

	if( !db -> connect(Host, User, Password, "", Port))
		error("Could not connect\n");

	stringstream stream;
	stream << "grant " << privilege << " ON " << database << ".* TO " << user;

	if(!remote && host == "") stream << "@localhost";
	else if(host != "")       stream << "@" << host;
	else                      stream << "@\'%\'";

	if(pass != "")
		stream << " IDENTIFIED BY \'" << pass << "\'";
	stream << ";";

	string sql = StreamToChar(stream);

//	cout << sql << endl;

	if( !db->execute(sql) )
	{
		string err = db->errorMessage();
		delete db;
		error( err.c_str() );
	}

	delete db;
    PrintSilentYes;
	return RNULL;
}

SEXP aRTconn::GetPermissions(SEXP data)
{
    SEXP result;
    SEXP *each_column;
    SEXP colnames;
    SEXP rownames;
	string query;
	string user = GET_STRING_ELEMENT(data, "user"  );
	bool global = GET_BOOL_ELEMENT  (data, "global");

	TeDatabase* db = NewTeDatabase();

	if( !db -> connect(Host, User, Password, "", Port))
		error("Could not connect\n");

	TeDatabasePortal* portal = db->getPortal();

	if(global)
		query = "select Host,User,Password,Select_priv,Insert_priv,Update_priv,Delete_priv,Create_priv from mysql.user";
	else
		query = "select Host,User,Db,Select_priv,Insert_priv,Update_priv,Delete_priv,Create_priv from mysql.db";

	if(user != "")
	{
		query += " where user= \"";
		query += user;
		query += "\";";
	}
	else
	{
		query += ";";
	}

	portal->query(query);

    each_column = new SEXP[8];
    colnames    = PROTECT(allocVector( STRSXP, 8 ));
    result      = PROTECT(allocVector( VECSXP, 8 ));
    rownames    = PROTECT(allocVector( STRSXP, portal->numRows() ));

	for(int i = 0; i < 8; i++)
	    each_column[i] = PROTECT(allocVector( STRSXP,  portal->numRows() ));

    SET_STRING_ELT(colnames, 0, mkChar("host"    ));
    SET_STRING_ELT(colnames, 1, mkChar("user"    ));
    SET_STRING_ELT(colnames, 2, mkChar(global? "password": "db"));
	SET_STRING_ELT(colnames, 3, mkChar("select"  ));
    SET_STRING_ELT(colnames, 4, mkChar("insert"  ));
    SET_STRING_ELT(colnames, 5, mkChar("update"  ));
    SET_STRING_ELT(colnames, 6, mkChar("delete"  ));
    SET_STRING_ELT(colnames, 7, mkChar("create"  ));

    for(int i = 0; i != portal->numRows(); i++)
    {
		portal->fetchRow();
        
		string host = portal->getData(0);

		if(host == "%") host = "<any>";
		SET_STRING_ELT( each_column[0], i, mkChar(host.c_str()) );
		
		// user
        SET_STRING_ELT( each_column[1], i, mkChar(portal->getData(1)) );

		if(global)
		{
			string password = portal->getData(2);
			password = password == ""? "No" : "Yes";
			SET_STRING_ELT( each_column[2], i, mkChar(password.c_str()) );
		}
		else
		{
			string db = portal->getData(2);
			SET_STRING_ELT( each_column[2], i, mkChar(db.c_str()) );
		}

		for(unsigned j = 3; j != 8; j++)
		{
			string value = portal->getData(j);
			if(value == "Y") value = "Yes";
			if(value == "N") value = "No";

        	SET_STRING_ELT( each_column[j], i, mkChar(value.c_str()) );
		}

        stringstream str;
        str << i+1; // rownames must start from 1
        SET_STRING_ELT(rownames, i, mkChar(str.str().c_str()));
    }

    setAttrib(result, R_NamesSymbol, colnames);

	for(unsigned j = 0; j != 8; j++)
    	SET_VECTOR_ELT(result, j, each_column[j]);

    result = AsDataFrame(result, rownames);
    UNPROTECT(11);
    delete[] each_column;
	delete db;

    return result;
}

SEXP aRTconn::DeleteDb(SEXP data)
{
	string databasename = GET_STRING_ELEMENT(data, "dbname");

	// "checking ...": R code already does.
	TeDatabase* database = NewTeDatabase();

	if( !database -> connect(Host, User, Password, "", Port))
	{
		PrintSilentNo;
		error("Could not connect\n");
	}
	
	PrintSilent("Deleting database \'%s\' ... ", databasename.c_str());
	
	stringstream stream;
	stream << "DROP DATABASE " << databasename << ";";
	string sql = StreamToChar(stream);

	if( !database->execute(sql) )
	{
		delete database;
		PrintSilentNo;
		error( database->errorMessage().c_str() );
	}
	
	delete database;
    PrintSilentYes;
	return RNULL;
}

SEXP aRTconn::List()
{
	TeDatabase* database = NewTeDatabase();	
	vector<string> db_names;
	SEXP vectorDbs;
	if (!database -> showDatabases (Host, User, Password, db_names, Port))
	{
		delete database;
		return NULL;
	}

	if(!db_names.size())
	{
		return RNULL;
	}

	vectorDbs = allocVector(STRSXP, db_names.size()); // Vector containing the database names

	for (unsigned i = 0; i != db_names.size(); i++)
	  SET_STRING_ELT(vectorDbs, i, mkChar(db_names[i].c_str()));

	delete database;
	return vectorDbs;
}

aRTconn::aRTconn(string user, string pass, unsigned port, string host, string dbms)
{
	PrintSilent("Trying to connect ... ");
	User = user;
	Password = pass;
	Port = port;
	Host = host;

	if     (dbms == "mysql")   { Type = aRTmySQL;    if(!port) Port=3306; }
	else if(dbms == "postgre") { Type = aRTpostgres; if(!port) Port=5432; }
	else if(dbms == "postgis") { Type = aRTpostgis;  if(!port) Port=5432; }
	//else error("Invalid database type: %s\n", dbms.c_str());

	TeDatabase* database = NewTeDatabase();
//	vector<string> db_names;

	bool Valid = database->connect(Host, User, Password,  "", Port);

	if (!Valid)
	{
		PrintSilentNo;
		string error_msg = database -> errorMessage();
		delete database;
		error(error_msg.c_str());
	}
    PrintSilentYes;

	string query = "SELECT VERSION()";

	TeDatabasePortal* portal = database->getPortal();

	portal->query(query);
    portal->fetchRow();

	PrintSilent("Connected to %s version %s\n", dbms.c_str(), portal->getData(0));

	delete database;
}

SEXP aRTconn::Print()
{
	stringstream s;
	string ret;
	string dbms;

    if(Type == aRTmySQL)    dbms = "MySQL";
    if(Type == aRTpostgres) dbms = "PostgreSQL";
    if(Type == aRTsqlite)   dbms = "SQLite";

	s << artOBJECT("aRTconn")
	  << "\n\nDBMS: " << dbms;

	if(Type == aRTsqlite)
	{
		s  << "\nUser: <none>"
		   << "\nUsing password: <none>"
		   << "\nPort: <none>"
		   << "\nHost: localhost"  << endl;
	}
	else
	{
		s  << "\nUser: "           << User
		   << "\nUsing password: " << (Password.size()? "Yes" : "No")
		   << "\nPort: "           << Port
		   << "\nHost: "           << (Host.size()? Host : "localhost") << endl;
	}

	TeDatabase* database = NewTeDatabase();	
	vector<string> db_names;
	SEXP vectorDbs;
	s << "Databases available:\n";
	if (!database -> showDatabases (Host, User, Password, db_names, Port))
	{
		s << "<could not connect!>" << endl;
	}
	else if(!db_names.size())
	{
		s << "<none>" << endl;
	}
	else
	{
		unsigned i = 0;
		for(; i != db_names.size() && i < 12; i++)
		{
			s << "    \"" << db_names[i] << "\"\n";
		}
		if(i != db_names.size())
			s << "    <omitting other " << db_names.size()-i << " databases>";
	}
	s << endl;

	delete database;
	Rprintf( StreamToChar(s) );
	return RNULL;
}

// aRTSummary
enum aRTconnSenum
{
	aRTconnSClass = 0,
	aRTconnSUser,
	aRTconnSPassword,
	aRTconnSPort,
	aRTconnSHost,
	aRTconnSDatabases,
	aRTconnSSum
};

SEXP aRTconn::Summary()
{
	SEXP info, element, names;

	info = allocVector(VECSXP, aRTconnSSum); // List containing the connection attributes
	names = allocVector(STRSXP, aRTconnSSum); // Names Vector

    STRING_LIST_ITEM(info, names, aRTconnSClass,     "class",    Class());
    STRING_LIST_ITEM(info, names, aRTconnSUser,      "user",     User);
    STRING_LIST_ITEM(info, names, aRTconnSPassword,  "password", Password);
    STRING_LIST_ITEM(info, names, aRTconnSHost,      "host",     Host);

    INTEGER_LIST_ITEM(info, names, aRTconnSPort,     "port",     Port);

    SET_STRING_ELT(names, aRTconnSDatabases, mkChar("databases"));
    element = List();
    SET_VECTOR_ELT(info, aRTconnSDatabases, element);

	setAttrib(info, R_NamesSymbol, names); // Set the names of the list
	
	return info;		 
}

// Checks the database type and return a pointer to it.
TeDatabase* aRTconn::NewTeDatabase()
{
	switch(Type)
	{
		case aRTmySQL:
#ifdef TE_USE_MYSQL
			if(!Port) Port=3306;
			return new TeMySQL();
#else
			error("MySQL DBMS not found during the installation");
#endif

		case aRTpostgres:
#ifdef TE_USE_PGIS
			if(!Port) Port=5432;
			return new TePostgreSQL();
#else
			error("PostgreSQL DBMS not found during the installation");
#endif

		case aRTpostgis:
#ifdef TE_USE_PGIS
			if(!Port) Port=5432;
			return new TePostGIS();
#else
			error("PostgreSQL DBMS not found during the installation");
#endif

		default:
#ifdef TE_USE_MYSQL 
			return new TeMySQL();
#endif
#ifdef TE_USE_PGIS
			return new TePostgreSQL();
#endif
			return NULL;
	}
	return NULL;
}

// Try to open an existent database
aRTcomponent* aRTconn::OpenDb(SEXP data)
{
	bool silent = Silent; // **

    string dbname = GET_STRING_ELEMENT(data, "dbname");
	bool update   = GET_BOOL_ELEMENT(data,   "update");

	PrintSilent("Connecting to database \'%s\' ... ", dbname.c_str());
	
	Silent = false; // **
	TeDatabase* database = NewTeDatabase();

	// ** things doesn't work fine if this line executes with Silent == true
	// ** strange... I've already tried to change the name of this variable.
    if (database -> connect(Host, User, Password, dbname, Port))
	{
		Silent = silent; // **
		PrintSilentYes;

		string DBversion;
	    TeDatabasePortal* portal = database->getPortal();
	    if(!portal)
	        return false;

	    string sql = " SELECT db_version FROM te_database ";
	    //The database does not have the te_database connection
	    if(!portal->query(sql))
	    {
	        DBversion = "";
	        delete portal;
	        return NULL;
	    }

	    if(!portal->fetchRow())
	    {
	        DBversion = "";
	        delete portal;
	        return NULL;
	    }

	    DBversion = portal->getData(0);

		PrintSilent("Connected to database version %s\n", DBversion.c_str());

		string version, error_msg;
		if( needUpdateDB(database, version) )
		{
			if(update)
			{
				PrintSilent("Updating database \'%s\' to version %s ... ", dbname.c_str(), TeDBVERSION.c_str());
				if( updateDBVersion(database, version, error_msg) )
					{ PrintSilentYes; }
				else
					error("Could not update the database version: %s\n", error_msg.c_str());
			}
			else warning("Database needs to be updated to version %s. Call this function again with update=TRUE, otherwise things may not work correctly\n", TeDBVERSION.c_str());
		}
	}
	else
	{
		Silent = silent; // **
		PrintSilentNo;
		error("Database \'%s\'does not exist.", dbname.c_str());
	}
	
	aRTdb* root = new aRTdb(database);
	return root;
}

aRTcomponent* aRTconn::CreateDb(SEXP data)
{
    string dbname = GET_STRING_ELEMENT(data, "dbname");

	TeDatabase* database = NewTeDatabase();

	PrintSilent("Creating database \'%s\' ... ", dbname.c_str());
    if (!database -> newDatabase(dbname, User, Password, Host, Port))
	{
        string error_ = database -> errorMessage();
		PrintSilentNo;
        delete database;
		error(error_.c_str());
    }
	PrintSilentYes;
	PrintSilent("Creating conceptual model of database \'%s\' ... ", dbname.c_str());
	if (!database->createConceptualModel(true, false))
	{
		string error_ = database -> errorMessage();
		PrintSilentNo;
		delete database;
		error(error_.c_str());
	}
	PrintSilentYes;

	PrintSilent("Creating application theme table \'%s\' ... ", dbname.c_str());
	if(!createAppThemeTable(database))
	{
		string error_ = database -> errorMessage();
		PrintSilentNo;
		delete database;
		error(error_.c_str());
	}
	PrintSilentYes;

	aRTdb* root = new aRTdb(database);
	return root;
}

}

