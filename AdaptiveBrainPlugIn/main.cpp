#include "breve/slBrevePluginAPI.h"
#include "asrs/AdaptiveBrain.h"

DLLEXPORT void AdaptiveBrainLoadFunctions(void *);

//declaracao das funcoes wrapper
int AdapBrainMakeClass(brEval args[], brEval *target, void *i);
int AdapBrainGetRobotPlan(brEval args[], brEval *target, void *i);

//implementacao das funcoes wrapper
int AdapBrainMakeClass(brEval args[], brEval *target, void *i)
{
	//inicializando o automato adaptativo, enviando o nome do robo como parametro.
	AdaptiveBrain* ab = new AdaptiveBrain(BRSTRING(&args[0]));
	target->set(ab);         //retornando a instancia do objeto de automato adaptativo.
	return EC_OK;
}

int AdapBrainGetRobotPlan(brEval args[], brEval *target, void *i)
{
	AdaptiveBrain* ab = (AdaptiveBrain *)BRPOINTER(&args[0]);
	target->set(ab->getRobotPlan(BRSTRING(&args[1]), BRSTRING(&args[2])));
	return EC_OK;
}

//funcao de carga das funcoes breve nativas.
DLLEXPORT void AdaptiveBrainLoadFunctions(void *data)
{
	brNewBreveCall(data, "AdapBrainMakeClass", AdapBrainMakeClass, AT_POINTER, AT_STRING, 0);
	brNewBreveCall(data, "AdapBrainGetRobotPlan", AdapBrainGetRobotPlan, AT_INT, AT_POINTER, AT_STRING, AT_STRING, 0);
}
