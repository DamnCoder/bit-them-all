/**
 * CConfigLoadingState.cpp
 *
 *  Created on: 03/09/2010
 *      Author: jorge
 */

#include "ConfigLoading.h"

#include "gui/text/FontsManager.h"

namespace app {

	CConfigLoading::~CConfigLoading() {
	}

	void CConfigLoading::onActivate(){
		CYaoController::onActivate();
		//std::cout<<"Activado subestado ConfigLoading\n";

		// Aqui debería leer el fichero de configuracion y llamar luego a quien tenga que llamar para cargar los recursos

		// Hay que iniciar primero el servidor de fuentes, ya que hay recursos que lo usan
		gui::CFontsCache::instance().init("./data/export/fonts/");

		_parentState->createLoadHud();
	}

	void CConfigLoading::onRun(float dt){
		_parentState->setState("loadResources");
	}

}
