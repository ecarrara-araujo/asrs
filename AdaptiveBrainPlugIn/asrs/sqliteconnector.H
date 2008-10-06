/*
 * SQLiteConnector.h
 *
 *  Created on: 23/09/2008
 *      Author: ecarrara
 */

#ifndef SQLITECONNECTOR_H_
#define SQLITECONNECTOR_H_

#define SQLITE_EXTENSAO_ARQUIVO       ".db"
#define SQLITE_SUFIXO_DB              "DB"

#include "../sqlite/sqlite3.h"
#include <string>

class SQLiteConnector {

public:
	SQLiteConnector(std::string strNomeDB);
	virtual ~SQLiteConnector();
	int executarVoidQuery(const char *strQuery, int value = 3);
	int executarQuery(const char *strQuery, int (*callback) (void*,int,char**,char**), int value = 3);
	
	static int callback(void *NotUsed, int argc, char **argv, char **azColName);
	static std::string gerarNomeDB(const char* p_cPrefixo);
	

private:
	std::string m_strNomeDB;
	int criarBase(int value = 3);
};

#endif /* SQLITECONNECTOR_H_ */
