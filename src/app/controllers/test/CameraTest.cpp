/*
 * CameraTest.cpp
 *
 *  Created on: 19/02/2011
 *      Author: jorge
 */

#include "CameraTest.h"

#include "gui/cam/StaticCamera.h"

#include "logic/entitys/Entity.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Screen.h"

namespace app {

	CCameraTest::CCameraTest(CBitThemAllApp* app):
			CYaoController(app),
			_host(0),
			_player(0)

	{
	}

	CCameraTest::~CCameraTest()
	{
	}

	void CCameraTest::onCreate(){
		CYaoController::onCreate();
		activateGfxManagers();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Creamos una camara simple
		_cameraManager->addCamera(
				"estaticaPresentador",
				new gui::CStaticCamera(
						"estaticaPresentador",
						math::CVector3f(100, 50, 0),
						math::CVector3f(0, 0, 0)));

		_cameraManager->addCameraToQueue("estaticaPresentador");


		_player = new logic::CPlayer("player", "jugador",
				10, 0, 30,
				90, true, 10);

		_player->setSkin("stormtrooper");

		_host = new logic::CPlayer("host", "presentador",
				10, 0, -30,
				90, true, 10);

		_animatedEntitys.push_back(_player);
		_animatedEntitys.push_back(_host);

		_sceneManager->addGfxComponent(_host);
		_sceneManager->addGfxComponent(_player);

		_screenRotation = 0;
	}

	void CCameraTest::onDestroy(){
		CYaoController::onDestroy();

		//std::cout<<"[CCameraTest::onDestroy] Destruido estado camera test.\n";
/*
		vector<logic::CEntity*>::iterator iten(_entitys.begin()), enden(_entitys.end());
		for( ; iten!=enden; ++iten){
			_3dManager->destroyStaticEntity(*iten);
		}

		_3dManager->destroyScreen(_screen);
*/
		_sceneManager->removeGfxComponent(_host);
		_sceneManager->removeGfxComponent(_player);

		delete _player;
		delete _host;

		deactivateGfxManagers();
	}

	void CCameraTest::keyReleased(const event::KeyEvent& ke){

		switch(ke.getKey()){

		case event::KeyEvent::MK_A:
			break;
		case event::KeyEvent::MK_S:
			break;
		case event::KeyEvent::MK_D:
			break;
		case event::KeyEvent::MK_F:
			break;
		default:
			break;
		}
	}

	void CCameraTest::executeLogic(float dt){
		if(!_player) return;
		_player->actualize(dt);
	}

	void CCameraTest::render(float dt){
		//_screenRotation += 0.1f * dt;
		//_screen->setOrientation(_screenRotation);

		// Limpiamos la pantalla
		gui::CWindowManager::instance().clearScreen();

		_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		gui::CWindowManager::instance().swapBuffers();

	}

}
