/*
 * ScenariosXmlDao.cpp
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#include "ScenariosXmlDao.h"

#include "persist/DaoServer.h"

#include "logic/entitys/Screen.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Host.h"
#include "logic/entitys/MultimediaFurniture.h"

namespace persist {
	CScenariosXmlDao::CScenariosXmlDao() : CXmlDao<logic::CScenario*>() {}

	CScenariosXmlDao::~CScenariosXmlDao() {}

	logic::CScenario* CScenariosXmlDao::read(int id){
		// Vamos a recuperar las rutas donde se encuentran las definiciones de los escenarios

		logic::CScenario* scenario = 0;

		TiXmlElement* pScenarioPathElement = _hRoot.FirstChild("scenario").Element();

		while(pScenarioPathElement){
			int idscenario = 0;
			pScenarioPathElement->QueryIntAttribute("id", &idscenario);
			string folder = pScenarioPathElement->Attribute("folder");
			string path = "./data/scenarios/";
			string scenariopath = path+folder+(string)"/gfx.xml";
			//cout<<"[CScenariosXmlDao::read] Escenario: "<<idscenario<<" en "<<scenariopath<<"\n";

			if(idscenario == id){
				_cameraDAO = CDaoServer::instance().getCameraDAO(scenariopath);
				std::vector<gui::CAbstractCamera*> cameras = _cameraDAO->read();
				scenario = readScenario(scenariopath);
				scenario->setCameras(cameras);
			}


			pScenarioPathElement = pScenarioPathElement->NextSiblingElement("scenario");
		}

		return scenario;

	}

	vector<logic::CScenario*> CScenariosXmlDao::read(){
		// Vamos a recuperar las rutas donde se encuentran las definiciones de los escenarios

		vector<logic::CScenario*> scenarioList;

		TiXmlElement* pScenarioPathElement = _hRoot.FirstChild("scenario").Element();

		string  path = _pElem->Attribute("path");
		//cout<<"[CScenariosXmlDao::read] Ruta a los escenarios: "<<path<<"\n";

		while(pScenarioPathElement){
			string folder = pScenarioPathElement->Attribute("folder");
			string scenariopath = path+folder+(string)"/gfx.xml";
			string camspath = path+folder+(string)"/cams.xml";
			//cout<<"[CScenariosXmlDao::read] Escenario en "<<scenariopath<<"\n";
			//cout<<"[CScenariosXmlDao::read] Camaras en "<<camspath<<"\n";

			logic::CScenario* scenario = readScenario(scenariopath);
			_cameraDAO = CDaoServer::instance().getCameraDAO(camspath);
			std::vector<gui::CAbstractCamera*> cameras = _cameraDAO->read();
			scenario->setCameras(cameras);
			//cout<<"[CScenariosXmlDao::read] Numero de camaras para el escenario "<<cameras.size()<<"\n";
			scenarioList.push_back(scenario);

			pScenarioPathElement = pScenarioPathElement->NextSiblingElement("scenario");
		}

		return scenarioList;

	}

	logic::CScenario* CScenariosXmlDao::readScenario(const string& path){
		TiXmlDocument doc(path.c_str());
		if(!doc.LoadFile())
			return false;

		// Creamos el escenario para comenzar a llenarlo
		logic::CScenario* scenario = new logic::CScenario();

		TiXmlHandle	hDoc(&doc);
		// Recuperamos el elemento del nodo raiz
		TiXmlElement* pElem = hDoc.FirstChildElement().Element();

		TiXmlHandle hRoot(pElem);

		// Recorremos los graficos
		TiXmlElement* pGfxElement = hRoot.FirstChild("gfx").Element();
		while(pGfxElement){
			//cout<<"Etiqueta: "<<pGfxElement->Value()<<"\n";

			// Recorremos los graficos estaticos
			TiXmlHandle hGfx(pGfxElement);
			TiXmlElement* pStaticGfxElement = hGfx.FirstChild("static_gfx").Element();
			while(pStaticGfxElement){
				//cout<<"Etiqueta: "<<pStaticGfxElement->Value()<<"\n";

				// Recorremos cada elemento estatico y multimedia
				TiXmlHandle hStaticGfx(pStaticGfxElement);
				TiXmlElement* pStaticElement = hStaticGfx.FirstChild("static").Element();
				TiXmlElement* pMultElement= hStaticGfx.FirstChild("multimedia").Element();
				TiXmlElement* pScreenElement= hStaticGfx.FirstChild("screen").Element();

				// Leemos las pantallas que haya
				while(pScreenElement){
					logic::CScreen* screen = readScreen(pScreenElement);
					scenario->setScreen(screen);
					pScreenElement = pScreenElement->NextSiblingElement("screen");
				}

				// Leo elementos multimedia
				while(pMultElement){
					logic::CMultimediaFurniture* mult = readMultFur(pMultElement);

					scenario->addMultimediaFurniture(mult);
					pMultElement = pMultElement->NextSiblingElement("multimedia");
				}

				// Leo elementos estaticos
				while(pStaticElement){
					//cout<<"Etiqueta: "<<pStaticElement->Value()<<"\n";
					logic::CEntity* entity = readStaticEntity(pStaticElement);
					if(entity->id() == "screen")
						scenario->setScreen(entity);
					scenario->addFurniture(entity);
					pStaticElement = pStaticElement->NextSiblingElement("static");
				}

				pStaticGfxElement = pStaticGfxElement->NextSiblingElement("static_gfx");
			}

			// Recorremos los graficos animados
			TiXmlElement* pAnimatedGfxElement = hGfx.FirstChild("animated_gfx").Element();
			while(pAnimatedGfxElement){
				//cout<<"Etiqueta: "<<pAnimatedGfxElement->Value()<<"\n";

				TiXmlHandle hAnimatedGfx(pAnimatedGfxElement);

				// Recorremos cada jugador animado
				TiXmlElement* pPlayerElement = hAnimatedGfx.FirstChild("player").Element();
				while(pPlayerElement){
					logic::CPlayer* player = readPlayer(pPlayerElement);
					scenario->addPlayer(player);
					pPlayerElement = pPlayerElement->NextSiblingElement("player");
				}

				// Recorremos cada personaje del publico
				TiXmlElement* pHostElement = hAnimatedGfx.FirstChild("host").Element();
				while(pHostElement){
					logic::CQuizShowHost* host = readHost(pHostElement);
					scenario->setHost(host);
					pHostElement = pHostElement->NextSiblingElement("host");
				}


				// Recorremos cada personaje del publico
				TiXmlElement* pPublicElement = hAnimatedGfx.FirstChild("public").Element();
				while(pPublicElement){
					logic::CAvatar* audience = readAudience(pPublicElement);
					scenario->addAudience(audience);
					pPublicElement = pPublicElement->NextSiblingElement("public");
				}

				pAnimatedGfxElement = pAnimatedGfxElement->NextSiblingElement("animated_gfx");
			}

			pGfxElement = pGfxElement->NextSiblingElement("gfx");
		}

		return scenario;
	}


	logic::CPlayer* CScenariosXmlDao::readPlayer(TiXmlElement* pPlayerElement) {
		//cout<<"Etiqueta: "<<pPlayerElement->Value()<<"\n";

		string id = pPlayerElement->Attribute("id");

		string type = "";
		if(pPlayerElement->Attribute("type"))
			type = pPlayerElement->Attribute("type");

		TiXmlHandle hPlayer(pPlayerElement);
		TiXmlElement* pPositionElement = hPlayer.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hPlayer.FirstChild("orientation").Element();

		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		//cout<<"[ScenariosXmlDao::readScenario] Animado "<<id<<", ["<<x<<", "<<y<<", "<<z<<"], "<<orientation<<"\n";

		logic::CPlayer* player = new logic::CPlayer(id, id, x, y, z, orientation, true);
		return player;
	}

	logic::CQuizShowHost* CScenariosXmlDao::readHost(TiXmlElement* pHostElement) {
		//cout<<"Etiqueta: "<<pHostElement->Value()<<"\n";

		string id = pHostElement->Attribute("id");

		string type = "";
		if(pHostElement->Attribute("type"))
			type = pHostElement->Attribute("type");

		TiXmlHandle hHost(pHostElement);
		TiXmlElement* pPositionElement = hHost.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hHost.FirstChild("orientation").Element();

		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		//cout<<"[ScenariosXmlDao::readScenario] Animado "<<id<<", ["<<x<<", "<<y<<", "<<z<<"], "<<orientation<<"\n";

		logic::CQuizShowHost* host= new logic::CQuizShowHost(id, id, x, y, z, orientation, true, 9);
		return host;
	}

	logic::CAvatar* CScenariosXmlDao::readAudience(TiXmlElement* pPublicElement) {
		//cout<<"Etiqueta: "<<pPublicElement->Value()<<"\n";

		string id = pPublicElement->Attribute("id");

		string type = "";
		if(pPublicElement->Attribute("type"))
			type = pPublicElement->Attribute("type");

		TiXmlHandle hPublic(pPublicElement);
		TiXmlElement* pPositionElement = hPublic.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hPublic.FirstChild("orientation").Element();

		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		//cout<<"[ScenariosXmlDao::readScenario] Animado "<<id<<", ["<<x<<", "<<y<<", "<<z<<"], "<<orientation<<"\n";

		logic::CAvatar* audience= new logic::CAvatar("player", id, x, y, z, orientation, true, 10);
		return audience;
	}

	/**
	 * Lee el tag XML static y agrega al escenario la entidad correspondiente
	 * @param pStaticElement
	 * @param scenario
	 */
	logic::CEntity* CScenariosXmlDao::readStaticEntity(TiXmlElement* pStaticElement) {
		TiXmlHandle hStatic(pStaticElement);
		TiXmlElement* pPositionElement = hStatic.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hStatic.FirstChild("orientation").Element();

		string id = pStaticElement->Attribute("id");

		bool reflect = false;
		logic::tRGBAColor color;
		if(pStaticElement->Attribute("reflect")){
			TiXmlElement* pColorElement = hStatic.FirstChild("color").Element();
			pColorElement->QueryFloatAttribute("r",&color.r);
			pColorElement->QueryFloatAttribute("g",&color.g);
			pColorElement->QueryFloatAttribute("b",&color.b);
			pColorElement->QueryFloatAttribute("a",&color.a);
			//cout<<"[ScenariosXmlDao::readScenario] Elemento: "<<id<<" REFLEJA!!\n";
			reflect = true;
		}

		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		//cout<<"[ScenariosXmlDao::readScenario]Entidad "<<id<<", ["<<x<<", "<<y<<", "<<z<<"], "<<orientation<<"\n";

		logic::CEntity* entity = new logic::CEntity(id, x, y, z, orientation, true);
		entity->setReflection(reflect);
		entity->setColor(color);

		return entity;
	}

	/**
	 * Lee el tag XML multimedia y devuelve el objeto correspondiente
	 * @param pMultFurElement
	 * @return
	 */
	logic::CMultimediaFurniture* CScenariosXmlDao::readMultFur(TiXmlElement* pMultFurElement) {
		TiXmlHandle hMultFur(pMultFurElement);
		TiXmlElement* pPositionElement = hMultFur.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hMultFur.FirstChild("orientation").Element();
		TiXmlElement* pSkinsElement = hMultFur.FirstChild("skins").Element();

		string id = pMultFurElement->Attribute("id");

		bool reflect = false;
		logic::tRGBAColor color;
		if(pMultFurElement->Attribute("reflect")){
			TiXmlElement* pColorElement = hMultFur.FirstChild("color").Element();
			pColorElement->QueryFloatAttribute("r",&color.r);
			pColorElement->QueryFloatAttribute("g",&color.g);
			pColorElement->QueryFloatAttribute("b",&color.b);
			pColorElement->QueryFloatAttribute("a",&color.a);
			//cout<<"[ScenariosXmlDao::readScenario] Elemento: "<<id<<" REFLEJA!!\n";
			reflect = true;
		}

		float interval = 0;
		pSkinsElement->QueryFloatAttribute("interval", &interval);


		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		vector<string> skinList = readSkins(pSkinsElement);

		logic::CMultimediaFurniture* mult = new logic::CMultimediaFurniture(id, x, y, z, orientation, true, 1.0);
		mult->setInterval(interval);
		mult->setSkinList(skinList);
		mult->setReflection(reflect);
		mult->setColor(color);
		return mult;
	}


	logic::CScreen* CScenariosXmlDao::readScreen(TiXmlElement* pScreenElement) {
		TiXmlHandle hScreen(pScreenElement);
		TiXmlElement* pPositionElement = hScreen.FirstChild("position").Element();
		TiXmlElement* pOrientationElement = hScreen.FirstChild("orientation").Element();
		TiXmlElement* pSkinsElement = hScreen.FirstChild("skins").Element();

		string id = pScreenElement->Attribute("id");

		float interval = 0;
		pSkinsElement->QueryFloatAttribute("interval", &interval);

		float x=0,y=0,z=0;
		pPositionElement->QueryFloatAttribute("x",&x);
		pPositionElement->QueryFloatAttribute("y",&y);
		pPositionElement->QueryFloatAttribute("z",&z);

		float orientation = 0;
		pOrientationElement->QueryFloatAttribute("value",&orientation);

		vector<string> skinList = readSkins(pSkinsElement);

		//cout<<"[ScenariosXmlDao::readScreen]Entidad "<<id<<", ["<<x<<", "<<y<<", "<<z<<"], "<<orientation<<"\n";

		logic::CScreen* screen = new logic::CScreen(id, x, y, z, orientation, true);
		screen->setSkinList(skinList);
		screen->setInterval(interval);

		return screen;

	}

	vector<string> CScenariosXmlDao::readSkins(TiXmlElement* pSkinsElement) {
		vector<string> skinList;
		TiXmlHandle hSkin(pSkinsElement);
		TiXmlElement* pSkinElement = hSkin.FirstChild("skin").Element();
		while(pSkinElement){
			string skinId = pSkinElement->Attribute("id");
			//std::cout<<"[CScenariosXmlDao::readSkins] Skin: "<<skinId<<"\n";
			skinList.push_back(skinId);
			pSkinElement = pSkinElement->NextSiblingElement("skin");
		}
		return skinList;
	}

}
