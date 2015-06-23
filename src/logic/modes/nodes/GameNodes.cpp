/*
 * IdleNode.cpp
 *
 *  Created on: 09/04/2011
 *      Author: jorge
 */

#include "GameNodes.h"

#include "logic/Game.h"
#include "logic/Question.h"

#include "logic/entitys/Player.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Scenario.h"
#include "logic/entitys/MultimediaFurniture.h"

#include "logic/ui/TextLayout.h"

#include "logic/modes/NormalMode.h"

#include "utilitys/utils.h"
#include "utilitys/Random.h"

namespace logic {

	CIdleNode::CIdleNode(int secs):
			_millisecs(secs*1000.0f),
			_elapsedMillisecs(0)
	{_name = "Idle";}

	CIdleNode::CIdleNode(float millisecs):
		_millisecs(millisecs),
		_elapsedMillisecs(0)
	{
		_name = "Idle";
	}
	CIdleNode::~CIdleNode() {};

	BEHAVIOR_STATUS CIdleNode::execute(void* agent)
	{
		_data = (tGameData*)agent;
		_elapsedMillisecs += _data->dt;
		//std::cout<< "[CIdleNode::execute] "<<_name<<" durante "<<_elapsedMillisecs<<" con dt "<<data->dt<<"\n";
		if(_elapsedMillisecs>_millisecs){
			//std::cout<<"[CIdleNode::execute] Idle durante "<<_elapsedMillisecs<<" milisegundos\n";
			return BT_SUCCESS;
		}

		return BT_RUNNING;
	}

