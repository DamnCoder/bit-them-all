/**
 * @file LogicFacade.h
 * Contiene la interfaz que han de implementar las clases que sirvan de
 * fachada de comunicación entre lógica y aplicación/GUI
 *
 * @see logic::ILogicFacade
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */

#ifndef IFACADE_H_
#define IFACADE_H_

#include "utilitys/utils.h"

#include "logic/entitys/EntityManager.h"
#include "logic/listener/CreationDestroyListener.h"

namespace logic {

	// Predeclaraciones de clases de la logica

	class CMultimediaFurniture;
	class CScreen;
	class CScenario;
	class CEntity;
	class CAvatar;
	class CPlayer;
	class CQuizShowHost;

	class CUiComponent;
	class CFrame;
	class CLabel;
	class CTextBox;
	class CButton;

	class IRound;
	class CNormalRound;
	class CRound;

	class CQuestionManager;
	class CQuestion;

	enum EIdCommand{
		NOTHING = 0,
		BOTON_JUGAR,
		BOTON_SALIR,

		YK_UP,
		YK_DOWN,
		YK_LEFT,
		YK_RIGHT,

		YK_Z,
		YK_X,
		YK_C,
		YK_V,
		YK_SPACE,

		P1_A,
		P1_B,
		P1_C,
		P1_D,

		P2_A,
		P2_B,
		P2_C,
		P2_D,

		P3_A,
		P3_B,
		P3_C,
		P3_D,

		P4_A,
		P4_B,
		P4_C,
		P4_D,

		GAME_INTRO,
		MODE_INTRO,
		INIT_QUESTIONS,
		END_INTRO,
		END_PRESENTATION,

		// COMANDOS JUEGO
		INIT_PLAY,
		END_PLAY,

		INIT_ROUND,
		NEXT_ROUND,
		END_ROUND,

		ANSWER_TURN,
		END_OF_ANSWER_TURN,
		END_OF_ANSWER_TIME,

		NEXT_PLAYER,
		NEXT_QUESTION,

		// COMANDOS CAMARAS
		END_CAMERAS,

	};

	enum EidGameEvent{
		INIT_EVENT,

		END_INTRO_EVENT,
		END_PRESENTATION_EVENT,

		CORRECT_ANSWER_EVENT,
		WRONG_ANSWER_EVENT,

		INIT_ROUND_EVENT,

		NEXT_PLAYER_EVENT,
		NEXT_QUESTION_EVENT,
		NEXT_ROUND_EVENT,

		END_OF_ANSWER_TURN_EVENT,
		END_OF_ANSWER_TIME_EVENT,
		END_ROUND_EVENT,
		END_GAME_EVENT
	};

	struct tCommand {
		EIdCommand id; // identificador del comando

		int	introId;

		tCommand(EIdCommand pid, int introId) : id(pid), introId(introId) {}
		tCommand(EIdCommand pid) : id(pid) {}
		tCommand():id(NOTHING){}
	};

	struct TGameEvent{
		EidGameEvent id;

		TGameEvent(EidGameEvent _id):id(_id){}
	};

	class CAnimationManager {
	public:
		CAnimationManager(){}

		std::string				_avatarType;

		CManager<std::string> 	_legsAnimations;
		CManager<std::string> 	_torsoAnimations;
		CManager<std::string> 	_headAnimations;
	};

	typedef std::map<std::string, CAnimationManager*> TAnimationsMap;

	class ILogicFacade {
	public:

		/**
		 * Observador de eventos de la partida.
		 * Le comunica al estado de juego "PlayingState" sobre eventos como: acceder a la pausa, que ha terminado el juego, etc..
		 */
		class GameEventListener{
		public:
			virtual ~GameEventListener(){}

			virtual void changeStateEvent(const std::string& name) = 0;
			virtual void endGameEvent(const ILogicFacade* f) = 0;
			virtual void addCameraEvent(const std::string& camId, const ILogicFacade* facade) = 0;
			virtual void clearCamerasEvent() = 0;
		};

		ILogicFacade();
		virtual ~ILogicFacade();

		//--------------------------------------------------------------------------------------
		// Metodos a implementar en las especializaciones
		//--------------------------------------------------------------------------------------

