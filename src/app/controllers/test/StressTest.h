/*
 * ShadowTest.h
 *
 *  Created on: 29/12/2010
 *      Author: jorge
 */

#ifndef SHADOWTEST_H_
#define SHADOWTEST_H_

#include "app/controllers/YaoController.h"
#include "app/BitThemAllApp.h"

namespace logic{
	class CEntity;
	class CPlayer;
	class CLabel;
}

namespace gui{
	class COglLight;
}

namespace app {

	class CStressTest : public CYaoController{
	public:
		CStressTest(CBitThemAllApp* app);
		virtual ~CStressTest();

		void onCreate();
		void onDestroy();

		void keyPressed(const event::KeyEvent& ke);
		void keyReleased(const event::KeyEvent& ke);

	protected:

		void executeLogic(float dt);
		void render(float dt);

		void createArmy();

	private:

		gui::COglCamera*		_cam;

		gui::COglLight*			_light;

		vector<logic::CPlayer*> _players;
		vector<logic::CEntity*>	_entitys;

		logic::CPlayer*			_player;
		logic::CEntity*			_floor;

		logic::CLabel*			_lSceneTris;

		math::Vec4 lightPosition;

	};

}

#endif /* SHADOWTEST_H_ */
