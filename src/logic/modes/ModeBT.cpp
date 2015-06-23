/*
 * ModeBT.cpp
 *
 *  Created on: 08/04/2011
 *      Author: jorge
 */

#include "ModeBT.h"

#include "audio/SdlAudio.h"

#include "logic/Game.h"

#include "logic/modes/nodes/GameNodes.h"
#include "logic/modes/nodes/CameraNodes.h"
#include "logic/modes/nodes/AudioNodes.h"
#include "logic/modes/nodes/UiNodes.h"
#include "logic/modes/nodes/ConditionNodes.h"
#include "logic/modes/nodes/AnimationNodes.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Label.h"
#include "logic/ui/TextLayout.h"

#include "logic/QuestionManager.h"

#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Host.h"

namespace logic {

	CBTABehavior::CBTABehavior(
			CGame* game,
			BehaviorTreeNode* bt):
		_currentBehavior(bt),
		_status(BT_RUNNING)
	{
		_data.game = game;
	}

	CBTABehavior::~CBTABehavior()
	{
		delete _currentBehavior;
		_currentBehavior = 0;
	}

	void CBTABehavior::actualize(float dt){
		if(_status == BT_SUCCESS || !_currentBehavior) return;

		_data.dt = dt;
		_status = _currentBehavior->execute(&_data);

		//if(_status == BT_SUCCESS) printf("Acabo el arbol\n");
	}

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	CIntroBehavior::CIntroBehavior(CGame* game, BehaviorTreeNode* bt, BehaviorTreeNode* stop) :
			CBTABehavior(game, bt),
			_stopBehavior(stop){}
	CIntroBehavior::~CIntroBehavior() {}

	void CIntroBehavior::stop() {
		_currentBehavior = _stopBehavior;
		_status = BT_RUNNING;
	}


	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	CModeBT::CModeBT(
			const std::string& explanation,
			CGame* game,
			BehaviorTreeNode* bt):
		CBTABehavior(game, bt),
		_explanation(explanation),
		_currentQuestion(0),
		_currentRound(0),
		_rounds(1),
		_pointsPerQuestion(10),
		_elapsedTime(0),
		_timePerQuestion(5000),
		_game(game)
	{

		_fQuestionsGui = _game->questionsGui();
		_bPlayerName = _game->playerNameGui();
		_bColorAnswers = _game->colorButton();
		_bClock = _game->clockGui();

		_scenario = game->scenario();
		_host = _scenario->host();
		_questions = CQuestionManager::instance().getRandomQuestions(_rounds*4);
		_players = game->players();
		_ranking = game->players();
	}

	CModeBT::~CModeBT() { }

	/**
	 * Ordena la lista de jugadores del ranking en funcion de sus puntos
	 * conseguidos.
	 * @return Vector de jugadores ordenados
	 */
	std::vector<CPlayer*> CModeBT::playersRanking() {
		sort(_ranking.begin(), _ranking.end(), CPlayer::PointsOrder());

		return _ranking;
	}

	std::vector<CPlayer*> CModeBT::playersTiedForFirstPlace() {
		playersRanking();

		vector<CPlayer*> firstDraws;
		vector<CPlayer*>::iterator player(_ranking.begin()), end(_ranking.end());
		// Comprobacion de empate en el primer puesto
		CPlayer* tempWinner = _ranking[0];
		firstDraws.push_back(tempWinner);
		for(; player!=end; ++player) {
			if(tempWinner->name() != (*player)->name())
				if(tempWinner->points() == (*player)->points())
					firstDraws.push_back(*player);
		}

		return firstDraws;
	}

	void CModeBT::stop() {
		//_status = BT_RUNNING;
	}

	void CModeBT::setBehavior(int behaviorId) {
		_currentBehavior = _behaviors[behaviorId];
		_status = BT_RUNNING;
	}

	void CModeBT::setIntro(int introId) {
		_currentBehavior = _intros[introId];
		_status = BT_RUNNING;
	}

	void CModeBT::registerAnswer(int answer) {
		// Condicion que asegura que no se responda mas de una vez por turno
		if((int)_answers.size() > _currentQuestion)	return;

		// Se registra la pregunta
		_answers.push_back(answer);

		// Si la respuesta es un numero negativo es que no se ha respondido a tiempo
		if(answer < 0) {
			//std::cout<<"[CModeBT::registerAnswer] No se ha respondido por falta de tiempo!\n";
			return;
		}

		// Suena el ruido de elegir
		audio::CAudioManager::pointer()->playEffect(4, -1);
		// Se pone el botón elegido
		_bColorAnswers[answer]->pressed();

		// Guardamos el indice de la respuesta correcta para que se pueda comprobar en el nodo de condicion
		//int answerIndex = _questions[_currentQuestion]->correctAnswerIndex();

		//std::cout<<"[CModeBT::registerAnswer] Llevamos "<<_answers.size()-1<<" respuestas y estas es la numero "<<_currentQuestion<<"\n";
		//std::cout<<"[CModeBT::registerAnswer] Esta es la respuesta "<<_answers.size()-1<<" ";
		//std::cout<<"a la pregunta "<<_currentQuestion<<"\n";
		//std::cout<<"[CModeBT::registerAnswer] La pregunta es: "<<_questions[_currentQuestion]->getQuestion()<<"\n";
		//std::cout<<"[CModeBT::registerAnswer] Y has elegido la opcion: "<<answer<<" ";
		//std::cout<<"que es: "<<_questions[_currentQuestion]->getAnswers().at(answer)<<"\n";
		//std::cout<<"[CModeBT::registerAnswer] La respuesta correcta es la numero: "<<answerIndex<<" ";
		//std::cout<<"que es "<<_questions[_currentQuestion]->getAnswer()<<"\n";
	}

	bool CModeBT::checkAnswer() {
		return true;
	}

}
