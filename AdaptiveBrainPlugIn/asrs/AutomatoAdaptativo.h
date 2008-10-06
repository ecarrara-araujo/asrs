#ifndef AUTOMATOADAPTATIVO_H_
#define AUTOMATOADAPTATIVO_H_

//Queries pre-definidas para criacao da base utilizada pelo Automato Adaptativo.
#define QUERY_TBL_AUTOMATO    "CREATE TABLE Automato(IdRegraPlano INTEGER NOT NULL,	PRIMARY KEY (IdRegraPlano)  FOREIGN KEY (IdRegraPlano) REFERENCES Plano (IdRegraPlano));"
#define QUERY_TBL_PLANO       "CREATE TABLE Plano(IdRegraPlano INTEGER NOT NULL, IdEstadoInicial INTEGER NOT NULL,IdCondicao VARCHAR(2) NOT NULL,IdEstadoFinal INTEGER NOT NULL,RobotPlan INTEGER NOT NULL,Tipo CHAR(1) NOT NULL, PRIMARY KEY (IdRegraPlano) FOREIGN KEY (IdCondicao) REFERENCES Condicoes (CondId) FOREIGN KEY (IdEstadoFinal) REFERENCES Estados (EstadoId) FOREIGN KEY (IdEstadoInicial) REFERENCES Estados (EstadoId));"
#define QUERY_TBL_ESTADOS     "CREATE TABLE Estados(EstadoId INTEGER NOT NULL,	PRIMARY KEY (EstadoId));"
#define QUERY_TBL_CONDICOES   "CREATE TABLE Condicoes(CondId VARCHAR(2) NOT NULL,MascaraCondicao VARCHAR(32) NOT NULL,PRIMARY KEY (CondId),UNIQUE (MascaraCondicao));"

//Queries para consulta a base de cohecimento.
#define QUERY_TRANSITAR                 "SELECT Plano.IdEstadoFinal AS EstadoFinal, Plano.RobotPlan AS RobotPlan FROM Plano INNER JOIN Automato ON Automato.IdRegraPlano=Plano.IdRegraPlano INNER JOIN Condicoes ON Plano.IdCondicao=Condicoes.CondId WHERE "
#define QUERY_CONSULTAR_ADAPTACAO       "SELECT Plano.Tipo AS TipoEstado FROM Plano INNER JOIN Condicoes ON Plano.IdCondicao=Condicoes.CondId WHERE "
#define QUERY_ADAPTAR_INCLUIR_TIPO      "INSERT INTO Automato(IdRegraPlano) SELECT Plano.IdRegraPlano FROM Plano WHERE "
#define QUERY_ADAPTAR_EXCLUIR_TIPO      "DELETE FROM Automato WHERE IdRegraPlano IN (SELECT IdRegraPlano FROM Plano WHERE "

#define GRUPO_ADAPTATIVO_INICIAL        "i"
#define CONDICAO_PARA_ESTADO_INICIAL    "0001"
#define ESTADO_INICIAL                  "0"

#define COLUNA_GRUPO_ADAPTACAO          "TipoEstado"

#include <string>
#include "Estado.h"
#include "SQLiteConnector.h"

class AutomatoAdaptativo
{

public:
	AutomatoAdaptativo(char* p_strNomeRobo);
	int getPlan(const char* strMascaraCondicao);

private:
	Estado*                     m_pEstadoAtual;
	SQLiteConnector*            m_SQLConnector;
	bool                        m_bPodeAdaptar;
	std::string                 m_strGrupoAdaptacao;

	void robotInitialize();
	bool adaptar(const char*  strMascaraCondicao);
	bool transitar(const char*  strMascaraCondicao);
	std::string montarQueryTransitar(const char* strMascaraCondicao, char* strEstadoAtual);
	std::string montarQueryConsultarAdaptacao(const char* strMascaraCondicao, char* strEstadoAtual);
	std::string montarQueryAdaptarIncluirTipo(const char* strGrupoRegrasAdaptativas);
	std::string montarQueryAdaptarExcluirTipo(const char* strGrupoRegrasAdaptativas);
	int         verificarAdaptacao(void *NotUsed, int argc, char **argv, char **azColName);

	static int verificarAdaptacaoWrapper(void *NotUsed, int argc, char **argv, char **azColName);
	static int (*pVerificarAdaptacao)(void *NotUsed, int argc, char **argv, char **azColName);

	static AutomatoAdaptativo* sm_pWorkInstance;

};

#endif /* AUTOMATOADAPTATIVO_H_ */
