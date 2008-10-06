/*
 * AutomatoAdaptativo.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: BRT
 */

#include "AutomatoAdaptativo.h"

AutomatoAdaptativo* AutomatoAdaptativo::sm_pWorkInstance = 0;
int (*AutomatoAdaptativo::pVerificarAdaptacao) (void *NotUsed, int argc, char **argv, char **azColName) = &AutomatoAdaptativo::verificarAdaptacaoWrapper;

AutomatoAdaptativo::AutomatoAdaptativo(char* p_cNomeRobo)
{
	this->m_bPodeAdaptar = false;

	std::string nomeDB = SQLiteConnector::gerarNomeDB(p_cNomeRobo);
	this->m_SQLConnector = new SQLiteConnector(nomeDB);
	this->robotInitialize();
}

void AutomatoAdaptativo::robotInitialize()
{
	std::string query;

	//inicialiacao da estrutura da base de dados.
	this->m_SQLConnector->executarVoidQuery(QUERY_TBL_AUTOMATO);
	this->m_SQLConnector->executarVoidQuery(QUERY_TBL_PLANO);
	this->m_SQLConnector->executarVoidQuery(QUERY_TBL_ESTADOS);
	this->m_SQLConnector->executarVoidQuery(QUERY_TBL_CONDICOES);

	//inicializacao do conjunto de regras, estados e condicoes disponiveis.
	//implementar...
	//condicoes
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('a','0000000000000000');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('b','1000000000000000');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('c','2000000000000000');");

	//Estados
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(0);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(1);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(2);");

	//Planos
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(0,0,'a',0,2,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(1,0,'b',1,3,'1');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(3,1,'c',0,2,'1');");

	//inicializacao da configuracao inicial do automato.
	this->m_strGrupoAdaptacao = GRUPO_ADAPTATIVO_INICIAL;
	query = montarQueryAdaptarIncluirTipo(this->m_strGrupoAdaptacao.c_str());
	this->m_SQLConnector->executarVoidQuery(query.c_str());

	//inicializacao do estado inicial do automato.
	this->m_pEstadoAtual = Estado::getInstance();
	query = montarQueryTransitar(CONDICAO_PARA_ESTADO_INICIAL, ESTADO_INICIAL);
	this->m_SQLConnector->executarQuery(query.c_str(), Estado::pRefreshState);
}

int AutomatoAdaptativo::getPlan(const char* cMascaraCondicao)
{
	this->m_pEstadoAtual->setRefreshed(false);

	if(!transitar(cMascaraCondicao))
		if(adaptar(cMascaraCondicao))
			transitar(cMascaraCondicao);

	return this->m_pEstadoAtual->getRobotPlan();
}

bool AutomatoAdaptativo::transitar(const char*  strMascaraCondicao)
{
	std::string query = montarQueryTransitar(strMascaraCondicao, this->m_pEstadoAtual->getCodEstadoAsCharP());
	this->m_SQLConnector->executarQuery(query.c_str(), Estado::pRefreshState);
	return this->m_pEstadoAtual->wasRefreshed();
}

bool AutomatoAdaptativo::adaptar(const char* strMascaraCondicao)
{
	std::string query;

	//verifica se eh possivel adaptar
	AutomatoAdaptativo::sm_pWorkInstance = this;
	query = montarQueryConsultarAdaptacao(strMascaraCondicao, this->m_pEstadoAtual->getCodEstadoAsCharP());
	this->m_SQLConnector->executarQuery(query.c_str(), AutomatoAdaptativo::pVerificarAdaptacao);
	AutomatoAdaptativo::sm_pWorkInstance = 0;

	//se possivel adapta
	if(this->m_bPodeAdaptar)
	{
		query = montarQueryAdaptarIncluirTipo(this->m_strGrupoAdaptacao.c_str());
		this->m_SQLConnector->executarVoidQuery(query.c_str());
	}

	return this->m_bPodeAdaptar;
}

std::string AutomatoAdaptativo::montarQueryTransitar(const char* strMascaraCondicao, char* strEstadoAtual)
{
	std::string query(QUERY_TRANSITAR);
	query += "Condicoes.MascaraCondicao=\"";
	query += strMascaraCondicao;
	query += "\" AND Plano.IdEstadoInicial=";
	query += strEstadoAtual;
	query += ";";

	return query;
}

std::string AutomatoAdaptativo::montarQueryConsultarAdaptacao(const char* strMascaraCondicao, char* strEstadoAtual)
{
	std::string query(QUERY_CONSULTAR_ADAPTACAO);
	query += "Condicoes.MascaraCondicao=\"";
	query += strMascaraCondicao;
	query += "\" AND Plano.IdEstadoInicial=";
	query += strEstadoAtual;
	query += ";";

	return query;
}

std::string AutomatoAdaptativo::montarQueryAdaptarIncluirTipo(const char* strGrupoRegrasAdaptativas)
{
	std::string query(QUERY_ADAPTAR_INCLUIR_TIPO);
	query += "Plano.Tipo=\"";
	query += strGrupoRegrasAdaptativas;
	query += "\";";

	return query;
}

std::string AutomatoAdaptativo::montarQueryAdaptarExcluirTipo(const char* strGrupoRegrasAdaptativas)
{
	std::string query(QUERY_ADAPTAR_EXCLUIR_TIPO);
	query += "Tipo=\"";
	query += strGrupoRegrasAdaptativas;
	query += "\");";

	return query;
}

int AutomatoAdaptativo::verificarAdaptacaoWrapper(void *NotUsed, int argc, char **argv, char **azColName)
{
	return AutomatoAdaptativo::sm_pWorkInstance->verificarAdaptacao(NotUsed, argc, argv, azColName);
}

int AutomatoAdaptativo::verificarAdaptacao(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	std::string grupo;

	for(i=0; i<argc; i++){
		if(strcmp(azColName[i], COLUNA_GRUPO_ADAPTACAO) == 0)
		{
			if(argv[i])
			{
				grupo = argv[i];
			}
		}
	}

	if(grupo.empty())
	{
		this->m_bPodeAdaptar = false;
		return 0;
	}

	this->m_strGrupoAdaptacao = grupo;
	this->m_bPodeAdaptar = true;
	return 0;
}
