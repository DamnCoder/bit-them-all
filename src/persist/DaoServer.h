/*
 * GestorPersistencia.h
 *
 *  Created on: 26/01/2010
 *      Author: jorge
 */

#ifndef DAOSERVER_H_
#define DAOSERVER_H_


#include "persist/IDao.h"

#include "utilitys/utils.h"

#include "gui/WindowManager.h"
#include "logic/Game.h"
#include "logic/Question.h"
#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"

namespace gui {
	class CAbstractCamera;
}

namespace persist {

	class CDaoServer : public CSingleton<CDaoServer>{
	friend class CSingleton<CDaoServer>;
	public:
		IDao<gui::tDisplayMode>* 		getScreenConfigDAO(const std::string& path);
		IDao<logic::CQuestion*>* 		getQuestionDAO(const std::string& path);
		IDao<logic::CScenario*>* 		getScenarioDAO(const std::string& path);
		IDao<logic::CPlayer*>* 			getPlayerDAO(const std::string& path);
		IDao<logic::TAnimationsMap>*	getAnimationsDAO(const std::string& path);
		IDao<gui::CAbstractCamera*>* 	getCameraDAO(const std::string& path);

	protected:
		CDaoServer();
		virtual ~CDaoServer();

	};

}

#endif /* DAOSERVER_H_ */
