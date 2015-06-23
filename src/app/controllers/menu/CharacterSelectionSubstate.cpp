/*
 * CharacterSelection.cpp
 *
 *  Created on: 27/10/2010
 *      Author: jorge
 */

#include "CharacterSelectionSubstate.h"

#include "gui/graphicserver/OglCamera.h"
#include "gui/cam/StaticCamera.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Button.h"
#include "logic/ui/Label.h"
#include "logic/entitys/Player.h"

namespace app {

	CCharacterSelectionSubstate::~CCharacterSelectionSubstate() {
	}

	void CCharacterSelectionSubstate::onCreate(){
		CYaoController::onCreate();
#ifdef _DEBUG
		//cout<<"Creado subestado Seleccion de Personaje\n";
#endif
		_sceneManager = _parentState->sceneManager();
		_playerManager = _appBTA->getPlayerManager();
		_cameraManager = _sceneManager->getCameraManager();
		_numChoosedPlayers = 0;
	}

	void CCharacterSelectionSubstate::onDestroy(){
#ifdef _DEBUG
		//cout<<"Destruido subestado Seleccion de Personaje\n";
#endif
		CYaoController::onDestroy();
	}

	void CCharacterSelectionSubstate::onActivate(){
#ifdef _DEBUG
		//cout<<"Activado subestado Seleccion de Personaje\n";
#endif

		_parentState->renderHudFirst(true);
		_parentState->setSelectedPlayers(false);
		_numChoosedPlayers = 0;

		//_lSelectionableChar->setVisible(true);
		//_background->setVisible(true);

		_currentCharacter = _playerManager->getSelected();
		_currentCharacter->setVisible(true);

		// Creamos una camara simple
		_cameraManager->clearCameraQueue();
		_cameraManager->addCamera(
				"estaticaJugador",
				new gui::CStaticCamera(
						"estaticaPresentador",
						math::CVector3f(0, 50, -130),
						math::CVector3f(0, 30, 0)));

		_cameraManager->addCameraToQueue("estaticaJugador");

		createCharacterSelectionHud();
		createSelectedCharactersHud();
		_lSelectionableChar->setText(_currentCharacter->name());
	}

	void CCharacterSelectionSubstate::onDeactivate(){
#ifdef _DEBUG
		//cout<<"Desactivado subestado Seleccion de Personaje\n";
#endif

		_lSelectionableChar->setVisible(false);
		//_background->setVisible(false);

		_parentState->renderHudFirst(false);
		_currentCharacter->setVisible(false);

		destroyCharacterSelectionHud();
		destroySelectedCharactersHud();
		destroyIconsCharactersHud();
	}

	void CCharacterSelectionSubstate::mouseReleased(const event::MouseEvent& me){
		const logic::CUiComponent* source = me.getSource();

		if(source == _returnBtn){
			_parentState->setState("menuPrincipal");
		}
		else if(source == _previousBtn){
			previousCharacter();
		}
		else if(source == _nextBtn){
			nextCharacter();
		}
		else if(source == _selectBtn){
			++_numChoosedPlayers;

			_logic->setPlayerAppearance(
					_numChoosedPlayers-1,
					_currentCharacter->id(),
					_currentCharacter->skin(),
					_currentCharacter->name(),
					_currentCharacter->scale());
#ifdef _DEBUG
			//cout<<"[CCharacterSelectionSubstate::handleMouseRelease] Has elegido "<<_currentCharacter->id()<<" como jugador "<<_numChoosedPlayers<<"\n";
#endif
			createIconCharacterHud(_numChoosedPlayers-1, _currentCharacter->name(), _currentCharacter->skin());

			if(_numChoosedPlayers >= 4){
				_selectBtn->setVisible(false);
				_parentState->setSelectedPlayers(true);
#ifdef _DEBUG
				//cout<<"[CCharacterSelectionSubstate::handleMouseRelease] Has elegido a los cuatro personajes ya\n";
#endif
			}
		}

	}

	void CCharacterSelectionSubstate::keyReleased(const event::KeyEvent& ke){
		switch(ke.getKey()){
		case event::KeyEvent::MK_LEFT:
			previousCharacter();
			break;
		case event::KeyEvent::MK_RIGHT:
			nextCharacter();
			break;
		default:
			break;
		}
	}

	/**
	 * Método reimplementado por las clases hijas, y que ejecuta la lógica
	 * asociada a este estado. Es llamada antes del dibujado de la
	 * escena.
	 * @param dt Tiempo transcurrido en milisegundos desde la última
	 * actualización de la lógica
	 */
	void CCharacterSelectionSubstate::executeLogic(float dt){
		_orientation += (_rotationSpeed * dt);
		if(_orientation > 360)
			_orientation = 0;
		_currentCharacter->setOrientation(_orientation);
	}

	void CCharacterSelectionSubstate::nextCharacter(){
		_currentCharacter->setVisible(false);

		_currentCharacter = _playerManager->getNext();
		_currentCharacter->setVisible(true);

		_lSelectionableChar->setText(_currentCharacter->name());

		_orientation = 100;

	}

	void CCharacterSelectionSubstate::previousCharacter(){
		_currentCharacter->setVisible(false);

		_currentCharacter = _playerManager->getPrevious();
		_currentCharacter->setVisible(true);

		_lSelectionableChar->setText(_currentCharacter->name());

		_orientation = 100;

	}

