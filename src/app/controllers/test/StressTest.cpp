/*
 * ShadowTest.cpp
 *
 *  Created on: 29/12/2010
 *      Author: jorge
 */

#include "StressTest.h"

#include "gui/graphicserver/OglCamera.h"
#include "gui/graphicserver/OglLight.h"

#include "logic/entitys/Player.h"
#include "logic/entitys/Entity.h"
#include "logic/ui/Label.h"

namespace app {

	CStressTest::CStressTest(CBitThemAllApp* app):
		CYaoController(app),
		_cam(0),
		_light(0),
		_player(0),
		_floor(0){
	}

	CStressTest::~CStressTest() {
	}

	void CStressTest::onCreate(){
		//std::cout<<"Creado estado Test para sombras\n";
		CYaoController::onCreate();
		activateGfxManagers();

		// Creamos una camara simple
		//_cam = _graphicserver->newCamera(math::CVector3f(150.0f, 100.0f, 0.0f), gui::CAM_PROJECTION);
		//_cam->setFarView(1500);
		//_cam->setAspectRatioCamera();

		// Creacion de una luz posicional
		float value = 0.4f;
		float w = 1.0f;

		lightPosition = math::Vec4(0.0f, 80.0f, 0.0f, 1.0f);
		math::Vec4 ambient(value, value, value, w);
		math::Vec4 diffuse(value, value, value, w);
		math::Vec4 specular(value, value, value, w);
		_light = new gui::COglLight(gui::COglLight::AGA_POSITIONAL, gui::COglLight::LIGHT_0, lightPosition);
		_light->switchLight(true);
		_light->setAmbientComponent(ambient);
		_light->setDiffuseComponent(diffuse);
		_light->setSpecularComponent(specular);
		_light->actualize();
		_sceneManager->addLight(_light);

		// Creamos los elementos de la escena
		_floor = new logic::CEntity("Suelo",0.0f, 0.0f, 0.0f, 0, true);

		_entitys.push_back(_floor);

		vector<logic::CEntity*>::iterator it(_entitys.begin()), end(_entitys.end());
		for( ; it!=end; ++it)
			_sceneManager->addGfxComponent(*it);

		_player = new logic::CPlayer(
						"player", "jugador",
						0, 22, 0,
						0, true, 10);

		_player->setSkin("stormtrooper");
		_player->setOrientationOffset(270.0f);
		_players.push_back(_player);
		_sceneManager->addGfxComponent(_player);

		createArmy();

		std::string trisText =  "Triangulos en escena: ";
		trisText += data2string(_sceneManager->getTrisInScene());

		_lSceneTris = new logic::CLabel(20, 20, trisText);
		_sceneManager->addGfxComponent(_lSceneTris);

		//cout<<"TRIANGULOS EN LA ESCENA: "<<_3dManager->getTrisInScene()*2<<"\n";

	}

	void CStressTest::onDestroy(){
		//std::cout<<"Destruido estado Test para sombras\n";
		CYaoController::onDestroy();

		vector<logic::CPlayer*>::const_iterator it(_players.begin()), end(_players.end());
		for(; it!=end; ++it)
			_sceneManager->removeGfxComponent(*it);

		vector<logic::CEntity*>::iterator iten(_entitys.begin()), enden(_entitys.end());
		for( ; iten!=enden; ++iten){
			_sceneManager->removeGfxComponent(*iten);
		}

		safeDelete(_light);

		deleteVector(_entitys);
		deleteVector(_players);

		safeDelete(_cam);

		deactivateGfxManagers();
	}

	void CStressTest::executeLogic(float dt){
		if(!_player) return;
		_player->actualize(dt);
	}

	void CStressTest::render(float dt){
		// Limpiamos la pantalla
		gui::CWindowManager::instance().clearScreen();

		_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		gui::CWindowManager::instance().swapBuffers();
	}


	void CStressTest::keyPressed(const event::KeyEvent& ke){
		float step = 3.0f;

		switch(ke.getKey()){

		case event::KeyEvent::MK_LEFT:
			_player->turnLeft();
			break;

		case event::KeyEvent::MK_RIGHT:
			_player->turnRight();
			break;

		case event::KeyEvent::MK_UP:
			_player->moveFrontwards();
			break;

		case event::KeyEvent::MK_DOWN:
			_player->moveBackwards();
			break;

		case event::KeyEvent::MK_W:
		{
			lightPosition.x -= step;
			_light->setPosition(lightPosition);
		}
			break;
		case event::KeyEvent::MK_S:
		{
			lightPosition.x += step;
			_light->setPosition(lightPosition);
		}
			break;
		case event::KeyEvent::MK_A:
		{
			lightPosition.z += step;
			_light->setPosition(lightPosition);
		}
			break;
		case event::KeyEvent::MK_D:
		{
			lightPosition.z -= step;
			_light->setPosition(lightPosition);
		}
			break;

		case event::KeyEvent::MK_I:
		{
			math::CVector3f camPos = _cam->getPosition();
			camPos.y += step;
			_cam->setPosition(camPos);
		}
			break;
		case event::KeyEvent::MK_K:
		{
			math::CVector3f camPos = _cam->getPosition();
			camPos.y -= step;
			_cam->setPosition(camPos);
		}
			break;
		default:
			break;
		}
	}

	void CStressTest::keyReleased(const event::KeyEvent& ke){
		switch(ke.getKey()){

		case event::KeyEvent::MK_LEFT:
		case event::KeyEvent::MK_RIGHT:
			_player->stopTurning();
			break;

		case event::KeyEvent::MK_UP:
		case event::KeyEvent::MK_DOWN:
			_player->stopped();
			break;

		default:
			break;
		}
	}

	void CStressTest::createArmy(){
		float separation = 50.0f;
		float x = 0.0f;
		float z = 0.0f;

		//int num = 15;
		int num = 4;

		for(int j = -num; j<num; ++j)
			for (int i = -num; i < num ; ++i) {
				x = i * separation;
				z = j * separation;
/*
				float z = -separation;
				switch(j){
				case 1:
					z = 0.0f;
					break;
				case 2:
					z = separation;
					break;
				}
*/

//*
				logic::CPlayer* player = new logic::CPlayer(
								"host", "jugador",
								x, 22, z,
								180, true, 10);
//*/

/*
				logic::CPlayer* player = new logic::CPlayer(
								"player", "jugador",
								x, 22, z,
								180, true, 10);
*/
				//logic::CPlayer* player = new logic::CPlayer("cloud", "Jorge", x, 22, z, 0, true);
				//player->setShadow(true);
				_players.push_back(player);
			}

		vector<logic::CPlayer*>::const_iterator it(_players.begin()), end(_players.end());
		for(; it!=end; ++it)
			_sceneManager->addGfxComponent(*it);
	}
}
