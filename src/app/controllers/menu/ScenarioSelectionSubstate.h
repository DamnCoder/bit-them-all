/*
 * ScenarioSelectionSubstate.h
 *
 *  Created on: 05/12/2010
 *      Author: jorge
 */

#ifndef SCENARIOSELECTIONSUBSTATE_H_
#define SCENARIOSELECTIONSUBSTATE_H_

#include "MenuState.h"

namespace app {

	class CScenarioSelectionSubstate:
		public CMenuState::CMenuSubstate{

	public:

		CScenarioSelectionSubstate(
				CBitThemAllApp *app,
				CMenuState *menuState,
				ILogicFacade* f):
					CMenuSubstate(app, menuState, f),
					_scenarioBackground(0)
		{}

		virtual ~CScenarioSelectionSubstate();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void mouseReleased(const event::MouseEvent& me);
		void keyReleased(const event::KeyEvent& ke);

	private:
		void nextScenario();
		void previousScenario();

		void createScenarioHud();

		CFrame*							_scenarioFrame;
		CButton* 						_acceptButton;

		CManager<CScenario* >*	_scenarioManager;

		CScenario*						_scenarioBackground;

	};

}

#endif /* SCENARIOSELECTIONSUBSTATE_H_ */
