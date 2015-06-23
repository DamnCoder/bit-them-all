/*
 * CharacterSelection.h
 *
 *  Created on: 27/10/2010
 *      Author: jorge
 */

#ifndef CHARACTERSELECTION_H_
#define CHARACTERSELECTION_H_

#include "MenuState.h"

namespace app {

	class CCharacterSelectionSubstate :
		public CMenuState::CMenuSubstate{

	public:

		CCharacterSelectionSubstate(
				CBitThemAllApp *app,
				CMenuState *menuState,
				logic::ILogicFacade* f):
					CMenuSubstate(app, menuState, f),
					_orientation(100),
					_rotationSpeed(0.1f)
		{}
		virtual ~CCharacterSelectionSubstate();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void mouseReleased(const event::MouseEvent& me);
		void keyReleased(const event::KeyEvent& ke);

	protected:
		void executeLogic(float dt);

	private:
		void nextCharacter();
		void previousCharacter();

		void createCharacterSelectionHud();
		void destroyCharacterSelectionHud();

		void createSelectedCharactersHud();
		void destroySelectedCharactersHud();

		void createIconCharacterHud(int numPlayer, const std::string& name, const std::string& skin);
		void destroyIconsCharactersHud();

		void imprimeCaracteristicasPersonaje();

		gui::COglCamera*		_cam;

		CManager<CPlayer* >*	_playerManager;

		logic::CPlayer*			_currentCharacter;
		logic::CFrame*			_background;

		logic::CFrame*			_fChar[4];
		logic::CFrame*			_fCharFrame[4];

		logic::CButton* 		_returnBtn;
		logic::CButton* 		_previousBtn;
		logic::CButton* 		_nextBtn;
		logic::CButton* 		_selectBtn;

		logic::CLabel*			_lSelectionableChar;

		int						_numChoosedPlayers;

		// Relativo a la rotacion de los personajes
		float					_orientation;
		float					_rotationSpeed;
	};

}

#endif /* CHARACTERSELECTION_H_ */
