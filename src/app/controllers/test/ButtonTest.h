/*
 * ButtonTest.h
 *
 *  Created on: 11/02/2011
 *      Author: jorge
 */

#ifndef BUTTONTEST_H_
#define BUTTONTEST_H_

#include "app/controllers/YaoController.h"
#include "app/BitThemAllApp.h"

namespace app {

	class CButtonTest : public CYaoController{
	public:
		CButtonTest(CBitThemAllApp* app);
		virtual ~CButtonTest();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void mousePressed(const event::MouseEvent& me);

	protected:
		void executeLogic(float dt);
		void render(float dt);

		logic::CFrame*	_frame;
		logic::CButton*	_button1;
		logic::CButton*	_button2;

	};

}

#endif /* BUTTONTEST_H_ */