		virtual void init() = 0;
		virtual void finish() = 0;
		virtual void actualize(float dt) = 0;
		virtual void insertCommand(tCommand command) = 0;

		/**********************************************************************************/
		/** METODOS DE ACCESO A LA LOGICA DE LA APLICACION POR PARTE DEL RESTO DE CAPAS **/
/*
		virtual const bool 			endRound() 			const = 0;
		virtual const string 		roundExplanation() 	const = 0;
		virtual CScreen* 			screen() 			const = 0;
		virtual CQuestion* 			getQuestion() 		const = 0;
		virtual CQuizShowHost* 		host() 				const = 0;
		virtual CPlayer* 			roundWinner() 		const = 0;
		virtual CPlayer* 			currentPlayer()		const = 0;
		virtual CScenario* 			scenario() 			const = 0;
		virtual vector<CPlayer*> 	players()	 		const = 0;
		virtual vector<CAvatar*> 	audience() 			const = 0;
		virtual TAnimationsMap		animationsMap()		const = 0;

		virtual void setAudienceAppearance(int numAud, const string& id, const string& skin, int scale) = 0;
*/
		virtual void setAnimationMap(TAnimationsMap animationsMap) = 0;
		virtual void setScenario(CScenario* scenario) = 0;
		virtual void setPlayerAppearance(int numPlayer, const string& id, const string& skin, const string& name, int scale) = 0;
		/**********************************************************************************/

		//--------------------------------------------------------------------------------------
		// METODOS DE ADICION / ELIMINACION DE OBSERVADORES
		//--------------------------------------------------------------------------------------

		bool addListener(CreationDestroyListener* newlistener);
		bool addListener(GameEventListener* newlistener);

		bool removeListener(CreationDestroyListener* oldlistener);
		bool removeListener(GameEventListener* oldlistener);

		const bool playing() const { return _playing; }
		const bool isInit() const { return _init; }

	protected:

		//--------------------------------------------------------------------------------------
		// METODOS DE CREACION / DESTRUCCION DE ENTIDADES EN EL HUD
		//--------------------------------------------------------------------------------------
		void addGfx(CFrame* uiComponent);
		void addGfx(CButton* uiComponent);
		void addGfx(CLabel* uiComponent);
		void addGfx(CTextBox* uiComponent);

		void deleteGfx(CUiComponent* uiComponent);
		void deleteGfx(CFrame* uiComponent);

		//--------------------------------------------------------------------------------------
		// METODOS DE CREACION / DESTRUCCION DE ENTIDADES EN MODO NORMAL
		//--------------------------------------------------------------------------------------
		void addGfx(CScenario* scenario);
		void addGfx(CAvatar* avatar);
		void addGfx(CScreen* screen);
		void addGfx(CEntity* entity);
		void addGfx(CMultimediaFurniture* furniture);

		void deleteGfx(CEntity* entity);
		void deleteGfx(CAvatar* avatar);
		void deleteGfx(CScenario* scenario);

		//--------------------------------------------------------------------------------------
		// Emision de eventos
		//--------------------------------------------------------------------------------------

		void emitChangeState(const std::string& state);
		void emitEndGameEvent();
		void emitAddCamera(const std::string& camId);
		void emitClearCameras();

		bool	_init;
		bool 	_playing;

	private:
		//--------------------------------------------------------------------------------------
		// ATRIBUTOS
		//--------------------------------------------------------------------------------------

		typedef std::list<CreationDestroyListener*> TCreationDestroyListeners;
		typedef std::list<GameEventListener*> 		TGameEventListeners;

		typedef TCreationDestroyListeners::iterator TCDOIterator;
		typedef TGameEventListeners::iterator 		TGEOIterator;

		TGEOIterator				_gameEventIterator, _gameEventEnd;
		TCDOIterator				_cdeIterator, _cdeEnd;

		TGameEventListeners 		_gameEventListeners;		/// Lista de observadores de eventos de la partida
		TCreationDestroyListeners	_creationDestroyListeners;	/// Lista de observadores de eventos de creacion/destruccion de entidades graficas

	};

}

#endif /* IFACADE_H_ */
