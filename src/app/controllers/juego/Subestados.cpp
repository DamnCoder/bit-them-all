/*
 * SubestadoIntro.cpp
 *
 *  Created on: 26/05/2011
 *      Author: jorge
 */

#include "Subestados.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Label.h"
#include "logic/ui/Button.h"

namespace app {

	void CIntroSubstate::onActivate() {
		//printf("Activado Subestado Intro\n");
		_game->insertCommand(logic::tCommand((logic::EIdCommand)_command, _introId));
	}

	void CIntroSubstate::keyPressed(const event::KeyEvent& ke)
	{
		switch(ke.getKey()){
		case event::KeyEvent::MK_ESCAPE:
			_app->askForTermination();
			//std::cout<<"Solicitado fin\n";
			break;
		case event::KeyEvent::MK_P:
			_parentState->setState("pausa");
			//_parentState->pause();
			//std::cout<<"Pausa\n";
			break;
		default:
			_game->insertCommand(logic::tCommand(logic::END_INTRO));
			break;
		}
	}

//--------------------------------------------------------------------------------------------------------------------

	void CSubestadoPresentacionRonda::keyPressed(const event::KeyEvent& ke)
	{
		switch(ke.getKey()){
		case event::KeyEvent::MK_ESCAPE:
			_app->askForTermination();
			//std::cout<<"Solicitado fin\n";
			break;
		case event::KeyEvent::MK_P:
			_parentState->setState("pausa");
			//_parentState->pause();
			//std::cout<<"Pausa\n";
			break;
		default:
			_game->insertCommand(logic::tCommand(logic::END_PRESENTATION));
			break;
		}
	}

//--------------------------------------------------------------------------------------------------------------------

	void CPlayingState::onActivate() {
		//printf("Activado Subestado Jugando\n");
		_game->insertCommand(logic::tCommand(logic::INIT_QUESTIONS));
	}

	void CPlayingState::keyPressed(const event::KeyEvent& ke)
	{
		switch(ke.getKey()){
		case event::KeyEvent::MK_ESCAPE:
			_app->askForTermination();
			//std::cout<<"Solicitado fin\n";
			break;
		case event::KeyEvent::MK_P:
			//_parentState->pause();
			_parentState->setState("pausa");
			//std::cout<<"Pausa\n";
			break;
		case event::KeyEvent::MK_A:
			_game->insertCommand(logic::tCommand(logic::P1_A));
			break;
		case event::KeyEvent::MK_S:
			_game->insertCommand(logic::tCommand(logic::P1_B));
			break;
		case event::KeyEvent::MK_D:
			_game->insertCommand(logic::tCommand(logic::P1_C));
			break;
		case event::KeyEvent::MK_F:
			_game->insertCommand(logic::tCommand(logic::P1_D));
			break;
		default:
			break;
		}
	}

//--------------------------------------------------------------------------------------------------------------------

	void CPauseSubstate::onCreate() {
		CGameSubstate::onCreate();
		_sceneManager = _parentState->sceneManager();

		//createPauseGui();
	}

	void CPauseSubstate::onDestroy() {
		//_fPauseGui->removeListener((event::MouseListener*) this);
		//_inputManager->removeListener(_fPauseGui);

		//destroyPauseGui();
		//deactivateGfxManagers();

		CGameSubstate::onDestroy();
	}

	void CPauseSubstate::onActivate() {
		//printf("Activado Subestado Pausa\n");
		_parentState->pause();
		//_fPauseGui->setVisible(true);
		//_audioManager->playSong(0, true);
	}

	void CPauseSubstate::onDeactivate() {
		//printf("Desactivado Subestado Pausa\n");
		_parentState->pause();
		//_fPauseGui->setVisible(false);
		//_audioManager->stop();
	}

	void CPauseSubstate::keyPressed(const event::KeyEvent& ke)
	{
		switch(ke.getKey()){
		case event::KeyEvent::MK_ESCAPE:
			_app->askForTermination();
			//std::cout<<"Solicitado fin\n";
			break;
		case event::KeyEvent::MK_P:
			_parentState->setPreviousState();
			//std::cout<<"Pausa\n";
			break;

		case event::KeyEvent::MK_UP:
			_fPauseGui->setDepth(_fPauseGui->depth()+1);
			//std::cout<<"[CPauseSubstate::keyPressed] La profundidad es: "<<_fPauseGui->depth()<<"\n";
			break;
		case event::KeyEvent::MK_DOWN:
			_fPauseGui->setDepth(_fPauseGui->depth()-1);
			//std::cout<<"[CPauseSubstate::keyPressed] La profundidad es: "<<_fPauseGui->depth()<<"\n";
			break;
		default:
			break;
		}
	}

	void CPauseSubstate::mouseReleased(const event::MouseEvent& me) {
		const logic::CUiComponent* source = me.getSource();

		if(source == _bReturnToGame){
			_parentState->setPreviousState();
		}
		else if(source == _bReturnToMenu) {
			_appBTA->setState("menu");
		}
	}

	void CPauseSubstate::createPauseGui() {
		float scrwidth = _appBTA->getWindow()->width();
		float scrheight = _appBTA->getWindow()->height();

		float xPos = scrwidth / 32.0f;
		float yPos = scrheight / 32.0f;

		_fPauseGui = new logic::CFrame(0, 0, scrwidth, scrheight);
		_fPauseGui->setID("menu_background");
		//_fPauseGui->setDepth(2);

		_bReturnToGame = new logic::CButton("Seguir jugando", 0, 0, 256, 64);
		_bReturnToGame->setStyle("dark_blue");
		_bReturnToGame->setPosition(12*xPos, 13*yPos);


		_bReturnToMenu = new logic::CButton("Volver al menu", 0, 0, 256, 64);
		_bReturnToMenu->setStyle("dark_blue");
		_bReturnToMenu->setPosition(12*xPos, 10*yPos);

		_fPauseGui->add(_bReturnToGame);
		_fPauseGui->add(_bReturnToMenu);

		_fPauseGui->setVisible(false);

		_sceneManager->addGfxComponent(_fPauseGui);

		_fPauseGui->addListener((event::MouseListener*) this);
		_inputManager->addListener(_fPauseGui);

	}

	void CPauseSubstate::destroyPauseGui() {
		_sceneManager->removeGfxComponent(_fPauseGui);

		safeDelete(_fPauseGui);
	}
}
