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

#include "MenuState.h"

#include "app/controllers/StateHandler.h"

#include "gui/OglGraphics.h"
#include "gui/graphicserver/OglLight.h"

#include "PrincipalMenuSubstate.h"
#include "CharacterSelectionSubstate.h"
#include "ScenarioSelectionSubstate.h"

#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"

namespace app {

	CMenuState::CMenuState(CBitThemAllApp* app, ILogicFacade* f) :
			CBTACompositeController(app),
			_game(f),
			_scenarioBackground(0),
			_renderHudFirt(false),
			_selectedPlayers(false),
			_selectedScenario(false)
	{}

	CMenuState::~CMenuState()
	{}

	void CMenuState::onCreate(){
		//std::cout<<"Creado estado contenedor Menu\n";
		CYaoCompositeController::onCreate();

		activateGfxManagers();

		//createCamera();
		createLight();

		// Solicitamos los escenarios, para usarlos como fondo del menu principal
		// y los personajes para el menu de seleccion
		_scenarioManager = _appBTA->getScenarioManager();
		_playerManager = _appBTA->getPlayerManager();

		_scenarioBackground = _scenarioManager->getRandom();

		createAnimatedEntitys();
		createScenarios();

		// Creacion de los subestados del menu
		addState("menuPrincipal",new CPrincipalMenuSubstate(_appBTA, this, _game));
		addState("character_selection", new CCharacterSelectionSubstate(_appBTA, this, _game));
		addState("scenario_selection", new CScenarioSelectionSubstate(_appBTA, this, _game));
		setState("menuPrincipal");

	}

	void CMenuState::onDestroy(){
		//std::cout<<"Destruido estado contenedor Menu\n";

		CYaoCompositeController::onDestroy();

		safeDelete(_light);

		//cout<<"[CMenuState::onDestroy] Elimino graficos escenario\n";
		// Destruimos todos los escenarios
		_scenarioIt = _scenarios.begin();
		_scenarioEnd = _scenarios.end();
		for(; _scenarioIt != _scenarioEnd; ++_scenarioIt)
			_sceneManager->removeGfxComponent(*_scenarioIt);

		// Destruimos los jugadores
		_playerIt = _players.begin();
		_playerEnd = _players.end();
		for( ; _playerIt != _playerEnd; ++_playerIt){
			_sceneManager->removeGfxComponent((CAvatar*)(*_playerIt));
		}
		deactivateGfxManagers();
	}

	void CMenuState::onActivate(){
		CYaoCompositeController::onActivate();
		_audioManager->playSong(0, true);
	}

	void CMenuState::onDeactivate(){
		CYaoCompositeController::onDeactivate();
		_audioManager->stop();
	}

	void CMenuState::render(float dt){
		// Limpiamos la pantalla
		_appBTA->getWindow()->clearScreen();

		if(_renderHudFirt)
			_sceneManager->render2dFirst(dt);
		else
			_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		_appBTA->getWindow()->swapBuffers();
	}

	void CMenuState::createCamera(){
		_cameraManager->addCameraToQueue("surround_cam");
	}

	void CMenuState::createLight(){
		// Creacion de una luz posicional
		float value = 0.8f;
		float w = 0.2f;

		_light = new gui::COglLight(
				gui::COglLight::AGA_POSITIONAL,
				gui::COglLight::LIGHT_0,
				math::Vec4(0, 40, 0, 1));

		_light->switchLight(true);
		_light->setAmbientComponent(math::Vec4(value, value, value, w));
		_light->setDiffuseComponent(math::Vec4(value, value, value, w));
		_light->setSpecularComponent(math::Vec4(value, value, value, w));
		_light->actualize();

		_sceneManager->addLight(_light);
		gui::COglFunctions::instance().setSceneManager(_sceneManager);
	}

	void CMenuState::createAnimatedEntitys(){
		// Creamos los graficos de todos los personajes seleccionables, pero con la visibilidad a false
		_players = _playerManager->getAll();
		_playerIt = _players.begin();
		_playerEnd = _players.end();
		for( ; _playerIt != _playerEnd; ++_playerIt){
			_sceneManager->addGfxComponent((CAvatar*)(*_playerIt));
			//cout<<"[CMenuState::createAnimatedEntitys] "<<(*_playerIt)->getId()<<"\n";
			(*_playerIt)->setVisible(false);
		}
	}

	void CMenuState::createScenarios(){
		// Creamos los graficos de todos los escenarios, pero con la visibilidad a false
		_scenarios = _scenarioManager->getAll();
		_scenarioIt = _scenarios.begin();
		_scenarioEnd = _scenarios.end();
		for(; _scenarioIt != _scenarioEnd; ++_scenarioIt){
			_sceneManager->addGfxComponent((*_scenarioIt));
			(*_scenarioIt)->setVisible(false);
		}
	}
}
