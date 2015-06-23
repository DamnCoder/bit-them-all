/*
 * CBTAController.h
 *
 *  Created on: 03/09/2011
 *      Author: jorge
 */

#ifndef CBTACONTROLLER_H_
#define CBTACONTROLLER_H_

#include "app/controllers/YaoCompositeController.h"
#include "app/BitThemAllApp.h"

namespace app {
	class CBTAController : public CYaoController {
	public:
		/** Constructor de la clase */
		CBTAController(CBitThemAllApp* btaApp):
			CYaoController(btaApp),
			_appBTA(btaApp){}

		/** Destructor de la clase */
		virtual ~CBTAController(){}

	protected:
		CBitThemAllApp*	_appBTA;
	};

	class CBTACompositeController : public CYaoCompositeController {
	public:
		/** Constructor de la clase */
		CBTACompositeController(CBitThemAllApp* btaApp):
			CYaoCompositeController(btaApp),
			_appBTA(btaApp){}

		/** Destructor de la clase */
		virtual ~CBTACompositeController(){}

	protected:
		CBitThemAllApp*	_appBTA;
	};
}

#endif /* CBTACONTROLLER_H_ */
