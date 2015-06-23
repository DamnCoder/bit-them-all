/**
 * @file GameState.h
 *
 * Contiene la declaración de la clase que representa el estado de
 * la aplicación en el que se juega al BitThemAll
 * <br>
 * Esta implementado como un estado compuesto, pues sera el contenedor
 * y gestor de todos los estados del juego en si.
 *
 * @see aplicacion::CBTAComposeState
 * @see aplicacion::CAplicacion
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */

#ifndef ESTADOJUEGO_H_
#define ESTADOJUEGO_H_

#include "app/controllers/CBTAController.h"

#include "logic/LogicFacade.h"

namespace app {

	class CBitThemAllApp;

	class CGameState :
		public CBTACompositeController,
		public logic::ILogicFacade::GameEventListener,
		public gui::CCameraManager::CameraListener{

	public:

		/**
		 * Declaracion de la clase que representa los subestados del juego.
		 */
		class CGameSubstate : public CBTACompositeController{
		public:

			CGameSubstate(CBitThemAllApp *app, CGameState *gameState, logic::ILogicFacade* f):
				CBTACompositeController(app),
				_parentState(gameState),
				_game(f)
			{}

		protected:

			/** Puntero al estado al que pertenece el subestado */
			CGameState*			_parentState;

			/** Puntero a la fachada logica del juego */
			logic::ILogicFacade*	_game;

		};

		CGameState(CBitThemAllApp* app, logic::ILogicFacade* f);
		~CGameState();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void pause();

		//-------------------------------------------------------------------------
		// METODOS IMPLEMENTADOS COMO OBSERVADOR DE EVENTOS DEL GESTOR DE CAMARAS
		//-------------------------------------------------------------------------
		void emptyCameraQueueEvent();

		//-------------------------------------------------------------------------
		// METODOS IMPLEMENTADOS COMO OBSERVADOR DE EVENTOS DE LA PARTIDA
		//-------------------------------------------------------------------------
		void changeStateEvent(const std::string& name);
		void endGameEvent(const logic::ILogicFacade* f);
		void addCameraEvent(const std::string& camId, const logic::ILogicFacade* facade);
		void clearCamerasEvent();

	protected:

		/**
		 * Método reimplementado por las clases hijas, y que ejecuta la lógica
		 * asociada a este estado. Es llamada antes del dibujado de la
		 * escena.
		 * @param dt Tiempo transcurrido en milisegundos desde la última
		 * actualización de la lógica
		 */
		virtual void executeLogic(float dt);

		/**
		 * Método reimplementado por las clases hijas y que realiza el dibujado
		 * de la escena
		 * @param dt Tiempo transcurrido en milisegundos desde el último
		 * dibujado.
		 */
		virtual void render(float dt);


		//--------------------------------------------------------------------------
		// ATRIBUTOS
		//--------------------------------------------------------------------------

		logic::ILogicFacade* 	_game;

		bool 					_pause;

	};

}

#endif /* ESTADOJUEGO_H_ */
