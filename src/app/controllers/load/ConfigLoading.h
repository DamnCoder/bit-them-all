/**
 * CConfigLoadingState.h
 *
 *  Created on: 03/09/2010
 *      Author: jorge
 */

#ifndef CCONFIGLOADINGSTATE_H_
#define CCONFIGLOADINGSTATE_H_

#include "LoadState.h"

namespace app {

	class CConfigLoading : public CLoadState::CLoadSubstate {

	public:

		CConfigLoading(
				CBitThemAllApp *app,
				CLoadState *loadState):
					CLoadSubstate(app, loadState)
		{}

		virtual ~CConfigLoading();

		void onActivate();
		void onRun(float dt);

	};

}

#endif /* CCONFIGLOADINGSTATE_H_ */
