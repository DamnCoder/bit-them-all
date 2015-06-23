/*
 * CScreenConfigXmlDao.cpp
 *
 *  Created on: 29/01/2010
 *      Author: jorge
 */

#include "ScreenConfigXmlDao.h"

#include "gui/WindowManager.h"

namespace persist {

	CScreenConfigXmlDao::CScreenConfigXmlDao() : CXmlDao<gui::tDisplayMode>() {}
	CScreenConfigXmlDao::~CScreenConfigXmlDao() {}

	gui::tDisplayMode CScreenConfigXmlDao::read(int id){
		// A Recuperar parametros de pantalla
		TiXmlElement* pScreenConfigElement = _hRoot.FirstChild("screen_config").Element();

		TiXmlHandle hScreenConfig = TiXmlHandle(pScreenConfigElement);

		TiXmlElement* pScreenElement = hScreenConfig.FirstChild("screen").Element();

		gui::tDisplayMode screenConfig;

		while(pScreenElement){

			int scrid = 0;
			pScreenElement->QueryIntAttribute("id", &scrid);

			if(scrid == id){
				screenConfig = getDisplayMode(pScreenElement);
				return screenConfig;
			}

			pScreenElement = pScreenElement->NextSiblingElement("screen");
		}

		return screenConfig;
	}

	gui::tDisplayMode CScreenConfigXmlDao::getDisplayMode(TiXmlElement* pScreenElement){
		gui::tDisplayMode displayMode;

		//std::cout << "*************************************\n";
		//std::cout << "[CScreenConfigXmlDao::getDisplayMode]\n";

		TiXmlHandle hScreen(pScreenElement);

		// A Recuperar parametros de pantalla

		std::string nombre_pantalla = hScreen.FirstChild("name").Element()->GetText();

		std::string ancho = hScreen.FirstChild("width").Element()->GetText();
		uint ancho_pantalla = atoi(ancho.c_str());

		std::string alto = hScreen.FirstChild("height").Element()->GetText();
		uint alto_pantalla = atoi(alto.c_str());

		std::string bpp = hScreen.FirstChild("bpp").Element()->GetText();
		uint bits_por_pixel = atoi(bpp.c_str());

		std::string pantalla = hScreen.FirstChild("fullscreen").Element()->GetText();
		int pantalla_completa = atoi(pantalla.c_str());

		//std::cout << "Nombre: "<<nombre_pantalla<<"\n";
		//std::cout << "Ancho: "<<ancho_pantalla<<"\n";
		//std::cout << "Alto: "<<alto_pantalla<<"\n";
		//std::cout << "BPP: "<<bits_por_pixel<<"\n";
		//std::cout << "Pantalla completa: "<<pantalla_completa<<"\n";

		displayMode = gui::tDisplayMode(
				ancho_pantalla, alto_pantalla,
				bits_por_pixel, pantalla_completa,
				nombre_pantalla);

		return displayMode;
	}

}
