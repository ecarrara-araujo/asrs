/*
 * Estado.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: BRT
 */
#include "Estado.h"

Estado* Estado::sm_pInstance = 0;
int (*Estado::pRefreshState) (void *NotUsed, int argc, char **argv, char **azColName) = &Estado::refreshStateWrapper;

Estado::Estado(int iCodEstado,int iRobotPlan)
{
	this->m_iCodEstado = iCodEstado;
	this->m_iRobotPlan = iRobotPlan;
	this->m_bWasRefreshed = false;
}

int Estado::refreshState(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	int codEstado = -1;
	int robotPlan = -1;

	for(i=0; i<argc; i++){
		if(strcmp(azColName[i], COLUNA_COD_ESTADO) == 0)
		{
			if(argv[i])
			{	
				codEstado = atoi(argv[i]);
			}
		}
		else if(strcmp(azColName[i], COLUNA_ROBOT_PLAN) == 0)
		{
			if(argv[i])
			{	
				robotPlan = atoi(argv[i]);
			}
		}
	}

	if(codEstado < 0 || robotPlan < 0)
	{
		this->m_bWasRefreshed = false;
		return 0;
	}

	this->m_iCodEstado = codEstado;
	this->m_iRobotPlan = robotPlan;
	this->m_bWasRefreshed = true;
	return 0;
}

int Estado::refreshStateWrapper(void *NotUsed, int argc, char **argv, char **azColName)
{
	return Estado::getInstance()->refreshState(NotUsed, argc, argv, azColName);
}
