/*
 * AdaptiveBrain.cpp
 *
 *  Created on: 05/10/2008
 *      Author: ecarrara
 */

#include "AdaptiveBrain.h"

AdaptiveBrain::AdaptiveBrain(char* strNomeRobo)
{
	this->m_AutomatoAdaptativo = new AutomatoAdaptativo(strNomeRobo);
}

int AdaptiveBrain::getRobotPlan(std::string strMessage, std::string strStateRegisters)
{
	std::string strMascaraCondicao = strMessage;
	strMascaraCondicao += strStateRegisters;

	return this->m_AutomatoAdaptativo->getPlan(strMascaraCondicao.c_str());
}
