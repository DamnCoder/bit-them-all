/*
 * ModeBT.h
 *
 *  Created on: 08/04/2011
 *      Author: jorge
 */

#ifndef MODEBT_H_
#define MODEBT_H_

#include "nodes/BtNode.h"

#include "logic/LogicFacade.h"

namespace logic {

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	class CBTABehavior {
	public:
		CBTABehavior(CGame* game, BehaviorTreeNode* bt);
		virtual ~CBTABehavior();
		virtual void actualize(float dt);
		virtual void stop() = 0;

	protected:
		tGameData			_data;				// Memoria compartida con todos los nodos de los comportamientos
		BehaviorTreeNode*	_currentBehavior;	// Comportamiento actual
		BEHAVIOR_STATUS		_status;
	};

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	class CIntroBehavior : public CBTABehavior {
	public:
		CIntroBehavior(CGame* game, BehaviorTreeNode* bt, BehaviorTreeNode* stop);
		~CIntroBehavior();
		virtual void stop();

	private:
		BehaviorTreeNode*	_stopBehavior;
	};

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	class CModeBT : public CBTABehavior {
	public:

		CModeBT(const std::string& explanation,
				CGame* game,
				BehaviorTreeNode* bt);
		virtual ~CModeBT();

		// Accesores
		const std::string	explanation()		const { return _explanation; }

		const int			numRounds()			const { return _rounds; }
		const int 			numPlayers()		const { return (int)_players.size(); }
		const int			numQuestions()		const { return (int)_questions.size(); }

		const int			currentRoundIndex()		const { return _currentRound; }
		virtual const int	currentPlayerIndex()	const { return -1; }
		const int 			currentQuestionIndex()	const { return _currentQuestion; }
		CQuestion*			currentQuestion() 		const { return _questions[_currentQuestion]; }

		std::string			rankingDialogue() 			const { return _rankingDialogue; }
		std::string			tiedForFirstPlaceDialogue() const { return _tiedForFirstPlaceDialogue; }
		std::string			nearFirstPlaceDialogue() 	const { return _nearFirstPlaceDialogue; }

		void setRankingDial(const std::string& dial) 			{ _rankingDialogue = dial; }
		void setTiedForFirstPlaceDial(const std::string& dial) 	{ _tiedForFirstPlaceDialogue = dial; }
		void setNearFirstPlaceDial(const std::string& dial) 	{ _nearFirstPlaceDialogue = dial; }

		std::vector<CPlayer*>	playersRanking();
		std::vector<CPlayer*>	playersTiedForFirstPlace();

		const float			elapsedTime()			const { return _elapsedTime; }
		const float			timePerQuestion()		const { return _timePerQuestion; }
		const int			lastRegisteredAnswer()	const { return (int)_answers.size() - 1; }

		const int			pointsPerQuestion()		const { return _pointsPerQuestion; }

		// Modificadores
		void nextQuestion() { ++_currentQuestion; }
		void nextRound() { ++_currentRound; }
		void setElapsedTime(float elapsedTime) { _elapsedTime = elapsedTime; }
		virtual void stop();

		void setBehavior(int behaviorId);
		void setIntro(int introId);

		virtual void registerAnswer(int answer);
		virtual bool checkAnswer();

	protected:

		/// Atributos de la ronda
		std::string			_explanation;		// Explicacion sobre las reglas del modo
		int 				_currentQuestion;	// Pregunta actual
		int					_currentRound;		// Ronda actual
		int					_rounds;			// Numero de rondas para el modo
		int					_pointsPerQuestion;

		/// Control de tiempo
		float				_elapsedTime;		// Tiempo transcurrido
		float				_timePerQuestion;	// Tiempo para responder una pregunta (en segundos)

		/// Comportamientos
		std::vector<BehaviorTreeNode*>	_behaviors;		// Lista de comportamientos
		std::vector<BehaviorTreeNode*>	_intros;

		/// Dialogos
		std::string		_rankingDialogue;
		std::string		_tiedForFirstPlaceDialogue;
		std::string		_nearFirstPlaceDialogue;

		/// Elementos del juego
		CGame*							_game;			// Instancia al juego
		CScenario*						_scenario;		// Elementos del escenario del juego
		std::vector<CQuestion*>			_questions;		// Lista preguntas

		CQuizShowHost*					_host;

		std::vector<CPlayer*>			_players;			// Lista de jugadores
		std::vector<CPlayer*>			_ranking;			// Lista de jugadores ordenados en funcion de su puntuacion
		std::vector<CPlayer*>			_tiedForFirstPlace;	// Lista de jugadores empatados en el primer puesto
		std::vector<CPlayer*>			_nearFirstPlace;	// Lista de jugadores cerca del primero que todavia pueden alcanzarle

		std::vector<int>				_answers;

		/// Elementos graficos del HUD
		CFrame* 			_fQuestionsGui;
		CButton*const*		_bPlayerName;
		CButton*const*		_bColorAnswers;
		CButton*			_bClock;

	};

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------


} // namespace logica

#endif /* MODEBT_H_ */