	void CCharacterSelectionSubstate::createCharacterSelectionHud(){
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		_lSelectionableChar = new logic::CLabel(scrwidth/2.0f, scrheight-100, "");
		//_lSelectionableChar->setVisible(false);

		_background = new logic::CFrame(0, 0, scrwidth, scrheight);
		_background->setID("fondo_uclm");
		//_background->setID("spotlight");

		_returnBtn = new logic::CButton("Volver", 		200, 100, 210, 70);
		_previousBtn = new logic::CButton("Anterior",	100, 300, 210, 70);
		_nextBtn = new logic::CButton("Siguiente", 		100, 400, 210, 70);
		_selectBtn = new logic::CButton("Seleccionar",	100, 500, 210, 70);

		_returnBtn->setStyle("webstyle_aqua");
		_previousBtn->setStyle("webstyle_aqua");
		_nextBtn->setStyle("webstyle_aqua");
		_selectBtn->setStyle("webstyle_aqua");

		_background->add(_returnBtn);
		_background->add(_previousBtn);
		_background->add(_nextBtn);
		_background->add(_selectBtn);

		//_background->setVisible(false);

/*
		_sceneManager->addUiComponent(_returnBtn);
		_sceneManager->addUiComponent(_previousBtn);
		_sceneManager->addUiComponent(_nextBtn);
		_sceneManager->addUiComponent(_selectBtn);
*/
		_sceneManager->addGfxComponent(_lSelectionableChar);
		_sceneManager->addGfxComponent(_background);

		_background->addListener((event::MouseListener*) this);

		_inputManager->addListener(_background);
	}

	void CCharacterSelectionSubstate::destroyCharacterSelectionHud() {

		_inputManager->removeListener(_background);

		_sceneManager->removeGfxComponent(_lSelectionableChar);
		/*
		_sceneManager->removeUiComponent(_returnBtn);
		_sceneManager->removeUiComponent(_previousBtn);
		_sceneManager->removeUiComponent(_nextBtn);
		_sceneManager->removeUiComponent(_selectBtn);
		*/
		_sceneManager->removeGfxComponent(_background);	// Al eliminar la parte grafica de frame,
														// se borran todas las que tenga asociadas
		safeDelete(_background);						// La clase frame borra todos los componentes
		safeDelete(_lSelectionableChar);
	}

	void CCharacterSelectionSubstate::createSelectedCharactersHud() {
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		float widthColumns = scrwidth / 4.0f;
		float yRowWidth = scrheight / 24.0f;

		float xPos = (3*widthColumns);

		_fCharFrame[0] = new logic::CFrame(0, 0, 166, 166);
		_fCharFrame[1] = new logic::CFrame(0, 0, 166, 166);
		_fCharFrame[2] = new logic::CFrame(0, 0, 166, 166);
		_fCharFrame[3] = new logic::CFrame(0, 0, 166, 166);

		_fCharFrame[0]->setPosition(xPos-8, (3*yRowWidth)-8);
		_fCharFrame[1]->setPosition(xPos-8, (8*yRowWidth)-8);
		_fCharFrame[2]->setPosition(xPos-8, (13*yRowWidth)-8);
		_fCharFrame[3]->setPosition(xPos-8, (18*yRowWidth)-8);

		_fChar[0] = new logic::CFrame(0, 0, 150, 150);
		_fChar[1] = new logic::CFrame(0, 0, 150, 150);
		_fChar[2] = new logic::CFrame(0, 0, 150, 150);
		_fChar[3] = new logic::CFrame(0, 0, 150, 150);

		_fChar[0]->setPosition(xPos, (3*yRowWidth));
		_fChar[1]->setPosition(xPos, (8*yRowWidth));
		_fChar[2]->setPosition(xPos, (13*yRowWidth));
		_fChar[3]->setPosition(xPos, (18*yRowWidth));

		_fCharFrame[0]->setID("tb_square_grey");
		_fCharFrame[1]->setID("tb_square_grey");
		_fCharFrame[2]->setID("tb_square_grey");
		_fCharFrame[3]->setID("tb_square_grey");

		_sceneManager->addGfxComponent(_fCharFrame[0]);
		_sceneManager->addGfxComponent(_fCharFrame[1]);
		_sceneManager->addGfxComponent(_fCharFrame[2]);
		_sceneManager->addGfxComponent(_fCharFrame[3]);
	}

	void CCharacterSelectionSubstate::destroySelectedCharactersHud() {
		for(int i=0; i<4; ++i){
			_sceneManager->removeGfxComponent(_fCharFrame[i]);
			safeDelete(_fCharFrame[i]);
		}

	}

	void CCharacterSelectionSubstate::createIconCharacterHud(int numPlayer, const std::string& name, const std::string& skin) {
		_fChar[numPlayer]->setID(skin+(std::string)"_icon");
		_sceneManager->addGfxComponent(_fChar[numPlayer]);
	}

	void CCharacterSelectionSubstate::destroyIconsCharactersHud() {
		for(int i=0; i<4; ++i){
			_sceneManager->removeGfxComponent(_fChar[i]);
			safeDelete(_fChar[i]);
		}

	}

	void CCharacterSelectionSubstate::imprimeCaracteristicasPersonaje(){
		//const std::string id = _currentCharacter->id();
		//const std::string name = _currentCharacter->name();
		//const std::string skin = _currentCharacter->skin();
		//const float scale = _currentCharacter->scale();
		//const float orientation = _currentCharacter->orientation();
#ifdef _DEBUG
		//printf("Id: %s, Nombre: %s, Skin: %s, Escala: %f, Orientacion: %f \n", id.c_str(), name.c_str(), skin.c_str(), scale, orientation);
#endif

	}

}
