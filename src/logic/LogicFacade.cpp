/**
 * @file LogicFacade.h
 * Contiene la interfaz que han de implementar las clases que sirvan de
 * fachada de comunicación entre lógica y aplicación/GUI
 *
 * @see logic::ILogicFacade
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */

#include "LogicFacade.h"

#include "Question.h"

#include "logic/entitys/MultimediaFurniture.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Scenario.h"
#include "logic/Question.h"

namespace logic {

	ILogicFacade::ILogicFacade():
		_init(false),
		_playing(false){
	}

	ILogicFacade::~ILogicFacade() {
	}

	//--------------------------------------------------------------------------------------
	// METODOS DE ADICION / ELIMINACION DE OBSERVADORES
	//--------------------------------------------------------------------------------------

	bool ILogicFacade::addListener(CreationDestroyListener* newob){
		_creationDestroyListeners.push_back(newob);
		return true;
	}
	bool ILogicFacade::removeListener(CreationDestroyListener *oldob){
		_creationDestroyListeners.remove(oldob);
		return true;
	}

	bool ILogicFacade::addListener(GameEventListener* newob){
		_gameEventListeners.push_back(newob);
		return true;
	}
	bool ILogicFacade::removeListener(GameEventListener* oldob){
		_gameEventListeners.remove(oldob);
		return true;
	}

	//--------------------------------------------------------------------------------------
	// METODOS DE CREACION / DESTRUCCION DE ENTIDADES EN MODO NORMAL
	//--------------------------------------------------------------------------------------

	void ILogicFacade::addGfx(CScenario* scenario){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(scenario);

	}

	void ILogicFacade::addGfx(CAvatar* avatar){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(avatar);
	}

	void ILogicFacade::addGfx(CEntity* entity){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(entity);
	}

	void ILogicFacade::addGfx(CMultimediaFurniture* furniture){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(furniture);
	}

	void ILogicFacade::addGfx(CScreen* screen){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(screen);
	}

	void ILogicFacade::addGfx(CFrame* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(uiComponent);
	}

	void ILogicFacade::addGfx(CButton* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(uiComponent);
	}

	void ILogicFacade::addGfx(CLabel* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(uiComponent);
	}

	void ILogicFacade::addGfx(CTextBox* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->addGfxComponent(uiComponent);
	}

	void ILogicFacade::deleteGfx(CUiComponent* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->removeGfxComponent(uiComponent);
	}

	void ILogicFacade::deleteGfx(CFrame* uiComponent){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->removeGfxComponent(uiComponent);
	}

	void ILogicFacade::deleteGfx(CAvatar* avatar){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->removeGfxComponent(avatar);
	}

	void ILogicFacade::deleteGfx(CEntity* entity){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->removeGfxComponent(entity);
	}

	void ILogicFacade::deleteGfx(CScenario* scenario){
		_cdeIterator = _creationDestroyListeners.begin();
		_cdeEnd = _creationDestroyListeners.end();
		for(;_cdeIterator!= _cdeEnd; ++_cdeIterator)
			(*_cdeIterator)->removeGfxComponent(scenario);
	}

	//--------------------------------------------------------------------------------------
	// Emision de eventos
	//--------------------------------------------------------------------------------------

	void ILogicFacade::emitChangeState(const std::string& state){
		_gameEventIterator = _gameEventListeners.begin();
		_gameEventEnd = _gameEventListeners.end();

		for(;_gameEventIterator!=_gameEventEnd; ++_gameEventIterator)
			(*_gameEventIterator)->changeStateEvent(state);
	}

	void ILogicFacade::emitEndGameEvent(){
		_gameEventIterator = _gameEventListeners.begin();
		_gameEventEnd = _gameEventListeners.end();

		for(;_gameEventIterator!=_gameEventEnd; ++_gameEventIterator)
			(*_gameEventIterator)->endGameEvent(this);
	}

	void ILogicFacade::emitAddCamera(const std::string& camId){
		_gameEventIterator = _gameEventListeners.begin();
		_gameEventEnd = _gameEventListeners.end();

		for(;_gameEventIterator!=_gameEventEnd; ++_gameEventIterator)
			(*_gameEventIterator)->addCameraEvent(camId, this);
	}

	void ILogicFacade::emitClearCameras(){
		_gameEventIterator = _gameEventListeners.begin();
		_gameEventEnd = _gameEventListeners.end();

		for(;_gameEventIterator!=_gameEventEnd; ++_gameEventIterator)
			(*_gameEventIterator)->clearCamerasEvent();
	}

}
