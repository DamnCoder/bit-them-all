/*
 * PrincipalMenu.cpp
 *
 *  Created on: 05/10/2010
 *      Author: jorge
 */

#include "PrincipalMenuSubstate.h"
#include "MenuState.h"

#include "gui/graphicserver/OglCamera.h"

#include "logic/ui/Button.h"
#include "logic/ui/Frame.h"
#include "logic/entitys/Entity.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Scenario.h"

namespace app {

	CPrincipalMenuSubstate::~CPrincipalMenuSubstate() {
	}

	void CPrincipalMenuSubstate::onCreate(){
		CMenuSubstate::onCreate();
		//std::cout<<"Creado subestado Menu Principal!\n";

		_sceneManager = _parentState->sceneManager();
		createPrincipalMenuHud();

		_backgroundScenario = _parentState->getBackgroundScenario();

		setRandomAudience();

	}

	void CPrincipalMenuSubstate::onDestroy(){
		//std::cout<<"Destruido subestado Menu Principal!\n";

		_menuFrame->removeListener((event::MouseListener*) this);
		_inputManager->removeListener(_menuFrame);

		_sceneManager->removeGfxComponent(_menuFrame);
		safeDelete(_menuFrame);

		CMenuSubstate::onDestroy();

	}

	void CPrincipalMenuSubstate::onActivate(){
		CMenuSubstate::onActivate();
		//std::cout<<"Activado subestado Menu Principal!\n";

		_logic->setScenario(_backgroundScenario);
		_backgroundScenario->setVisible(true);

		// Hacemos visibles los botones
		_menuFrame->setVisible(true);

	}

	void CPrincipalMenuSubstate::onDeactivate(){
		CMenuSubstate::onDeactivate();
		//std::cout<<"Desactivado subestado Menu Principal!\n";

		_backgroundScenario->setVisible(false);

		// Hacemos invisibles los botones
		_menuFrame->setVisible(false);
	}

	void CPrincipalMenuSubstate::mouseReleased(const event::MouseEvent& me){
		const logic::CUiComponent* source = me.getSource();

		if(source == _endAppBtn){
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Salimos\n";
			_app->askForTermination();
		}
		else if(source == _initGameBtn){

			_logic->setScenario(_backgroundScenario);

			bool selected = _parentState->arePlayersSelected();
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Jugadores al azar "<<(selected ? "true" : "false")<<"\n";
			// Si no se ha elegido seleccionar jugadores, se selecciona al azar
			if(!_parentState->arePlayersSelected()){
				//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Jugadores al azar "<<(selected ? "true" : "false")<<"\n";
				setRandomPlayers();
			}

			//_app->setState("game");
			_app->setState("juego");
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Iniciamos juego\n";
		}
		else if(source == _menuScenaryBtn){
			_parentState->setState("scenario_selection");
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Menu escenario\n";
		}
		else if(source == _menuCharacterBtn){
			_parentState->setState("character_selection");
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Menu de personajes\n";
		}
		else if(source == _menuTestBtn){
			_appBTA->setState("stress");
			//_appBTA->setState("video");
			//std::cout<<"[CPrincipalMenuSubstate::mouseReleased] Menu de test\n";
		}
	 }

	void CPrincipalMenuSubstate::keyReleased(const event::KeyEvent& ke){
		switch(ke.getKey()){
		case event::KeyEvent::MK_P:
			_appBTA->pauseTimer();
			//_audioManager->pause();
			break;
		default:
			break;
		}
	}

	void CPrincipalMenuSubstate::executeLogic(float dt){
		//float rad = dt * 0.0003f;
		//_parentState->getCamera()->rotate(rad, 0,0,0);
	}

	void CPrincipalMenuSubstate::createPrincipalMenuHud(){
		int scrwidth = _appBTA->getWindow()->width();
		int scrheight = _appBTA->getWindow()->height();

		_menuFrame = new logic::CFrame(0, 0, scrwidth, scrheight);

		_initGameBtn = new logic::CButton("Inicio Juego", 400, 600, 210, 70);
		_menuTestBtn = new logic::CButton("Test", 400, 500, 210, 70);
		_menuScenaryBtn = new logic::CButton("Elige Escenario", 400, 400, 210, 70);
		_menuCharacterBtn = new logic::CButton("Elige Personaje", 400, 300, 210, 70);
		_endAppBtn = new logic::CButton("Fin Juego", 400, 200, 210, 70);

		_initGameBtn->setStyle("webstyle_aqua");
		_menuTestBtn->setStyle("webstyle_aqua");
		_menuScenaryBtn->setStyle("webstyle_aqua");
		_menuCharacterBtn->setStyle("webstyle_aqua");
		_endAppBtn->setStyle("webstyle_aqua");

		_menuFrame->add(_initGameBtn);
		_menuFrame->add(_menuTestBtn);
		_menuFrame->add(_menuScenaryBtn);
		_menuFrame->add(_menuCharacterBtn);
		_menuFrame->add(_endAppBtn);

		_menuFrame->setVisible(false);

		_menuFrame->addListener((event::MouseListener*) this);
		_inputManager->addListener(_menuFrame);

		_sceneManager->addGfxComponent(_menuFrame);
	}

	void CPrincipalMenuSubstate::setRandomPlayers(){
		uint numChars = _parentState->getSelectionablePlayers().size();
		std::vector<int> randomNum = getNonRepeatedRandomNumbers(4, numChars);
		for(uint i=0; i<4; ++i){
			logic::CPlayer* player = _parentState->getSelectionablePlayers().at(randomNum[i]);
			string id = player->id();
			string skin = player->skin();
			string name = player->name();
			int scale = player->scale();
			//cout<<"Personaje "<<randomNum[i]<<" con id: "<<id<<"\n";
			_logic->setPlayerAppearance(i, id, skin, name, scale);
		}
	}

	void CPrincipalMenuSubstate::setRandomAudience(){
		uint numChars = _parentState->getSelectionablePlayers().size();
		uint numAudience = _backgroundScenario->audience().size();
		std::vector<int> randomNum = getRandomNumbers(numAudience, numChars);

		//cout<<"[CPrincipalMenuSubstate::setRandomAudience] Tenemos de publico "<<numAudience<<" personajes a aleatorizar de entre "<<numChars<<"\n";
		for(uint i=0; i<numAudience; ++i){
			logic::CAvatar* audience = _parentState->getSelectionablePlayers().at(randomNum[i]);
			string id = audience->id();
			string skin = audience->skin();
			int scale = audience->scale();
			//cout<<"[CPrincipalMenuSubstate::setRandomAudience] Personaje "<<randomNum[i]<<" con skin: "<<skin<<"\n";
			_backgroundScenario->setAudienceAppearance(i, id, skin, scale);
		}
	}

}
