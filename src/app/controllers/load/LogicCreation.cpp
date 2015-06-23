/*
 * LoadWarningState.cpp
 *
 *  Created on: 04/12/2010
 *      Author: jorge
 */

#include "LogicCreation.h"

#include "persist/DaoServer.h"
#include "persist/md3/AnimationLoader.h"

namespace app {

	CLogicCreation::~CLogicCreation(){}

	void CLogicCreation::onCreate(){
		CYaoController::onCreate();
		//std::cout<<"Creado subestado ScenarioCreation\n";

		// Solicitamos a la aplicacion los elementos necesarios para cargar escenarios

		_scenarioDAO = _appBTA->getDaoServer()->getScenarioDAO("./data/scenarios/scenarios.xml");
		_playerDAO = _appBTA->getDaoServer()->getPlayerDAO("./data/resources/players.xml");
		_animationsDAO = _appBTA->getDaoServer()->getAnimationsDAO("./data/resources/gfx_animated.xml");

		_scenarioManager = _appBTA->getScenarioManager();
		_playerManager = _appBTA->getPlayerManager();

	}

	void CLogicCreation::onDestroy(){
		CYaoController::onDestroy();
		//std::cout<<"Destruido subestado ScenarioCreation\n";
	}

	void CLogicCreation::onActivate(){
		CYaoController::onActivate();
		//std::cout<<"Activado subestado ScenarioCreation\n";
		_order = 0;
	}

	void CLogicCreation::onDeactivate(){
		CYaoController::onDeactivate();
		//std::cout<<"Desactivado subestado ScenarioCreation\n";
	}

	void CLogicCreation::onRun(float dt){
		CYaoController::onRun(dt);

		switch(_order){
		case 0:
		{
			// Leemos todos los escenarios
			_parentState->resourceTypeInLoad("scenarios");
			std::vector<CScenario*> _scenarios = _scenarioDAO->read();

			_scenarioManager->add(_scenarios);

			_parentState->addLoadFinishLabel("SCENARIOS");
			++_order;
		}
			break;
		case 1:
		{
			persist::CAnimationLoader animLoader;

			_parentState->resourceTypeInLoad("players");

			vector<CPlayer*> players = _playerDAO->read();

			_animationsMap = _animationsDAO->read(0);

			_appBTA->setAnimationsMap(_animationsMap);

			_playerManager->add(players);

			_parentState->addLoadFinishLabel("PLAYERS");
			++_order;
		}
			break;

		case 2:
			_parentState->resourceTypeInLoad("END");
			_order = 0;
			_appBTA->startGame();
			break;
		}




	}

}
