/*
 * CameraXmlDao.h
 *
 *  Created on: 22/02/2011
 *      Author: jorge
 */

#ifndef CAMERAXMLDAO_H_
#define CAMERAXMLDAO_H_

#include "persist/XmlDaoAdapter.h"

namespace gui {
	class CAbstractCamera;
}

namespace persist {

	class CCameraXmlDao :
		public CXmlDao<gui::CAbstractCamera*>,
		public CPrivateSingleton<CCameraXmlDao>{

	friend class CDaoServer;
	friend class CPrivateSingleton<CCameraXmlDao>;

	public:
		gui::CAbstractCamera*				read(int id) { return 0; }
		std::vector<gui::CAbstractCamera*>	read();

	protected:
		CCameraXmlDao();
		~CCameraXmlDao();

	};

}

#endif /* CAMERAXMLDAO_H_ */
