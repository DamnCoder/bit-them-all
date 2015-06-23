/*
 * SubestadoIntro.h
 *
 *  Created on: 26/05/2011
 *      Author: jorge
 */

#ifndef SUBESTADOINTRO_H_
#define SUBESTADOINTRO_H_

#include "EstadoJuego.h"

namespace app {

	class CIntroSubstate : public CGameState::CGameSubstate {
	public:
		CIntroSubstate(
				CBitThemAllApp *app,
				CGameState *gameState,
				logic::ILogicFacade* f,
				int command,
				int introId):
					CGameSubstate(app, gameState, f),
					_command(command),
					_introId(introId)
		{
		}
		virtual ~CIntroSubstate(){}

		void onActivate();
		void keyPressed(const event::KeyEvent& ke);

	private:
		int	_command;
		int	_introId;
	};

	//--------------------------------------------------------------------------------------------------------------------

	class CSubestadoPresentacionRonda : public CGameState::CGameSubstate {
	public:
		CSubestadoPresentacionRonda(
				CBitThemAllApp *app,
				CGameState *gameState,
				logic::ILogicFacade* f):
					CGameSubstate(app, gameState, f)
		{
		}

		virtual ~CSubestadoPresentacionRonda(){}

		void onActivate() {
			//printf("Activado Subestado Presentacion Ronda\n");
		}
		void keyPressed(const event::KeyEvent& ke);
	};

	//--------------------------------------------------------------------------------------------------------------------

	class CPlayingState : public CGameState::CGameSubstate {
	public:

		CPlayingState(
				CBitThemAllApp *app,
				CGameState *gameState,
				logic::ILogicFacade* f):
					CGameSubstate(app, gameState, f)
		{
		}

		virtual ~CPlayingState(){}

		void onActivate();
		void keyPressed(const event::KeyEvent& ke);
	};

	//--------------------------------------------------------------------------------------------------------------------

	class CPauseSubstate : public CGameState::CGameSubstate {
	public:

		CPauseSubstate(
				CBitThemAllApp *app,
				CGameState *gameState,
				logic::ILogicFacade* f):
					CGameSubstate(app, gameState, f)
		{
		}

		virtual ~CPauseSubstate(){}

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void keyPressed(const event::KeyEvent& ke);
		void mouseReleased(const event::MouseEvent& me);

	private:
		// Funciones privadas
		void createPauseGui();
		void destroyPauseGui();

		// Atributos
		logic::CFrame*	_fPauseGui;
		logic::CLabel*	_lPause;
		logic::CButton*	_bReturnToGame;
		logic::CButton*	_bReturnToMenu;

	};
}

#endif /* SUBESTADOINTRO_H_ */
