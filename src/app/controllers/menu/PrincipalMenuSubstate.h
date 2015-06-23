/**
 * @file PrincipalMenuSubstate.h
 *
 * Contiene la declaración de la clase que representa un subestado del
 * estado de la aplicacion del menu
 * <br>
 *
 * @see aplicacion::CBTASubstate
 * @see aplicacion::CBTAComposeState
 * @see aplicacion::CAplicacion
 *
 * @author Jorge Lopez Gonzalez
 * @date Enero, 2010
 */
#ifndef PRINCIPALMENUSUBSTATE_H_
#define PRINCIPALMENUSUBSTATE_H_

#include "MenuState.h"

namespace app {

	class CPrincipalMenuSubstate:
		public CMenuState::CMenuSubstate{

	public:

		CPrincipalMenuSubstate(
				CBitThemAllApp *app,
				CMenuState *menuState,
				logic::ILogicFacade* f):
					CMenuSubstate(app, menuState, f)
		{}

		virtual ~CPrincipalMenuSubstate();

		void onCreate();
		void onDestroy();
		void onActivate();
		void onDeactivate();

		void mouseReleased(const event::MouseEvent& me);
		void keyReleased(const event::KeyEvent& ke);

	protected:

		/**
		 * Método reimplementado por las clases hijas, y que ejecuta la lógica
		 * asociada a este estado. Es llamada antes del dibujado de la
		 * escena.
		 * @param dt Tiempo transcurrido en milisegundos desde la última
		 * actualización de la lógica
		 */
		void executeLogic(float dt);

	private:

		void createPrincipalMenuHud();

		void setRandomPlayers();
		void setRandomAudience();

		logic::CButton* 	_initGameBtn;
		logic::CButton*		_menuTestBtn;
		logic::CButton* 	_menuScenaryBtn;
		logic::CButton* 	_menuCharacterBtn;
		logic::CButton* 	_endAppBtn;

		logic::CFrame* 		_menuFrame;

		logic::CScenario*	_backgroundScenario;
	};

}

#endif /* PRINCIPALMENUSUBSTATE_H_ */
