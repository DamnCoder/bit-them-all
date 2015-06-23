/*
 * mMoloApp.cpp
 *
 *  Created on: 27/07/2010
 *      Author: jorge
 */

#include "BitThemAllApp.h"

#include "app/controllers/StateHandler.h"

#include "controllers/juego/EstadoJuego.h"
#include "controllers/menu/MenuState.h"
#include "controllers/load/LoadState.h"
#include "controllers/test/StressTest.h"
#include "controllers/test/ButtonTest.h"
#include "controllers/test/CameraTest.h"
#include "controllers/test/VideoTest.h"

#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"

#include "persist/DaoServer.h"

const char* configPath = "./data/config.xml";

namespace app {

	CBitThemAllApp::CBitThemAllApp() :
		CYaoApp(),
		_scenarioManager(0),
		_playerManager(0),
		_game(0),
		_daoServer(0)
	{}

	CBitThemAllApp::~CBitThemAllApp() { }

	bool CBitThemAllApp::init(){

		if(!CYaoApp::init())
			return false;

		// Creamos el juego
		_game = new logic::CGame();

		// Creamos el gestor de escenarios y de jugadores
		_scenarioManager = new logic::CManager<logic::CScenario*>();
		_playerManager = new logic::CManager<logic::CPlayer*>();

		// Recuperamos la instancia del servidor de DAOs
		// (debe ser la primera vez que se llama
		// para controlar donde se inicia)
		_daoServer 		= persist::CDaoServer::pointer();

		// Creamos los estados
		addState("load", new CLoadState(this));

		setState("load");

		return true;
	}

	void CBitThemAllApp::end(){
#ifdef _DEBUG
		//cout<<"Terminamos la aplicacion BitThemAll...\n";
#endif
		CYaoApp::end();

		// Destruimos la instancia del servidor de DAOs
		// (debe ser la ultima vez que se llama al
		// terminar la aplicacion)
		_daoServer->destroyInstance();
		_daoServer = 0;

		//cout<<"Elimino el gestor de personajes\n";
		safeDelete(_playerManager);
		//cout<<"Elimino el gestor de escenarios\n";
		safeDelete(_scenarioManager);
		//cout<<"Elimino la clase Juego\n";
		safeDelete(_game);
#ifdef _DEBUG
		//cout<<"Terminada aplicacion BitThemAll!!\n";
#endif
	}

	void CBitThemAllApp::startGame(){

		addState("juego", new CGameState(this, _game));
		addState("menu",	new CMenuState(this, _game));

		addState("stress",	new CStressTest(this));
		//addState("button",	new CButtonTest(this));
		addState("video",	new CVideoTest(this));
		addState("camera",	new CCameraTest(this));

		setState("menu");

	}

	gui::tDisplayMode CBitThemAllApp::loadInitDisplayMode(){
		// Solicitamos el DAO correspondiente a los display modes
		persist::IDao<gui::tDisplayMode>* scrDao = _daoServer->getScreenConfigDAO(configPath);

		return scrDao->read(0);
	}

}

