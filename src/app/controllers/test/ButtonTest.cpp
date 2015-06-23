/*
 * ButtonTest.cpp
 *
 *  Created on: 11/02/2011
 *      Author: jorge
 */

#include "ButtonTest.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Button.h"

namespace app {

	CButtonTest::CButtonTest(CBitThemAllApp* app):
			CYaoController(app),
			_button1(0){
	}

	CButtonTest::~CButtonTest() {
	}


	void CButtonTest::onCreate(){
		//cout<<"Creado estado contenedor ButtonTest\n";
		CYaoController::onCreate();

		activateGfxManagers();

		int scrwidth = 0;// _appBTA->getWindow()->getWidth();
		int scrheight = 0; // _appBTA->getWindow()->getHeight();

		_frame = new logic::CFrame(0, 0, scrwidth, scrheight);

		_button1 = new logic::CButton("Hola Button1!", 150, 250, 200, 50);
		_button1->setStyle("webstyle_aqua");

		_button2 = new logic::CButton("Hola Button2!", 150, 150, 200, 50);
		_button2->setStyle("webstyle_aqua");

		_frame->add(_button1);
		_frame->add(_button2);
		_frame->addListener((event::MouseListener*)this);

		_sceneManager->addGfxComponent(_button1);
		_sceneManager->addGfxComponent(_button2);

		_inputManager->addListener(_frame);
	}

	void CButtonTest::onDestroy(){
		//cout<<"Destruido estado contenedor ButtonTest\n";

		_inputManager->removeListener(_frame);

		_sceneManager->removeGfxComponent(_frame);

		deactivateGfxManagers();

		CYaoController::onDestroy();

	}

	void CButtonTest::onActivate(){

	}

	void CButtonTest::onDeactivate(){

	}

	void CButtonTest::mousePressed(const event::MouseEvent& me){
		if(me.getSource() == _button1){
			//cout<<"Presionado boton 1\n";
			_app->askForTermination();
		}
		if(me.getSource() == _button2){
			//cout<<"Presionado boton 2\n";
		}
	}

	void CButtonTest::executeLogic(float dt){

	}

	void CButtonTest::render(float dt){
		_yaoApp->getWindow()->clearScreen();
		_sceneManager->render(dt);
		_yaoApp->getWindow()->swapBuffers();
	}

}
