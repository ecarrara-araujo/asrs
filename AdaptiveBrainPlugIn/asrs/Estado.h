#ifndef ESTADO_H_
#define ESTADO_H_

#define COLUNA_COD_ESTADO          "EstadoFinal"
#define COLUNA_ROBOT_PLAN          "RobotPlan"

#include <stdio.h>
#include <string>

class Estado
{

public:
	Estado(int iCodEstado,int iRobotPlan);
	
	static Estado* sm_pWorkInstance;

	int        getCodEstado()                {return m_iCodEstado;};
	int        getRobotPlan()                {return m_iRobotPlan;};
	bool       wasRefreshed()                {return m_bWasRefreshed;};
	void       setRefreshed(bool bRefreshed) {m_bWasRefreshed = bRefreshed;};

	static int refreshStateWrapper(void *NotUsed, int argc, char **argv, char **azColName);
	static int (*pRefreshState)(void *NotUsed, int argc, char **argv, char **azColName);

	char*         getCodEstadoAsCharP()
	{
		char* codEstado = new char[16];
		sprintf(codEstado, "%d", m_iCodEstado);
		return codEstado;
	};

private:
	int            m_iCodEstado;
	int            m_iRobotPlan;
	bool           m_bWasRefreshed;

	int            refreshState(void *NotUsed, int argc, char **argv, char **azColName);	

};
#endif /* ESTADO_H_ */
