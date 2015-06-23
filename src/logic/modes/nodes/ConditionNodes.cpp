/*
 * ConditionNodes.cpp
 *
 *  Created on: 25/05/2011
 *      Author: jorge
 */

#include "ConditionNodes.h"

#include "logic/modes/ModeBT.h"
#include "logic/modes/NormalMode.h"
#include "logic/modes/nodes/GameNodes.h"
#include "logic/Game.h"
#include "logic/Question.h"

#include <cassert>

namespace logic {

bool CIntroEndCondition::check(void* agent){
	tGameData* data = (tGameData*)agent;
	bool output = data->game->endIntro();

	if(output){
		//printf("Condicion fin intro: %s\n",output?"true":"false");
	}
	return output;
}
//-----------------------------------------------------------------------------------------------------------------------------------
bool CEndModeCondition::check(void* agent){
	tGameData* data = (tGameData*)agent;
	int currRoundInd = data->game->currentMode()->currentRoundIndex();
	int numRounds = data->game->currentMode()->numRounds();
	bool output =  currRoundInd >= numRounds;
	if(output){
		//printf("Condicion fin de modo %i >= %i %s\n",currRoundInd, numRounds, output?"true":"false");
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CEndCamerasCondition::check(void* agent) {
	tGameData* data = (tGameData*)agent;
	bool output = !data->game->cameraActive();
	if(output){
		//printf("Condicion fin de camaras: %s\n",output?"true":"false");
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CRegisteredAnswer::check(void* agent) {
	tGameData* data = (tGameData*)agent;
	int currentQuestion = data->game->currentMode()->currentQuestionIndex();
	int lastAnswer = data->game->currentMode()->lastRegisteredAnswer();
	bool output = (currentQuestion == lastAnswer);
	//printf("Se ha registrado una pregunta?: %i == %i : %s\n",currentQuestion,lastAnswer,output?"SI":"NO");
	if(output){
		//printf("Se ha registrado una pregunta?: %s\n",output?"true":"false");
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CNoMoreRoundsCondition::check(void* agent) {
	bool output = (_mode->numRounds() >= _mode->currentRoundIndex());
	if(output){
		//printf("Condicion no mas rondas (%i >= %i): %s\n", _mode->numRounds(), _mode->currentRound(), output?"true":"false");
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CEndTimeCondition::check(void* agent) {
	bool output = _mode->elapsedTime() >= _mode->timePerQuestion();
	//printf("Condicion no mas tiempo (%f >= %f): %s\n", _mode->elapsedTime(), _mode->timePerQuestion(), output?"true":"false");
	if(output){
		tGameData* data = (tGameData*)agent;
		data->game->setTimeInHud(data2string(_mode->timePerQuestion()/1000));
		//printf("Condicion no mas tiempo (%f >= %f): %s\n", _mode->elapsedTime(), _mode->timePerQuestion(), output?"true":"false");
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CCorrectAnswer::check(void* agent) {
	bool output = _mode->checkAnswer();
	//std::cout<<"La respuesta es "<<_mode->currentQuestion()->getAnswer()<<"\n";
	//printf("Respuesta correcta?: %s\n", output?"SI":"NO");
	if(output){
		//std::cout<<"La respuesta es "<<_mode->currentQuestion()->getAnswer()<<"\n";
	}
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------

bool CNoMorePlayersCondition::check(void* agent) {
	bool output = (_normalMode->numPlayers() <= _normalMode->currentPlayerIndex());
	//if(output)
		//printf("Condicion no mas jugadores (%i <= %i): %s\n", _normalMode->numPlayers(), _normalMode->currentPlayer(), output?"true":"false");
	return output;
}

//-----------------------------------------------------------------------------------------------------------------------------------


	BEHAVIOR_STATUS CInterruptibleByConditionNode::execute(void* agent)
	{
		// GGG
		//printf("%s - ", _name.c_str());

		//printf("Iniciado?: %s\n",(_init)?"true":"false");
		if (!_init)
			init(agent);

		//printf("Condicion: %s\n",(_condition->check(agent))?"true":"false");
		// Si se cumple la condicion devolvemos exito
		if (_condition->check(agent)){
			children.pop_back();
			return BT_SUCCESS;
		}

		// Si no se cumple y no hay hijos, devolvemos exito
		if (children.size()== 0)
			return BT_SUCCESS;

		// Si no se cumple la condicion se ejecuta el hijo
		BEHAVIOR_STATUS status = children.at(0)->execute(agent);

		//printf("Estatus: %i\n",status);
		return status;
	}

	void CInterruptibleByConditionNode::init( void* agent )
	{
		//printf("Iniciando nodo Interrumpible mediante condicion\n");
		_init = true;
		if (children.size() == 1)
			children.at(0)->init(agent);
	}

	BehaviorTreeInternalNode* CInterruptibleByConditionNode::addChild( BehaviorTreeNode* newChild )
	{
		assert(children.size() == 0);
		BehaviorTreeInternalNode::addChild(newChild);
		return this;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CConditionNode::execute(void* agent)
	{
		if (!_condition->check(agent))
			return BT_SUCCESS;

		// Si se cumple la condicion pero no hay hijos se devuelve exito
		if (children.size()== 0)
			return BT_SUCCESS;

		// Mientras tanto se devuelve el estatus de la ejecucion del hijo
		return children.at(0)->execute(agent);
	}

	void CConditionNode::init( void* agent )
	{
		_init = true;
		if (children.size() == 1)
			children.at(0)->init(agent);
	}

	BehaviorTreeInternalNode* CConditionNode::addChild( BehaviorTreeNode* newChild )
	{
		assert(children.size() == 0);
		BehaviorTreeInternalNode::addChild(newChild);
		return this;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CInvertedConditionNode::execute(void* agent)
	{
		if (_condition->check(agent))
			return BT_SUCCESS;

		// Si se cumple la condicion pero se han ejecutado ya
		// el arbol devolvemos exito
		if (children.size()== 0)
			return BT_SUCCESS;

		// Mientras tanto se devuelve el estatus de la ejecucion del hijo
		return children.at(0)->execute(agent);
	}

//-----------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
// FUNCIONES EXTERNAS
//--------------------------------------------------------------------------------------------

	BehaviorTreeNode* getDoNothingWhileCondition(CCondition* condition) {
		BehaviorTreeInternalNode* bt;
		(bt = new CInterruptibleByConditionNode(condition))
				->addChild((new RepeatNode(-1))
						->addChild(new CDoNothingNode)
				)
				;
		return bt;
	}

	BehaviorTreeNode* getDoWhileCondition(CCondition* condition, BehaviorTreeNode* behavior) {
		BehaviorTreeInternalNode* bt;
		(bt = new CInterruptibleByConditionNode(condition))
				->addChild((new RepeatNode(-1))
						->addChild(behavior)
				)
				;
		return bt;
	}

	BehaviorTreeNode* getIfThenElse(CCondition* condition, BehaviorTreeNode* bt1, BehaviorTreeNode* bt2) {
		BehaviorTreeInternalNode* sequence;

		(sequence = new SequentialNode())
				->addChild((new CConditionNode(condition))
						->addChild(bt1)
						)
				->addChild((new CConditionNode(condition))
						->addChild(bt2)
						)
				;
		return sequence;
	}

}
