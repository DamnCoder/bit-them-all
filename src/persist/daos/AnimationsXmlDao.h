/*
 * AnimationsXmlDao.h
 *
 *  Created on: 30/05/2011
 *      Author: jorge
 */

#ifndef ANIMATIONSXMLDAO_H_
#define ANIMATIONSXMLDAO_H_

#include "utilitys/Singleton.h"

#include "persist/XmlDaoAdapter.h"
#include "logic/LogicFacade.h"

namespace persist {

	class CAnimationLoader;

	class CAnimationsXmlDao :
		public CXmlDao<logic::TAnimationsMap>,
		public CPrivateSingleton<CAnimationsXmlDao>{

	friend class CDaoServer;
	friend class CPrivateSingleton<CAnimationsXmlDao>;

	public:
		logic::TAnimationsMap read(int i);
		std::vector<logic::TAnimationsMap> read(){
			std::vector<logic::TAnimationsMap> v;
			return v;
		}

	protected:
		CAnimationsXmlDao();
		~CAnimationsXmlDao();

		bool exists(const std::string& foldername);

		logic::TAnimationsMap 	_animationMap;

	};

}

#endif /* ANIMATIONSXMLDAO_H_ */
