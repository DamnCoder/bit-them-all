/*
 * LoadState.h
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#ifndef LOADSTATE_H_
#define LOADSTATE_H_

#include "app/controllers/CBTAController.h"

namespace app {

	using namespace logic;

	class CBitThemAllApp;

	class CLoadState: public CBTACompositeController {
	public:

		class CLoadSubstate : public CBTAController {
		public:
			CLoadSubstate(CBitThemAllApp *app, CLoadState *loadState):
				CBTAController(app),
				_parentState(loadState){}

		protected:
			/** Puntero al estado al que pertenece el subestado */
			CLoadState*	_parentState;
		};

		CLoadState(CBitThemAllApp* app);
		~CLoadState();

		void onCreate();
		void onDestroy();

		void createLoadHud();
		void destroyLoadHud();

		void resourceTypeInLoad(const std::string& resourceType);
		void addLoadFinishLabel(const std::string& text);

	protected:

		/**
		 * Método reimplementado por las clases hijas y que realiza el dibujado
		 * de la escena
		 * @param dt Tiempo transcurrido en milisegundos desde el último
		 * dibujado.
		 */
		void render(float dt);

	private:

		//--------------------------------------------------------------------------
		// ATRIBUTOS
		//--------------------------------------------------------------------------

		CLabel*					_loadingLabel;
		CLabel*					_resourceTypeLabel;

		std::vector<CLabel*>	_labels;

		float					_heightOkLabel;

		float 					_scrwidth;
		float 					_scrheight;
		float 					_dividedHeight;
		float 					_dividedWidth;

	};

}

#endif /* LOADSTATE_H_ */
