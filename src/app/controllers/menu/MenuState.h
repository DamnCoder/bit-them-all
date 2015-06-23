/**
 * @file MenuState.h
 *
 * Contiene la declaración de la clase que representa un estado de la
 * aplicación cuando se está mostrando el menú.
 * <br>
 * Esta implementado como un estado compuesto, pues sera el contenedor
 * y gestor de todos los estados del menu.
 *
 * @see aplicacion::CBTAComposeState
 * @see aplicacion::CAplicacion
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */

#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "app/controllers/CBTAController.h"

#include "logic/LogicFacade.h"

namespace gui{
	class COglLight;
}

namespace app {

	using namespace logic;

	class CBitThemAllApp;

	class CMenuState : public CBTACompositeController {
	public:

		class CMenuSubstate : public CBTAController {
		public:
			CMenuSubstate(
					CBitThemAllApp *app,
					CMenuState *menuState,
					ILogicFacade* f):
						CBTAController(app),
						_parentState(menuState),
						_logic(f){}

		protected:
			CMenuState*		_parentState; /** Puntero al estado al que pertenece el subestado */
			ILogicFacade*	_logic; /** Puntero a la fachada logica del juego */

		};

		CMenuState(CBitThemAllApp* app, ILogicFacade* f);
		~CMenuState();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void renderHudFirst(bool first) { _renderHudFirt = first; }

		/**
		 * Establece el escenario a usar de fondo
		 * @param scenario
		 */
		void setBackgroundScenario(CScenario* scenario) { _scenarioBackground = scenario; }
		void setSelectedPlayers(bool selected) { _selectedPlayers = selected; }
		void setSelectedScenario(bool selected){ _selectedScenario = selected; }

		const bool arePlayersSelected() const { return _selectedPlayers; }
		const bool isScenarioSelected() const { return _selectedScenario; }

		/**
		 * Devuelve la lista de jugadores seleccionables
		 * @return
		 */
		vector<CPlayer*> getSelectionablePlayers() const { return _players; }

		/**
		 * Devuelve el escenario a usar de fondo en el menu
		 * @return
		 */
		CScenario* getBackgroundScenario() const { return  _scenarioBackground; }

	protected:
		void render(float dt);

		void createCamera();
		void createLight();
		void createAnimatedEntitys();
		void createScenarios();

		//--------------------------------------------------------------------------
		// ATRIBUTOS
		//--------------------------------------------------------------------------

		ILogicFacade* 					_game;

		CManager<CScenario* >* 			_scenarioManager;
		CManager<CPlayer* >*			_playerManager;

		CScenario*						_scenarioBackground;

		vector<CScenario*> 				_scenarios;
		vector<CPlayer*>				_players;

		vector<CScenario*>::iterator	_scenarioIt, _scenarioEnd;
		vector<CPlayer*>::iterator		_playerIt, _playerEnd;

		gui::COglLight*					_light;

		bool							_renderHudFirt, _selectedPlayers, _selectedScenario;

	};

}

#endif /* MENUSTATE_H_ */
