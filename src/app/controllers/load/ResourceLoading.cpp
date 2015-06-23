/*
 * ResourceLoading.cpp
 *
 *  Created on: 09/02/2011
 *      Author: jorge
 */

#include "ResourceLoading.h"

#include "logic/QuestionManager.h"
#include "gui/graphicserver/GraphicServer.h"

#include <tinyxml/tinyxml.h>

namespace app {

	CResourceLoading::~CResourceLoading() {}

	void CResourceLoading::onCreate() {
		//std::cout<<"Creado subestado ResourceLoading\n";
		CLoadState::CLoadSubstate::onCreate();
		readPaths();
	}
	void CResourceLoading::onActivate(){
		//std::cout<<"Activado subestado ResourceLoading\n";
		_order = 0;
	}

	void CResourceLoading::onRun(float dt){
		// Cargo los recursos del juego en orden
		switch(_order){
		case 0:
			_parentState->resourceTypeInLoad("textures");
			break;
		case 1:
			// Cargo texturas
			gui::CGraphicServer::instance().loadSprites(_res.texturesPath);
			_parentState->addLoadFinishLabel("TEXTURES");
			_parentState->resourceTypeInLoad("buttons");
			break;

		case 2:
			// Cargo botones
			gui::CGraphicServer::instance().loadButtons(_res.buttonsPath);
			_parentState->addLoadFinishLabel("BUTTONS");
			_parentState->resourceTypeInLoad("static models");
			break;

		case 3:
			// Cargo modelos estaticos
			gui::CGraphicServer::instance().loadStaticGfx(_res.staticGfxPath);
			_parentState->addLoadFinishLabel("STATIC MODELS");
			_parentState->resourceTypeInLoad("animated models");
			break;

		case 4:
			// Cargo modelos animados
			gui::CGraphicServer::instance().loadAnimatedGfx(_res.animatedGfxPath);
			_parentState->addLoadFinishLabel("ANIMATED MODELS");
			_parentState->resourceTypeInLoad("questions");
			break;

		case 5:
			// Cargo la base de datos de preguntas
			logic::CQuestionManager::instance().load("./data/resources/questions.xml");
			_parentState->addLoadFinishLabel("QUESTIONS");
			break;

		case 6:
			// Cargo la musica

			_audioManager->loadSong("./data/export/sounds/songs/first_question_round.ogg");
			_audioManager->loadSong("./data/export/sounds/songs/end_program.ogg");
			_audioManager->loadSong("./data/export/sounds/songs/explaining_rules.ogg");
			_audioManager->loadSong("./data/export/sounds/songs/suspense2.ogg");
			_audioManager->loadSong("./data/export/sounds/songs/overture.ogg");

			_audioManager->loadEffect("./data/export/sounds/effects/applause01.wav");		// 0
			_audioManager->loadEffect("./data/export/sounds/effects/start_round.wav");		// 1
			_audioManager->loadEffect("./data/export/sounds/effects/ff7_victory.wav");		// 2
			_audioManager->loadEffect("./data/export/sounds/effects/fallo.wav");			// 3
			_audioManager->loadEffect("./data/export/sounds/effects/sonido_pulsar.wav");	// 4

			_audioManager->loadDialogue(0, "./data/export/sounds/questions/question_0.wav");
			_audioManager->loadDialogue(1, "./data/export/sounds/questions/question_1.wav");
			_audioManager->loadDialogue(2, "./data/export/sounds/questions/question_2.wav");
			_audioManager->loadDialogue(3, "./data/export/sounds/questions/question_3.wav");
			_audioManager->loadDialogue(4, "./data/export/sounds/questions/question_4.wav");
			_audioManager->loadDialogue(5, "./data/export/sounds/questions/question_5.wav");
			_audioManager->loadDialogue(6, "./data/export/sounds/questions/question_6.wav");
			_audioManager->loadDialogue(7, "./data/export/sounds/questions/question_7.wav");
			_audioManager->loadDialogue(8, "./data/export/sounds/questions/question_8.wav");
			_audioManager->loadDialogue(9, "./data/export/sounds/questions/question_9.wav");
			_audioManager->loadDialogue(10, "./data/export/sounds/questions/question_10.wav");
			_audioManager->loadDialogue(11, "./data/export/sounds/questions/question_11.wav");
			_audioManager->loadDialogue(12, "./data/export/sounds/questions/question_12.wav");
			_audioManager->loadDialogue(13, "./data/export/sounds/questions/question_13.wav");
			_audioManager->loadDialogue(14, "./data/export/sounds/questions/question_14.wav");
			_audioManager->loadDialogue(15, "./data/export/sounds/questions/question_15.wav");
			_audioManager->loadDialogue(16, "./data/export/sounds/questions/question_16.wav");
			_audioManager->loadDialogue(17, "./data/export/sounds/questions/question_17.wav");
			_audioManager->loadDialogue(18, "./data/export/sounds/questions/question_18.wav");
			_audioManager->loadDialogue(19, "./data/export/sounds/questions/question_19.wav");
			_audioManager->loadDialogue(20, "./data/export/sounds/questions/question_20.wav");


			gui::CVideoServer::instance().add("video1.mpg", gui::CVideoServer::SMPEG);
			gui::CVideoServer::instance().add("video2.mpg", gui::CVideoServer::SMPEG);
			gui::CVideoServer::instance().add("intro.mpg", gui::CVideoServer::SMPEG);
			gui::CVideoServer::instance().add("historia_parte4.mpg", gui::CVideoServer::SMPEG);
			gui::CVideoServer::instance().add("historia_parte9.mpg", gui::CVideoServer::SMPEG);

			break;
		case 7:
			_parentState->setState("logicCreation");
			break;
		}
		++_order;
	}

	void CResourceLoading::readPaths() {
		TiXmlDocument doc("./data/config.xml");
		if(!doc.LoadFile())
			return ;

		TiXmlHandle	hDoc(&doc);
		// Recuperamos el elemento del nodo raiz
		TiXmlElement* pElem = hDoc.FirstChildElement().Element();

		TiXmlHandle hRoot(pElem);

		TiXmlElement* pResElement = hRoot.FirstChild("resources").Element();

		std::string resourcesPath = pResElement->Attribute("path");

		TiXmlHandle hResource(pResElement);
		TiXmlElement* pGfxElement = hResource.FirstChild("animated").Element();
		std::string animatedFile = pGfxElement->Attribute("file");
		_res.animatedGfxPath = resourcesPath+animatedFile;
		//std::cout<<"[CResourceLoading::readPaths]"<<_res.animatedGfxPath<<"\n";

		pGfxElement = hResource.FirstChild("static").Element();
		std::string staticFile = pGfxElement->Attribute("file");
		_res.staticGfxPath = resourcesPath+staticFile;
		//std::cout<<"[CResourceLoading::readPaths]"<<_res.staticGfxPath<<"\n";

		pGfxElement = hResource.FirstChild("button").Element();
		std::string buttonFile = pGfxElement->Attribute("file");
		_res.buttonsPath = resourcesPath+buttonFile;
		//std::cout<<"[CResourceLoading::readPaths]"<<_res.buttonsPath<<"\n";

		pGfxElement = hResource.FirstChild("textures").Element();
		std::string texturesFile = pGfxElement->Attribute("file");
		_res.texturesPath = resourcesPath+texturesFile;
		//std::cout<<"[CResourceLoading::readPaths]"<<_res.texturesPath<<"\n";

	}

}
