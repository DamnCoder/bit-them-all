/*
 * UiNodes.h
 *
 *  Created on: 23/05/2011
 *      Author: jorge
 */

#ifndef UINODES_H_
#define UINODES_H_

#include "BtNode.h"

namespace logic {

	class CUiComponent;
	class CTextBox;
	class CButton;
	class CQuestion;

	class CBlinkingUiNode : public CGameNode {
	public:
		CBlinkingUiNode(CUiComponent* uiComp, float speed);
		virtual ~CBlinkingUiNode();

		BEHAVIOR_STATUS	execute	(void* agent);
		void 			init	(void* agent);

	private:
		CUiComponent* _uiComponent;
		float			_speed;	// Velocidad a la que crece o decrece el alpha
		float			_alpha;	// Alpha de la etiqueta en un momento determinado
	};

	//--------------------------------------------------------------------------------------------

	class CFadeUiNode : public CGameNode {
	public:
		CFadeUiNode(CUiComponent* uiComp, float speed);
		virtual ~CFadeUiNode();

		BEHAVIOR_STATUS	execute	(void* agent);
		void 			init	(void* agent);

	private:
		const bool endCondition() const;
		CUiComponent* _uiComponent;
		float			_speed;	// Velocidad a la que crece o decrece el alpha
		float			_alpha;	// Alpha de la etiqueta en un momento determinado
	};

	//--------------------------------------------------------------------------------------------

	class CSetAlphaNode : public CGameNode {
	public:
		CSetAlphaNode(CUiComponent* uiComp, float alpha);
		virtual ~CSetAlphaNode();

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		CUiComponent* _uiComponent;
		float			_alpha;	// Alpha de la etiqueta en un momento determinado
	};
	//--------------------------------------------------------------------------------------------

	class CUiComponentVisibility : public CGameNode {
	public:
		CUiComponentVisibility(CUiComponent* uiComp, bool visible);
		virtual ~CUiComponentVisibility(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		CUiComponent* 	_uiComp;
		bool 			_visible;
	};

	//--------------------------------------------------------------------------------------------

	class CTextBoxNode : public CGameNode {
	public:
		CTextBoxNode(CTextBox* textBox, const std::string& text);
		virtual ~CTextBoxNode(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		CTextBox* 	_textBox;
		std::string	_text;
	};

	//--------------------------------------------------------------------------------------------

	class CSetButtonStatus : public CGameNode {
	public:
		CSetButtonStatus(CButton* button, int status);
		virtual ~CSetButtonStatus(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		CButton* 	_button;
		int			_status;
	};

	//--------------------------------------------------------------------------------------------

	class CTimerButtonNode : public CGameNode {
	public:
		CTimerButtonNode(CButton* button, int secs);
		virtual ~CTimerButtonNode(){}

		BEHAVIOR_STATUS	execute	(void* agent);
		void 			init	(void* agent);

	private:
		CButton* 	_button;

		int		_currentSec;
		int		_maxSecs;
		float	_elapsedMillisecs;
	};
	//--------------------------------------------------------------------------------------------

	class CPlayerHudVisibility : public CGameNode {
	public:
		CPlayerHudVisibility(int player, bool visible);
		virtual ~CPlayerHudVisibility(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		int		_numPlayer;
		bool	_visible;
	};

	//--------------------------------------------------------------------------------------------

	class CSetQuestionInHud : public CGameNode {
	public:
		CSetQuestionInHud(CQuestion* question);
		virtual ~CSetQuestionInHud(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	private:
		CQuestion* _question;
	};

	//--------------------------------------------------------------------------------
	// Funciones externas
	//--------------------------------------------------------------------------------

	BehaviorTreeNode* getTextBoxNodeWithVisibility(CTextBox* textBox, const std::string& text, const bool visible);

	BehaviorTreeNode* getPlayerHudVisbility(const bool visible);

}

#endif /* UINODES_H_ */
