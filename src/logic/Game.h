/**
 * CGame.h
 *
 *  Created on: 22/01/2010
 *      Author: jorge
 */

#ifndef PARTIDA_H_
#define PARTIDA_H_

#include "LogicFacade.h"

#include "logic/modes/ModeBT.h"

namespace logic {

	class CGame : public ILogicFacade {
	public:

		CGame();
		~CGame();

		void init();
		void finish();
		void actualize(float dt);
		void insertCommand(tCommand command);

		/*****************************************************************************************/
		/** METODOS DE ACCESO A LA LOGICA DE LA APLICACION POR PARTE DEL RESTO DE CAPAS  *********/

		const bool			endRound() 			const;
		const string 		roundExplanation() 	const;
		CScreen* 			screen() 			const;
		CScenario* 			scenario() 			const { return _scenario; }
		int 				modesNumber()	 	const { return (int)_modes.size(); }
		CQuestion* 			getQuestion() 		const { return 0; }
		CQuizShowHost* 		host() 				const;
		CPlayer* 			roundWinner() 		const { return 0; }
		CPlayer* 			currentPlayer() 	const;
		vector<CPlayer*> 	players() 			const;
		vector<CAvatar*> 	audience() 			const;
		TAnimationsMap		animationsMap()		const { return _animationsMap; }
		CModeBT*			currentMode()		const { return _currentMode; }

		void setAnimationMap(TAnimationsMap animationsMap);
		void setScenario(CScenario* scenario);
		void setPlayerAppearance(int numPlayer, const string& id, const string& skin, const string& name, int scale);
		void setAudienceAppearance(int numPlayer, const string& id, const string& skin, int scale);

		/*****************************************************************************************/

		void returnToMenu() {emitEndGameEvent();}

		// Gestion de camaras
		void setCameraActive(bool cameraActive) { _cameraActive = cameraActive; }
		void addCamera(const string& camID);
		void clearCameras();
		const bool cameraActive()	 const { return _cameraActive; }

		// FLAGS DE JUEGO
		void setEndIntro(const bool endIntro) { _endIntro = endIntro; }
		void setEndModeIntro(const bool endModeIntro) { _endModeIntro = endModeIntro; }

		const bool endIntro()		const { return _endIntro; }
		const bool endModeIntro()	const { return _endModeIntro; }
		void terminateIntro();

		void changeState(const std::string& state);

		// CONTROL DEL HUD
		void setTimeInHud(const std::string& time);
		void setHostTextBoxVisibility(const bool visible);
		void setPlayerHudVisibility(const bool visible, const int numPlayer);
		void setPlayerPointsInHud(const int numPlayer, const int numPoints);
		void createRankingFromScoreHud();
		void setQuestionInHud(CQuestion* question);
		void cleanHud();

		CTextBox*		hostTextBox()	const { return _tbHost; }
		CButton* 		clockGui() 		const { return _bClock; }
		CFrame*			questionsGui() 	const { return _fQuestionsGui; }
		CButton* const* playerNameGui() const { return _bPlayerName; }
		CButton* const*	colorButton() 	const { return _bColorAnswers; }
		CButton*		colorButton(int i) const { return _bColorAnswers[i]; }

	protected:
		void processCommandQueue();
		void processEventQueue();

	private:
		///--------------------------------------------------------------------------
		/// ATRIBUTOS DEL JUEGOS
		///--------------------------------------------------------------------------
		typedef vector<tCommand>::iterator		TCommandIterator;
		typedef vector<TGameEvent>::iterator 	TGameEventIterator;

		vector<tCommand>	_commandQueue;	// Cola de comandos
		vector<TGameEvent> 	_eventQueue;	// Cola de eventos

		// Entidades del juegos
		vector<CPlayer*>	_players;		// Lista de jugadores
		vector<CPlayer*>	_ranking;		// Ranking de la pruebas
		vector<CPlayer*>	_tiedPlayers;	// Lista de jugadores empatados en el primer puesto
		CScenario*			_scenario;

		// Gestores de animaciones de personajes
		TAnimationsMap		_animationsMap;

		//------------------
		// Comportamientos

		// Comportamientos de los modos
		uint				_nMode;	// Ronda que se esta jugando en estos momentos
		vector<CModeBT*>	_modes;
		CModeBT*			_currentMode;

		// Comportamientos de las secuencias animadas
		vector<CIntroBehavior*> _intros;

		CBTABehavior*			_currentBehavior;	// Comportamiento actual en ejecucion
		//------------------



		// Preguntas
		CQuestionManager* 	_questionManager;
		vector<CQuestion*>	_questions;				// Listas de preguntas

		// Flags del juego
		bool				_cameraActive;
		bool				_endIntro;
		bool				_endModeIntro;

		///--------------------------------------------------------------------------
		/// FUNCIONES PRIVADAS
		///--------------------------------------------------------------------------
		void nextMode();
		void registerAnswer(int answer);

		void actualizeEntitys(float dt);
		void actualizeBehavior(float dt);

		void emitPlayers();
		void emitPublic();

		void destroyAnimatedEntitys();

		///--------------------------------------------------------------------------
		/// COMPORTAMIENTOS
		///--------------------------------------------------------------------------
		BehaviorTreeNode* getIntroSequence();
		BehaviorTreeNode* getEndIntroSequence();

		BehaviorTreeNode* getPlayersPresentationSequence();
		BehaviorTreeNode* getEndPlayersPresentation();

		BehaviorTreeNode* getEndModeSequence();
		BehaviorTreeNode* getEndEndModeSequence();

		BehaviorTreeNode* getEndDemoSequence();
		BehaviorTreeNode* getEndEndDemoSequence();

		BehaviorTreeNode* initQuizShowCharactersAnimation() const;

		///--------------------------------------------------------------------------
		/// HUD
		///--------------------------------------------------------------------------
		void createGui();
		void destroyGui();

		void createHostTextBox();
		void destroyHostTextBox();

		void createScoreHud();
		void destroyScoreHud();

		void createQuestionsHud();
		void destroyQuestionsHud();

		//void setPlayerHudVisibility(const bool visible, const int numPlayer);

		// GUI DEL JUEGO
		CFrame*		_introGui;
		CFrame* 	_fWhiteFlash;

		CFrame*		_fLogoTv;
		CLabel*		_lQuit;
//
		// Gui de dialogos del Host
		CTextBox*	_tbHost;

		// Gui de puntos
		CFrame*		_scoreGui;
		CFrame*		_fIconFrame[4];		// Marco de los iconos
		CFrame*		_fPlayerIcon[4];	// Iconos de los jugadores
		CButton*	_bPlayerPoints[4];
		CButton*	_bPlayerName[4];

		// Gui de preguntas
		CFrame*		_fQuestionsGui;
		CButton*	_bColorAnswers[4];
		CButton*	_bAnswers[4];
		CButton*	_bQuestion;
		CButton*	_bClock;
	};

}

#endif /* PARTIDA_H_ */
