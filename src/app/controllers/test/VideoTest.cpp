/*
 * CVideoTest.cpp
 *
 *  Created on: 19/08/2011
 *      Author: jorge
 */

#include "VideoTest.h"

#include "gui/cam/StaticCamera.h"

#include "logic/entitys/Screen.h"
#include "logic/entitys/Player.h"

namespace app {

	CVideoTest::CVideoTest(CBitThemAllApp* app):
		CYaoController(app),
		_screen(0),
		_rotation(0)
	{
	}

	CVideoTest::~CVideoTest() {
	}

	void CVideoTest::onCreate() {
		CYaoController::onCreate();
		activateGfxManagers();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		const char* camName = "estaticaPantalla";

		// Creamos una camara simple
		_cameraManager->addCamera(
				camName,
				new gui::CStaticCamera(
						camName,
						math::CVector3f(170, 40, 0),
						math::CVector3f(0, 10, 0)));

		_cameraManager->addCameraToQueue(camName);

		_pos = 0;
		_step = 0.1f;

		// Creamos los elementos de la escena
		_floor = new logic::CEntity("Suelo",
				0.0f, -80.0f, 0.0f,
				0, true);

		// Creamos la pantalla
		_screen = new logic::CScreen("screen",
				-50, 50, 0,
				270, true);

		// Creamos los personajes
		_player1 = new logic::CPlayer(
				"player", "jugador",
				0, -70, 30,
				270, true, 10);
		_player2 = new logic::CPlayer(
				"player", "jugador",
				0, -70, -30,
				270, true, 10);

		_player1->setOrientationOffset(270.0f);
		_player2->setOrientationOffset(270.0f);

		_player1->setSkin("chico_refugio");
		_player2->setSkin("scot_pilgrim");

		_player2->setLegsAnimation("LEGS_NERVOUS");
		_player2->setTorsoAnimation("TORSO_APPLAUSE");

		_player1->setTorsoAnimation("TORSO_SWINGING");

		this->_sceneManager->addGfxComponent(_floor);
		this->_sceneManager->addGfxComponent(_screen);
		this->_sceneManager->addGfxComponent(_player1);
		this->_sceneManager->addGfxComponent(_player2);

		//std::cout<<"[CVideoTest::onCreate] Creado estado video test.\n";
	}

	void CVideoTest::onDestroy() {
		CYaoController::onDestroy();
		this->_sceneManager->removeGfxComponent(_floor);
		this->_sceneManager->removeGfxComponent(_player1);
		this->_sceneManager->removeGfxComponent(_player2);
		this->_sceneManager->removeGfxComponent(_screen);
		safeDelete(_floor);
		safeDelete(_screen);
		safeDelete(_player1);
		safeDelete(_player2);

		deactivateGfxManagers();

		//std::cout<<"[CVideoTest::onDestroy] Destruido estado video test.\n";
	}

	void CVideoTest::onActivate() {
		CYaoController::onActivate();
		_screen->playVideo("intro.mpg");
		_screen->setSkin("default");//
		//std::cout<<"[CVideoTest::onActivate] Activado estado video test.\n";
	}

	void CVideoTest::onDeactivate() {
		CYaoController::onDeactivate();
		_screen->stopVideo();
		//std::cout<<"[CVideoTest::onDeactivate] Desactivado estado video test.\n";
	}

	void CVideoTest::keyReleased(const event::KeyEvent& ke){

		switch(ke.getKey()){
		case event::KeyEvent::MK_P:
			_screen->pauseVideo();
			break;
		case event::KeyEvent::MK_S:
			_screen->stopVideo();
			break;
		case event::KeyEvent::MK_SPACE:
			_screen->playVideo("intro.mpg");
			break;

		case event::KeyEvent::MK_LEFT:
		case event::KeyEvent::MK_RIGHT:
			_player1->stopTurning();
			break;

		case event::KeyEvent::MK_UP:
		case event::KeyEvent::MK_DOWN:
			_player1->stopped();
			_player1->setLegsAnimation("LEGS_STILL");
			_player1->setTorsoAnimation("TORSO_SWINGING");
			break;

		default:
			_screen->pauseVideo();
			break;
		}
	}

	void CVideoTest::keyPressed(const event::KeyEvent& ke){
		switch(ke.getKey()){

		case event::KeyEvent::MK_LEFT:
			_player1->turnLeft();
			break;

		case event::KeyEvent::MK_RIGHT:
			_player1->turnRight();
			break;

		case event::KeyEvent::MK_UP:
			_player1->moveFrontwards();
			_player1->setLegsAnimation("LEGS_SWINGING");
			_player1->setTorsoAnimation("TORSO_HARD_SWINGING");
			break;

		case event::KeyEvent::MK_DOWN:
			_player1->moveBackwards();
			_player1->setLegsAnimation("LEGS_NERVOUS");
			_player1->setTorsoAnimation("TORSO_HARD_SWINGING");
			break;
		default:
			break;
		}
	}

	void CVideoTest::executeLogic(float dt){
		if(!_screen || !_player1) return;

		_player1->actualize(dt);
/*
		if(_pos >20)
			_pos += _step;
		else
			_pos -= _step;

		_screen->setPosition(0, 0, _pos);
*/
	}

	void CVideoTest::render(float dt){
		// Limpiamos la pantalla
		gui::CWindowManager::instance().clearScreen();

		_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		gui::CWindowManager::instance().swapBuffers();

	}

}
