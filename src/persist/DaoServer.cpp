/*
 * GestorPersistencia.cpp
 *
 *  Created on: 26/01/2010
 *      Author: jorge
 */

#include "DaoServer.h"

#include "daos/ScreenConfigXmlDao.h"
#include "daos/QuestionXmlDao.h"
#include "daos/ScenariosXmlDao.h"
#include "daos/PlayerXmlDao.h"
#include "daos/CameraXmlDao.h"
#include "daos/AnimationsXmlDao.h"

namespace persist {

	CDaoServer::CDaoServer() {}

	CDaoServer::~CDaoServer() {
		CScreenConfigXmlDao::destroyInstance();
		CQuestionXmlDao::destroyInstance();
		CScenariosXmlDao::destroyInstance();
		CPlayerXmlDao::destroyInstance();
		CAnimationsXmlDao::destroyInstance();
		CCameraXmlDao::destroyInstance();
	}

	IDao<gui::tDisplayMode>* CDaoServer::getScreenConfigDAO(const std::string& path){
		CXmlDao<gui::tDisplayMode>* displayDao = CScreenConfigXmlDao::pointer();
		displayDao->init(path);
		return displayDao;
	}

	IDao<logic::CQuestion*>* CDaoServer::getQuestionDAO(const std::string& path){
		CXmlDao<logic::CQuestion*>* questionDao = CQuestionXmlDao::pointer();
		questionDao->init(path);
		return questionDao;
	}

	IDao<logic::CScenario*>* CDaoServer::getScenarioDAO(const std::string& path){
		CXmlDao<logic::CScenario*>* scenarioDao = CScenariosXmlDao::pointer();
		scenarioDao->init(path);
		return scenarioDao;
	}

	IDao<logic::CPlayer*>* CDaoServer::getPlayerDAO(const std::string& path){
		CXmlDao<logic::CPlayer*>* playerDao = CPlayerXmlDao::pointer();
		playerDao->init(path);
		return playerDao;
	}

	IDao<logic::TAnimationsMap>* CDaoServer::getAnimationsDAO(const std::string& path){
		CXmlDao<logic::TAnimationsMap>* animDao = CAnimationsXmlDao::pointer();
		animDao->init(path);
		return animDao;
	}

	IDao<gui::CAbstractCamera*>* CDaoServer::getCameraDAO(const std::string& path){
		CXmlDao<gui::CAbstractCamera*>* camDao = CCameraXmlDao::pointer();
		camDao->init(path);
		return camDao;
	}
}

