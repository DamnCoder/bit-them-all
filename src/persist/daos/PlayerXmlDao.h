/*
 * PlayerXmlDao.h
 *
 *  Created on: 14/02/2011
 *      Author: jorge
 */

#ifndef PLAYERXMLDAO_H_
#define PLAYERXMLDAO_H_

#include "persist/XmlDaoAdapter.h"

namespace logic {
	class CPlayer;
}

namespace persist {

	class CPlayerXmlDao :
		public CXmlDao<logic::CPlayer*>,
		public CPrivateSingleton<CPlayerXmlDao> {

	friend class CDaoServer;
	friend class CPrivateSingleton<CPlayerXmlDao>;

	public:
		logic::CPlayer*	read(int id) { return 0; }
		std::vector<logic::CPlayer*> read();

	protected:
		//CPlayerXmlDao() : CXmlDao<logic::CPlayer*>() {}
		CPlayerXmlDao();
		~CPlayerXmlDao();

	};
}

#endif /* PLAYERXMLDAO_H_ */
