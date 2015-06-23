/*
 * LoadState.cpp
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#include "LoadState.h"

#include "app/BitThemAllApp.h"

#include "logic/ui/Label.h"

#include "ConfigLoading.h"
#include "ResourceLoading.h"
#include "LogicCreation.h"

namespace app {

	CLoadState::CLoadState(CBitThemAllApp* app):
				CBTACompositeController(app),
			_loadingLabel(0),
			_resourceTypeLabel(0),
			_heightOkLabel(0){}

	CLoadState::~CLoadState() {
	}

	void CLoadState::onCreate(){
		//cout<<"Creado estado contenedor Load\n";
		CYaoCompositeController::onCreate();

		// Activamos los gestores de representacion grafica
		activateGfxManagers();

		// Iniciamos las variables
		_scrwidth = (float)_appBTA->getWindow()->width();
		_scrheight = (float)_appBTA->getWindow()->height();

		_dividedWidth = _scrwidth / 7.0f;
		_dividedHeight = _scrheight / 5.0f;

		_heightOkLabel = 8;

		// Creamos los subestados
		addState("loadConfig", new CConfigLoading(_appBTA,this));
		addState("loadResources", new CResourceLoading(_appBTA,this));
		addState("logicCreation", new CLogicCreation(_appBTA,this));

		setState("loadConfig");

	}

	void CLoadState::onDestroy(){
		//cout<<"Destruido estado contenedor Load\n";
		CYaoCompositeController::onDestroy();

		destroyLoadHud();
		deactivateGfxManagers();

	}

	void CLoadState::createLoadHud(){

		_loadingLabel = new logic::CLabel(3*_dividedWidth, _dividedHeight*4, "LOADING...");
		_resourceTypeLabel = new logic::CLabel(4*_dividedWidth, _dividedHeight*4 , "");

		_loadingLabel->setTextSize(25);
		_resourceTypeLabel->setTextSize(25);

		_labels.push_back(_loadingLabel);
		_labels.push_back(_resourceTypeLabel);

		_sceneManager->addGfxComponent(_loadingLabel);
		_sceneManager->addGfxComponent(_resourceTypeLabel);
	}

	void CLoadState::destroyLoadHud(){
		std::vector<logic::CLabel*>::iterator
			it(_labels.begin()), end(_labels.end());

		for(; it!=end; ++it){
			_sceneManager->removeGfxComponent(*it);
			safeDelete(*it);
		}
	}

	void CLoadState::resourceTypeInLoad(const std::string& resourceType){
		_resourceTypeLabel->setText(resourceType);
	}

	void CLoadState::addLoadFinishLabel(const std::string& text){

		float heightSepa = (_dividedHeight*4.0f) / 9.0f;

		std::string loadOktext = text;

		logic::CLabel* label = new logic::CLabel(3*_dividedWidth, _heightOkLabel*heightSepa, loadOktext);
		logic::CLabel* oklabel = new logic::CLabel(4.4f*_dividedWidth, _heightOkLabel*heightSepa, "OK");
		label->setTextSize(15);
		oklabel->setTextSize(15);


		_labels.push_back(label);
		_labels.push_back(oklabel);

		_sceneManager->addGfxComponent(label);
		_sceneManager->addGfxComponent(oklabel);
		--_heightOkLabel;

	}

	void CLoadState::render(float dt){
		// Limpiamos la pantalla
		gui::CWindowManager::instance().clearScreen();

		_sceneManager->render(dt);

		// Intercambiamos los buffers de video
		gui::CWindowManager::instance().swapBuffers();
	}

}
