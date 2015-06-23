/*
 * ScenariosXmlDao.h
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#ifndef SCENARIOSXMLDAO_H_
#define SCENARIOSXMLDAO_H_

#include "persist/XmlDaoAdapter.h"

#include "logic/entitys/Scenario.h"
#include "gui/cam/AbstractCamera.h"

namespace persist {

	class CScenariosXmlDao :
		public CXmlDao<logic::CScenario*>,
		public CPrivateSingleton<CScenariosXmlDao> {

	friend class CDaoServer;
	friend class CPrivateSingleton<CScenariosXmlDao>;

	public:
		logic::CScenario* 			read(int id);
		vector<logic::CScenario*> 	read();

	protected:
		CScenariosXmlDao();
		~CScenariosXmlDao();

		logic::CScenario* readScenario(const string& path);

		logic::CPlayer* readPlayer(TiXmlElement* pPlayerElement);
		logic::CQuizShowHost* readHost(TiXmlElement* pHostElement);
		logic::CAvatar* readAudience(TiXmlElement* pPublicElement);


		logic::CEntity* readStaticEntity(TiXmlElement* pStaticElement);
		logic::CMultimediaFurniture* readMultFur(TiXmlElement* pMultFurElement);
		logic::CScreen* readScreen(TiXmlElement* pScreenElement);

		vector<string> readSkins(TiXmlElement* pSkinsElement);

		persist::IDao<gui::CAbstractCamera*>*	_cameraDAO;

	};

}

#endif /* SCENARIOSXMLDAO_H_ */
