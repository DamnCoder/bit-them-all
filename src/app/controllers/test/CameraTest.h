/*
 * CameraTest.h
 *
 *  Created on: 19/02/2011
 *      Author: jorge
 */

#ifndef CAMERATEST_H_
#define CAMERATEST_H_

#include "app/controllers/YaoController.h"
#include "app/BitThemAllApp.h"

namespace logic{
	class CEntity;
	class CPlayer;
	class CScreen;
}

namespace app {

	class CCameraTest : public CYaoController {
	public:
		CCameraTest(CBitThemAllApp* app);
		~CCameraTest();

		void onCreate();
		void onDestroy();

		void keyReleased(const event::KeyEvent& ke);

	protected:
		void executeLogic(float dt);
		void render(float dt);

	private:
		gui::COglCamera*		_cam;

		logic::CPlayer*			_host;
		logic::CPlayer*			_player;

		vector<logic::CPlayer*> _animatedEntitys;
		vector<logic::CEntity*>	_entitys;

		float				_screenRotation;
	};

}

#endif /* CAMERATEST_H_ */
