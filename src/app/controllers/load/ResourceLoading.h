/*
 * ResourceLoading.h
 *
 *  Created on: 09/02/2011
 *      Author: jorge
 */

#ifndef RESOURCELOADING_H_
#define RESOURCELOADING_H_

#include "LoadState.h"

namespace app {

	class CResourceLoading : public CLoadState::CLoadSubstate {
	public:

		CResourceLoading(
				CBitThemAllApp *app,
				CLoadState *loadState):
					CLoadSubstate(app, loadState),
					_order(0)
		{}

		virtual ~CResourceLoading();

		void onCreate();
		void onActivate();

		void onRun(float dt);

	private:

		struct tResourcePaths {
			std::string texturesPath;
			std::string buttonsPath;
			std::string staticGfxPath;
			std::string animatedGfxPath;
		};

		void readPaths();

		uint			_order;

		tResourcePaths	_res;

	};

}

#endif /* RESOURCELOADING_H_ */
