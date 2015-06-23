/**
 * @file BitThemAllApp.h
 *
 * Contiene la declaración de la clase de aplicación del juego BitThemAll
 * <br>
 * Esta clase es la especializacion de la clase app::CYaoApp, se encarga de
 * inicializar y finalizar aquellos elementos que tengan que ver con la lógica
 * y sus controladores.
 * <br>
 *
 * @see app::CApplication
 * @see app::CYaoApp
 * @see app::CYaoController
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */

#ifndef CBITTHEMALLAPP_H_
#define CBITTHEMALLAPP_H_

#include "app/YaoApp.h"

#include "logic/Game.h"
#include "logic/entitys/EntityManager.h"

namespace logic{
	class ILogicFacade;
	class CPlayer;
	class CScenario;
}

namespace persist{
	class CDaoServer;
}

namespace app {

	class CBitThemAllApp :
		public CYaoApp{

	public:
		CBitThemAllApp();
		~CBitThemAllApp();

		bool init();
		void end();
		void startGame();

		void setAnimationsMap(logic::TAnimationsMap animationsMap) { _animationsMap = animationsMap; }

		persist::CDaoServer* 					getDaoServer() 		const { return _daoServer; }
		logic::CManager<logic::CScenario* >* 	getScenarioManager()const { return _scenarioManager; }
		logic::CManager<logic::CPlayer* >* 		getPlayerManager()	const { return _playerManager; }
		logic::TAnimationsMap					getAnimationMap()	const { return _animationsMap; }

	protected:

		gui::tDisplayMode loadInitDisplayMode();
		//--------------------------------------------------------------------------------------
		// ATRIBUTOS
		//--------------------------------------------------------------------------------------

		// Elementos de la logica del juego
		logic::CManager<logic::CScenario* >* 	_scenarioManager;
		logic::CManager<logic::CPlayer* >*		_playerManager;
		logic::TAnimationsMap					_animationsMap;
		logic::ILogicFacade* 					_game;

		// Persistencia
		persist::CDaoServer*					_daoServer;				/** Instancia del servidor de Daos */


	};

}

#endif /* CBITTHEMALLAPP_H_ */
