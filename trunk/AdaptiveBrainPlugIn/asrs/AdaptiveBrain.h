/*
 * AdaptiveBrain.h
 *
 *  Created on: 05/10/2008
 *      Author: ecarrara
 */

#ifndef ADAPTIVEBRAIN_H_
#define ADAPTIVEBRAIN_H_

#include <string>
#include "AutomatoAdaptativo.h"

class AdaptiveBrain
{
public:
	AdaptiveBrain(char* strNomeRobo);

	int getRobotPlan(std::string strMessage, std::string strStateRegisters);

private:
	AutomatoAdaptativo* m_AutomatoAdaptativo;
};

#endif /* ADAPTIVEBRAIN_H_ */
