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
	//Estados
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(1,40);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(0,30);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(2,50);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(3,60);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(4,70);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(5,80);");
	//this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(6,90);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(7,100);");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Estados VALUES(8,110);");

	//Condicoes
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('na','0000000000000000');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('ri','0000000000000010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('cm','0010000000000010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('pr','0001000000010010');");
	//this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('re','0000110000010010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('rm','0000110000010010');");
	//this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('we','00000100000100101');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('oe','0001110000010010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('op','0001110000010110');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('om','0001110000110110');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('rp','0000000000010110');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('em','0000100000110010');");//13/11
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('df','0000000010000010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('vb','1100000000010010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('va','1100000000011010');");//13/11
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('vm','1110000000010010');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Condicoes VALUES('fm','1100000000010110');");

	//Planos
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(0,0,'na',0,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(1,0,'ri',1,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(2,1,'cm',2,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(3,2,'pr',1,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(4,2,'em',7,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(5,7,'om',8,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(6,2,'oe',4,'3');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(7,2,'vb',7,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(8,2,'vm',4,'3');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(9,2,'va',8,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(10,4,'df',5,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(11,2,'rm',2,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(13,7,'oe',8,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(14,7,'rp',2,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(15,2,'fm',7,'2');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(16,8,'rp',2,'i');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(17,2,'om',8,'4');");
	this->m_SQLConnector->executarVoidQuery("INSERT INTO Plano VALUES(17,2,'op',8,'4');");

	//inicializacao da configuracao inicial do automato.
	this->m_strGrupoAdaptacao = GRUPO_ADAPTATIVO_INICIAL;
	query = montarQueryAdaptarIncluirTipo(this->m_strGrupoAdaptacao.c_str());
	this->m_SQLConnector->executarVoidQuery(query.c_str());

	//inicializacao do estado inicial do automato.
	this->m_pEstadoAtual = new Estado(0,0);
	//Estado::sm_pWorkInstance = this->m_pEstadoAtual;
	this->transitar(CONDICAO_PARA_ESTADO_INICIAL);
	//query = montarQueryTransitar(CONDICAO_PARA_ESTADO_INICIAL, ESTADO_INICIAL);
	//this->m_SQLConnector->executarQuery(query.c_str(), Estado::pRefreshState);
	//Estado::sm_pWorkInstance = this->m_pEstadoAtual;
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
	Estado::sm_pWorkInstance = this->m_pEstadoAtual;
	std::string query = montarQueryTransitar(strMascaraCondicao, this->m_pEstadoAtual->getCodEstadoAsCharP());
	this->m_SQLConnector->executarQuery(query.c_str(), Estado::pRefreshState);
	return this->m_pEstadoAtual->wasRefreshed();
	Estado::sm_pWorkInstance = 0;
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
