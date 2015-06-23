/*
 * CVideoTest.h
 *
 *  Created on: 19/08/2011
 *      Author: jorge
 */

#ifndef CVIDEOTEST_H_
#define CVIDEOTEST_H_

#include "app/controllers/YaoController.h"
#include "app/BitThemAllApp.h"

namespace logic {
	class CEntity;
	class CScreen;
	class CPlayer;
}

namespace app {

	class CVideoTest : public CYaoController {
	public:
		CVideoTest(CBitThemAllApp* app);
		virtual ~CVideoTest();

		void onCreate();
		void onDestroy();

		void onActivate();
		void onDeactivate();

		void keyReleased(const event::KeyEvent& ke);
		void keyPressed(const event::KeyEvent& ke);

	protected:
		void executeLogic(float dt);
		void render(float dt);

	private:
		logic::CEntity*	_floor;
		logic::CScreen* _screen;
		logic::CPlayer*	_player1;
		logic::CPlayer*	_player2;

		float			_pos;
		float			_step;

		float 			_rotation;
	};

}

#endif /* CVIDEOTEST_H_ */
