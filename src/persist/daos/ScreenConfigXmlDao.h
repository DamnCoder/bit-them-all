/*
 * CScreenConfigXmlDao.h
 *
 *  Created on: 29/01/2010
 *      Author: jorge
 */

#ifndef SCREENCONFIGXMLDAO_H_
#define SCREENCONFIGXMLDAO_H_

#include "persist/XmlDaoAdapter.h"

namespace gui {
	struct tDisplayMode;
}

namespace persist {

	class CScreenConfigXmlDao :
		public CXmlDao<gui::tDisplayMode>,
		public CPrivateSingleton<CScreenConfigXmlDao>{

	friend class CDaoServer;
	friend class CPrivateSingleton<CScreenConfigXmlDao>;

	public:
		gui::tDisplayMode read(int id);
		std::vector<gui::tDisplayMode> read(){
			std::vector<gui::tDisplayMode> v;
			return v;
		}

	protected:
		CScreenConfigXmlDao();
		~CScreenConfigXmlDao();

		gui::tDisplayMode getDisplayMode(TiXmlElement* pScreenElement);

	};

}

#endif /* SCREENCONFIGXMLDAO_H_ */
