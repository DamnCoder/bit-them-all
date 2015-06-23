/*
 * ScenarioSelectionSubstate.cpp
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#include "ScenarioSelectionSubstate.h"

#include "logic/entitys/Player.h"
#include "logic/entitys/Entity.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Scenario.h"

#include "logic/ui/Button.h"
#include "logic/ui/Frame.h"

#include "gui/graphicserver/OglCamera.h"

#include "persist/DaoServer.h"

namespace app {

	CScenarioSelectionSubstate::~CScenarioSelectionSubstate() {
	}

	void CScenarioSelectionSubstate::onCreate(){
		CMenuSubstate::onCreate();
		//cout<<"Creado subestado ScenarioSelection\n";

		// Recuperamos todos los escenarios
		_scenarioManager = _appBTA->getScenarioManager();

		_sceneManager = _parentState->sceneManager();

		createScenarioHud();
	}

	void CScenarioSelectionSubstate::onDestroy(){
		//cout<<"Destruido subestado ScenarioSelection\n";

		_sceneManager->removeGfxComponent(_scenarioFrame);
		safeDelete(_scenarioFrame);

		CMenuSubstate::onDestroy();
	}

	void CScenarioSelectionSubstate::onActivate(){
		//cout<<"Activado subestado ScenarioSelection\n";

		_scenarioBackground =_scenarioManager->getSelected();
		_scenarioBackground->setVisible(true);

		_acceptButton->setVisible(true);
	}

	void CScenarioSelectionSubstate::onDeactivate(){
		//cout<<"Desactivado subestado ScenarioSelection\n";

		_acceptButton->setVisible(false);
	}

	void CScenarioSelectionSubstate::keyReleased(const event::KeyEvent& ke){
		switch(ke.getKey()){

		case event::KeyEvent::MK_LEFT:
			previousScenario();
			break;

		case event::KeyEvent::MK_RIGHT:
			nextScenario();
			break;

		default:
			break;
		}
	}

	void CScenarioSelectionSubstate::mouseReleased(const event::MouseEvent& me){
		const logic::CUiComponent* source = me.getSource();

		if(source == _acceptButton){
			_parentState->setState("menuPrincipal");
			_parentState->setBackgroundScenario(_scenarioBackground);
		}
	}

	void CScenarioSelectionSubstate::nextScenario(){
		//cout<<"Siguiente escenario\n";
		_scenarioBackground->setVisible(false);

		_scenarioBackground = _scenarioManager->getNext();
		_scenarioBackground->setVisible(true);
	}

	void CScenarioSelectionSubstate::previousScenario(){
		//cout<<"Anterior escenario\n";
		_scenarioBackground->setVisible(false);

		_scenarioBackground = _scenarioManager->getPrevious();
		_scenarioBackground->setVisible(true);
	}

	void CScenarioSelectionSubstate::createScenarioHud(){
		_scenarioFrame = new logic::CFrame(
				0, 0,
				_appBTA->getWindow()->width(),
				_appBTA->getWindow()->height());

		_acceptButton = new logic::CButton("Seleccionar", 100, 100, 210, 70);
		_acceptButton->setStyle("webstyle_aqua");
		_acceptButton->setVisible(false);

		_scenarioFrame->add(_acceptButton);

		_sceneManager->addGfxComponent(_scenarioFrame);

		_scenarioFrame->addListener((event::MouseListener*) this);
		_inputManager->addListener(_scenarioFrame);

	}
}
