/*
 * BTAGameState.cpp
 *
 *  Created on: 08/09/2010
 *      Author: jorge
 */

#include "EstadoJuego.h"

#include "Subestados.h"

#include "logic/Game.h"

namespace app {

	CGameState::CGameState(CBitThemAllApp* app, logic::ILogicFacade* f) :
				CBTACompositeController(app),
			_game(f),
			_pause(false)
	{}

	CGameState::~CGameState() {
	}

	void CGameState::onCreate(){
		//std::cout<<"Creado estado contenedor Juego\n";
		CYaoCompositeController::onCreate();

		// Creamos el Gestor de escena, el gestor de Hud y el gestor de camaras
		activateGfxManagers();

		// Añadimos los listeners necesarios a la logica
		_game->addListener((logic::CreationDestroyListener*)_sceneManager);

		// Incluimos al estado de juego como listeners de los eventos de la partida
		_game->addListener((logic::ILogicFacade::GameEventListener*)this);

		_game->setAnimationMap(_appBTA->getAnimationMap());

		// El estado juego tambien escuchara eventos del gestor de camaras
		_cameraManager->addListener(this);

		// Añadimos los distintos estados de la aplicacion
		addState("intro", 				new CIntroSubstate(	_appBTA, this, _game, logic::GAME_INTRO, 0));
		addState("intro_concursantes", 	new CIntroSubstate(	_appBTA, this, _game, logic::GAME_INTRO, 1));
		addState("intro_ganadores", 	new CIntroSubstate(	_appBTA, this, _game, logic::GAME_INTRO, 2));
		addState("fin_demo", 			new CIntroSubstate(	_appBTA, this, _game, logic::GAME_INTRO, 3));

		addState("intro_modo", 		new CIntroSubstate(	_appBTA, this, _game, logic::MODE_INTRO, 0));
		addState("intro_preguntas", new CIntroSubstate(	_appBTA, this, _game, logic::MODE_INTRO, 1));
		addState("preguntando", 	new CPlayingState(	_appBTA, this, _game));

		addState("pausa", new CPauseSubstate(_appBTA, this, _game));

	}

	void CGameState::onDestroy(){
		//std::cout<<"Destruido estado Juego\n";
		CYaoCompositeController::onDestroy();

		_game->removeListener((logic::CreationDestroyListener*)_sceneManager);
		_game->removeListener((logic::ILogicFacade::GameEventListener*)this);

		_cameraManager->removeListener(this);

		deactivateGfxManagers();

	}

	void CGameState::onActivate(){
		//std::cout<<"Activado estado Juego\n";
		CYaoCompositeController::onActivate();

		_game->init();
		setState("intro");

		//std::cout<<"[CGameState::onActivate] Numero de triangulos: "<<_sceneManager->getTrisInScene()<<"\n";

		//std::cout<<"Fin activado estado Juego\n";
	}

	void CGameState::onDeactivate(){
		//std::cout<<"Desactivado estado Juego\n";
		CYaoCompositeController::onDeactivate();

		_game->finish();
		_audioManager->stop();

	}

	void CGameState::pause(){
		_pause = !_pause;
		_appBTA->pauseTimer();
		_audioManager->pause();
	}

	//-------------------------------------------------------------------------
	// METODOS IMPLEMENTADOS COMO OBSERVADOR DE EVENTOS DEL GESTOR DE CAMARAS
	//-------------------------------------------------------------------------
	void CGameState::emptyCameraQueueEvent(){
		_game->insertCommand(logic::tCommand(logic::END_CAMERAS));
	}

	//-------------------------------------------------------------------------
	// METODOS IMPLEMENTADOS COMO OBSERVADOR DE EVENTOS DE LA PARTIDA
	//-------------------------------------------------------------------------
	void CGameState::changeStateEvent(const std::string& name) {
		//std::cout<<"[CEstadoJuego::changeState] Establecemos el estado "<<name<<"\n";
		setState(name);
	}

	void CGameState::endGameEvent(const logic::ILogicFacade* f){
		_appBTA->setState("menu");
	}

	void CGameState::addCameraEvent(const std::string& camId, const logic::ILogicFacade* facade){
		_cameraManager->addCameraToQueue(camId);
	}

	void CGameState::clearCamerasEvent(){
		_cameraManager->clearCameraQueue();
	}
	//-------------------------------------------------------------------------
	// METODOS PROTEGIDOS
	//-------------------------------------------------------------------------

	void CGameState::executeLogic(float dt){
		if(_pause) return;
		// Actualizamos la logica del juego
		_game->actualize(dt);
	}

	void CGameState::render(float dt){
		if(_pause) dt = 0;

		// Limpiamos la pantalla
		_window->clearScreen();

		_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		_window->swapBuffers();

	}

}
