/*
 * IdleNode.h
 *
 *  Created on: 09/04/2011
 *      Author: jorge
 */

#ifndef IDLENODE_H_
#define IDLENODE_H_

#include "BtNode.h"

#include "logic/Game.h"

namespace logic {

	class CScreen;

	class CNormalMode;

	class CDoNothingNode : public CGameNode {
	public:
		CDoNothingNode(){}
		~CDoNothingNode(){}

		BEHAVIOR_STATUS execute(void* agent){ return BT_RUNNING; }
	};

	//--------------------------------------------------------------------------------------------

	class CIdleNode : public CGameNode {
	public:
		CIdleNode(int secs);
		CIdleNode(float millisecs);
		~CIdleNode();

		BEHAVIOR_STATUS execute(void* agent);
		void 			init(void* agent);

	private:
		float	_millisecs;
		float	_elapsedMillisecs;
	};

	//--------------------------------------------------------------------------------------------

	class CRandomIdleNode : public CGameNode {
	public:
		CRandomIdleNode(float maxMillisecs);
		~CRandomIdleNode();

		BEHAVIOR_STATUS execute(void*);
		void 			init(void* agent);

	private:
		float	_maxMillisecs;
		float	_idleMillisecs;
		float	_elapsedMillisecs;
	};

	//--------------------------------------------------------------------------------------------

	class CSkinChangeNode : public CGameNode {
	public:
		CSkinChangeNode(logic::CEntity* entity, const std::string& skin);
		~CSkinChangeNode(){}

		BEHAVIOR_STATUS execute(void*);

	private:
		logic::CEntity*	_entity;
		std::string 	_skin;
	};

	class CScreenVideoNode : public CGameNode {
	public:
		CScreenVideoNode(logic::CScreen* screen, const std::string& videoName);
		~CScreenVideoNode(){}

		BEHAVIOR_STATUS execute(void*);

	private:
		logic::CScreen*	_screen;
		std::string 	_videoName;
	};

	//--------------------------------------------------------------------------------------------

	class CAskQuestionNode : public CGameNode {
	public:
		CAskQuestionNode(logic::CScreen* screen, const std::string& skin);
		~CAskQuestionNode(){}

		BEHAVIOR_STATUS execute(void*);

	private:
		logic::CScreen*	_screen;
		std::string 	_skin;
	};

	//--------------------------------------------------------------------------------------------

	class CInsertCommand : public CGameNode {
	public:
		CInsertCommand(tCommand command);
		~CInsertCommand(){}

		BEHAVIOR_STATUS execute(void*);

	private:
		tCommand _command;
	};

	//--------------------------------------------------------------------------------------------

	class CCleanHud : public CGameNode {
	public:
		CCleanHud();
		~CCleanHud() {}

		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CEndIntro : public CGameNode {
	public:
		CEndIntro();
		~CEndIntro() {}

		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CChangeStateNode : public CGameNode {
	public:
		CChangeStateNode(const std::string& state);
		~CChangeStateNode() {}

		BEHAVIOR_STATUS execute(void*);

	private:
		std::string	_state;
	};

	//--------------------------------------------------------------------------------------------

	class CWinnersHud : public CGameNode {
	public:
		CWinnersHud(){}
		~CWinnersHud(){}

		BEHAVIOR_STATUS execute(void*);
		void			init(void*);

	};

	//--------------------------------------------------------------------------------------------

	class CReturnToMenu : public CGameNode {
	public:
		CReturnToMenu(){}
		~CReturnToMenu(){}

		BEHAVIOR_STATUS execute(void*);

	};

	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------

	class CModeNode : public CGameNode {
	public:
		CModeNode(CModeBT* mode) : _mode(mode) {}
		virtual ~CModeNode() {}

		virtual BEHAVIOR_STATUS execute(void*) = 0;

	protected:
		CModeBT*	_mode;
	};

	//--------------------------------------------------------------------------------------------

	class CNextRoundNode : public CModeNode {
	public:
		CNextRoundNode(CModeBT* mode) : CModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CNextQuestionNode : public CModeNode {
	public:
		CNextQuestionNode(CModeBT* mode) : CModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CIncTimeNode : public CModeNode {
	public:
		CIncTimeNode(CModeBT* mode) :
			CModeNode(mode),
			_elapsedMillis(0),
			_elapsedMillisAux(0),
			_elapsedSecs(0) {}
		BEHAVIOR_STATUS execute(void*);
	private:
		float	_elapsedMillis;
		float	_elapsedMillisAux;
		int		_elapsedSecs;
	};

	//--------------------------------------------------------------------------------------------

	class CResetTimeNode : public CModeNode {
	public:
		CResetTimeNode(CModeBT* mode) : CModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CIncorrectAnswer : public CModeNode {
	public:
		CIncorrectAnswer(CModeBT* mode) : CModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	// Nodos especificos para dialogos del presentador en los modos de juego
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	class CDialogue : public CModeNode {
	public:
		CDialogue(CModeBT* mode) : CModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*) = 0;
	protected:
		std::string	_dialogue;
	};

	class CCreateRankingDialogue : public CDialogue {
	public:
		CCreateRankingDialogue(CModeBT* mode) : CDialogue(mode) {}

		BEHAVIOR_STATUS	execute(void*);
	};

	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------
	// Nodos especificos para modificacion de estado del modo normal
	//--------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------

	class CNormalModeNode : public CModeNode {
	public:
		CNormalModeNode(CNormalMode* mode) : CModeNode((CModeBT*)mode), _normalMode(mode) {}
		virtual ~CNormalModeNode() {}

		virtual BEHAVIOR_STATUS execute(void*) = 0;

	protected:
		CNormalMode*	_normalMode;
	};

	//--------------------------------------------------------------------------------------------

	class CNextPlayerNode : public CNormalModeNode {
	public:
		CNextPlayerNode(CNormalMode* mode) : CNormalModeNode(mode) {}
		BEHAVIOR_STATUS execute(void*);
	};

	//--------------------------------------------------------------------------------------------

	class CSetPointsToCurrentPlayer : public CNormalModeNode {
	public:
		CSetPointsToCurrentPlayer(CNormalMode* mode) : CNormalModeNode(mode) {}
		virtual ~CSetPointsToCurrentPlayer(){}

		BEHAVIOR_STATUS	execute	(void* agent);

	};

	//--------------------------------------------------------------------------------
	// Funciones externas
	//--------------------------------------------------------------------------------



}

#endif /* IDLENODE_H_ */
