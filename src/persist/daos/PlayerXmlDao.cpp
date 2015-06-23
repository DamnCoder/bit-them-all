/*
 * PlayerXmlDao.cpp
 *
 *  Created on: 14/02/2011
 *      Author: jorge
 */

#include "PlayerXmlDao.h"

#include "logic/entitys/Player.h"

namespace persist {

	CPlayerXmlDao::CPlayerXmlDao() : CXmlDao<logic::CPlayer*>() {}

	CPlayerXmlDao::~CPlayerXmlDao() {}

	std::vector<logic::CPlayer*> CPlayerXmlDao::read(){
		std::vector<logic::CPlayer*> players;
		TiXmlElement* pAnimGfxElement = _hRoot.FirstChild("animated").Element();

		float scale = 1;
		std::string name, foldername, skin, id;

		while(pAnimGfxElement){
			//std::cout<<"Etiqueta: "<<pAnimGfxElement->Value()<<"\n";

			name = pAnimGfxElement->Attribute("name");
			foldername = pAnimGfxElement->Attribute("foldername");
			skin = pAnimGfxElement->Attribute("skin");
			pAnimGfxElement->QueryFloatAttribute("scale", &scale);

			//id = foldername + (std::string)"@" + skin;
			id = foldername;

			if(foldername != "host") {
				players.push_back(
						new logic::CPlayer(
								id, name,
								0, 0, 0,
								180, false,
								scale,
								skin)
				);
			}
			pAnimGfxElement = pAnimGfxElement->NextSiblingElement("animated");
		}

		return players;
	}

}