	void CIdleNode::init(void* agent)
	{
		//std::cout<<"Iniciando nodo Idle\n";
		_elapsedMillisecs = 0;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CRandomIdleNode::CRandomIdleNode(float maxMillisecs):
		_maxMillisecs(maxMillisecs),
		_idleMillisecs(0),
		_elapsedMillisecs(0)
	{
		_name = "Idle";

	}
	CRandomIdleNode::~CRandomIdleNode() {};

	BEHAVIOR_STATUS CRandomIdleNode::execute(void* agent)
	{
		_data = (tGameData*)agent;
		_elapsedMillisecs += _data->dt;
		//std::cout<< "[CIdleNode::execute] "<<_name<<" durante "<<_elapsedMillisecs<<" con dt "<<data->dt<<"\n";
		if(_elapsedMillisecs>_idleMillisecs){
			//std::cout<<"[CRandomIdleNode::execute] Idle durante "<<_elapsedMillisecs<<" milisegundos\n";
			return BT_SUCCESS;
		}

		return BT_RUNNING;
	}

	void CRandomIdleNode::init(void* agent)
	{
		_idleMillisecs = (float)utils::CRandom::pointer()->randomNumber((int)_maxMillisecs);
		//std::cout<<"Iniciando nodo Random Idle\n";
		_elapsedMillisecs = 0;
	}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

	CSkinChangeNode::CSkinChangeNode(logic::CEntity* entity, const std::string& skin):
			_entity(entity),
			_skin(skin)
	{
	}

	BEHAVIOR_STATUS CSkinChangeNode::execute(void* agent)
	{
		_data = (tGameData*)agent;
		CScreen* screen = _data->game->scenario()->screen();
		if(screen->visible()){
			screen->setVisible(false);
			screen->stopVideo();
			_entity->setVisible(true);
		}

		_entity->setSkin(_skin);
		//std::cout<<"[CScreenChangeNode::execute] Pantalla mostrando "<<_skin<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CScreenVideoNode::CScreenVideoNode(logic::CScreen* screen, const std::string& videoName):
			_screen(screen),
			_videoName(videoName)
	{
	}

	BEHAVIOR_STATUS CScreenVideoNode::execute(void* agent)
	{
		_data = (tGameData*)agent;
		CEntity* screenMult = _data->game->scenario()->screenMult();
		if(screenMult->visible()){
			screenMult->setVisible(false);
			_screen->setVisible(true);
		}

		_screen->playVideo(_videoName);
		//std::cout<<"[CScreenChangeNode::execute] Pantalla mostrando "<<_skin<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------

	CInsertCommand::CInsertCommand(tCommand command):
			_command(command)
	{
	}

	BEHAVIOR_STATUS CInsertCommand::execute(void* agent)
	{
		tGameData* data = (tGameData*)agent;
		data->game->insertCommand(_command);
		//std::cout<<"[CInsertCommand::execute] Ejecutando comando: "<<_command.id<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CCleanHud::CCleanHud(){}

	BEHAVIOR_STATUS CCleanHud::execute(void* agent) {
		tGameData* data = (tGameData*)agent;
		data->game->cleanHud();
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CEndIntro::CEndIntro(){}

	BEHAVIOR_STATUS CEndIntro::execute(void* agent) {
		tGameData* data = (tGameData*)agent;
		data->game->terminateIntro();
		//std::cout<<"[CEndModeIntroNode::execute] La introduccion del modo termino\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CChangeStateNode::CChangeStateNode(const std::string& state) : _state(state){}

	BEHAVIOR_STATUS CChangeStateNode::execute(void* agent) {
		tGameData* data = (tGameData*)agent;
		data->game->changeState(_state);
		//std::cout<<"[CChangeStateNode::execute] Cambiamos a estado "<<_state<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CWinnersHud::execute(void* agent) {
		for(int i=0; i<4; ++i){
			_data->game->setPlayerHudVisibility(true, i);
		}

		_data->game->createRankingFromScoreHud();
		return BT_SUCCESS;
	}

	void CWinnersHud::init(void* agent) {
		_data = (tGameData*)agent;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CReturnToMenu::execute(void* agent) {
		_data = (tGameData*)agent;
		_data->game->returnToMenu();
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CNextRoundNode::execute(void* agent) {
		_mode->nextRound();
		//std::cout<<"[CIncRoundNode::execute] Incrementamos ronda: "<<_mode->currentRoundIndex()<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CNextQuestionNode::execute(void* agent) {
		_mode->nextQuestion();
		//std::cout<<"[CNextQuestionNode::execute] Siguiente pregunta: "<<_mode->currentQuestionIndex()<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CIncTimeNode::execute(void* agent) {
		tGameData* data = (tGameData*)agent;
		if(_elapsedSecs == 0) data->game->setTimeInHud(data2string(_elapsedSecs));

		_elapsedMillisAux += data->dt;
		_elapsedMillis += data->dt;

		_mode->setElapsedTime(_elapsedMillis);
		if(_elapsedMillisAux>1000.0f){
			++_elapsedSecs;
			data->game->setTimeInHud(data2string(_elapsedSecs));
			_elapsedMillisAux = 0.0f;

			int maxSecs = (int)(_mode->timePerQuestion() / 1000.0f);
			if(_elapsedSecs >= maxSecs){
				_elapsedSecs = 0;
				data->game->setTimeInHud(data2string(_elapsedSecs));
				//std::cout<<"[CIncTimeNode::execute] Han transcurrido ya los "<<maxSecs<<"\n";
			}
			//std::cout<<"[CIncTimeNode::execute] Han transcurrido "<<_elapsedSecs<<" segundos y tenemos "<<maxSecs<<"\n";
		}

		//std::cout<<"[CIncTimeNode::execute] Incrementamos tiempo: "<<_elapsedTime<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CResetTimeNode::execute(void* agent) {
		_mode->setElapsedTime(0);
		tGameData* data = (tGameData*)agent;
		data->game->setTimeInHud(data2string(0));
		//std::cout<<"[CIncTimeNode::execute] Reseteamos tiempo\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CIncorrectAnswer::execute(void* agent) {
		_mode->registerAnswer(-1);		/// Registramos una respuesta incorrecta
		//std::cout<<"[CIncTimeNode::execute] Reseteamos tiempo\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	// Nodos especificos para dialogos del presentador en los modos de juego
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CCreateRankingDialogue::execute(void* agent) {
		std::vector<CPlayer*> ranking = _mode->playersRanking();

		_dialogue = "La clasificación queda como sigue:";

		vector<CPlayer*>::iterator player(ranking.begin()), end(ranking.end());
		for(; player!=end; ++player){
			_dialogue += (std::string)"\n"+(*player)->name() + (std::string)" con "+data2string((*player)->points())+(std::string)" puntos";
		}

		_mode->setRankingDial(_dialogue);

		tGameData* data = (tGameData*)agent;
		data->game->hostTextBox()->setText(_dialogue);

		//std::cout<<"[CRankingDialogue::execute] Dialogo correspondiente al ranking\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CNextPlayerNode::execute(void* agent) {
		_normalMode->nextPlayer();
		//std::cout<<"[CNextPlayerNode::execute] Siguiente jugador: "<<_normalMode->currentPlayerIndex()<<"\n";
		return BT_SUCCESS;
	}

//--------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CSetPointsToCurrentPlayer::execute(void* agent){
		//std::cout<<"[CSetPlayersPointsInHud::execute] "<<_points<<" para el jugador "<<_numPlayer<<"\n";
		tGameData* data = (tGameData*)agent;
		CPlayer* currentPlayer = data->game->currentPlayer();
		currentPlayer->addPoints(_mode->pointsPerQuestion());
		data->game->setPlayerPointsInHud(_mode->currentPlayerIndex(), currentPlayer->points());
		return BT_SUCCESS;
	}

	//--------------------------------------------------------------------------------
	// Funciones externas
	//--------------------------------------------------------------------------------
}
