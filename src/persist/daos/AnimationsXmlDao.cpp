/*
 * AnimationsXmlDao.cpp
 *
 *  Created on: 30/05/2011
 *      Author: jorge
 */

#include "AnimationsXmlDao.h"

#include "persist/md3/AnimationLoader.h"

namespace persist {

	CAnimationsXmlDao::CAnimationsXmlDao() : CXmlDao<logic::TAnimationsMap>() {}

	CAnimationsXmlDao::~CAnimationsXmlDao() {}

	logic::TAnimationsMap CAnimationsXmlDao::read(int i){

		std::string path = _pElem->Attribute("path");

		//std::cout<<"[CAnimationsXmlDao::read] Ruta: "<<path<<"\n";

		TiXmlElement* pAnimGfxElement = _hRoot.FirstChild("model").Element();

		std::string foldername;
		logic::CAnimationManager* animationManager;

		while(pAnimGfxElement){
			CAnimationLoader animLoader;

			foldername = pAnimGfxElement->Attribute("foldername");

			if(!exists(foldername)){

				animationManager = new logic::CAnimationManager();

				//std::cout<<"[CAnimationsXmlDao::read] Ruta: "<<path+foldername<<"\n";

				animLoader.importAnimations(path+foldername+(std::string)"/");

				animationManager->_avatarType = foldername;
				animationManager->_legsAnimations.add(animLoader.lowerAnimationsNames());
				animationManager->_torsoAnimations.add(animLoader.upperAnimationsNames());
				animationManager->_headAnimations.add(animLoader.headAnimationsNames());

				_animationMap[foldername] = animationManager;
			}
			pAnimGfxElement = pAnimGfxElement->NextSiblingElement("model");
		}

		return _animationMap;
	}


	bool CAnimationsXmlDao::exists(const std::string& foldername){
		logic::TAnimationsMap::iterator it = _animationMap.find(foldername);
		if(it == _animationMap.end())
			return false;
		return true;
	}


}
