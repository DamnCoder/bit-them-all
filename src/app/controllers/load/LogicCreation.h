/*
 * LoadWarningState.h
 *
 *  Created on: 04/12/2010
 *      Author: jorge
 */

#ifndef SCENARIOCREATION_H_
#define SCENARIOCREATION_H_

#include "LoadState.h"

#include "persist/IDao.h"

namespace app {

	class CLogicCreation: public CLoadState::CLoadSubstate {

	public:

		CLogicCreation(
				CBitThemAllApp *app,
				CLoadState *loadState):
					CLoadSubstate(app, loadState),
					_order(0)
		{}

		~CLogicCreation();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();
		void onRun(float dt);

	private:

		uint _order;

		persist::IDao<CScenario*>*		_scenarioDAO;
		persist::IDao<CPlayer*>*		_playerDAO;
		persist::IDao<TAnimationsMap>*	_animationsDAO;

		CManager<CScenario*>* 	_scenarioManager;
		CManager<CPlayer*>* 		_playerManager;

		TAnimationsMap					_animationsMap;

	};


}

#endif /* SCENARIOCREATION_H_ */
