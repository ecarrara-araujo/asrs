/*
 * SQLiteConnector.cpp
 *
 *  Created on: 23/09/2008
 *      Author: ecarrara
 */

#include "SQLiteConnector.h"

SQLiteConnector::SQLiteConnector(std::string strNomeDB)
{
	this->m_strNomeDB = strNomeDB;
	criarBase();
}

SQLiteConnector::~SQLiteConnector()
{
}

int SQLiteConnector::criarBase(int iValue)
{
	  sqlite3 *db;
	  int rc;
	  if( iValue!=3 ){
	    //fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", value);
	    return 1;
	  }
	  rc = sqlite3_open(this->m_strNomeDB.c_str(), &db);
	  if( rc ){
	    //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	    sqlite3_close(db);
	    return 1;
	  }
	  return 0;
}

int SQLiteConnector::executarVoidQuery(const char *strQuery, int iValue)
{
	  sqlite3 *db;
	  char *zErrMsg = 0;
	  int rc;
	  if( iValue!=3 ){
	    //fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", value);
		  return 1;
	  }
	  rc = sqlite3_open(this->m_strNomeDB.c_str(), &db);
	  if( rc ){
	    //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	    sqlite3_close(db);
	    return 1;
	  }
	  rc = sqlite3_exec(db, strQuery, 0, 0, &zErrMsg);
	  if( rc!=SQLITE_OK ){
	    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    sqlite3_free(zErrMsg);
	  }
	  sqlite3_close(db);
	  return 0;
}

int SQLiteConnector::executarQuery(const char *strQuery, int (*callback) (void*,int,char**,char**), int iValue){
	  sqlite3 *db;
	  char *zErrMsg = 0;
	  int rc;
	  if( iValue !=3 ){
	    //fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", value);
		return 1;
	  }
	  rc = sqlite3_open(this->m_strNomeDB.c_str(), &db);
	  if( rc ){
	    //fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	    sqlite3_close(db);
	    return 1;
	  }
	  rc = sqlite3_exec(db, strQuery, callback, 0, &zErrMsg);
	  if( rc!=SQLITE_OK ){
	    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    sqlite3_free(zErrMsg);
	  }
	  sqlite3_close(db);
	  return 0;
}

std::string SQLiteConnector::gerarNomeDB(const char* p_cPrefixo)
{
	std::string nome(p_cPrefixo);
	nome += SQLITE_SUFIXO_DB;
	nome += SQLITE_EXTENSAO_ARQUIVO;
	
	return nome;
}

