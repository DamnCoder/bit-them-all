/*
 * ConditionNodes.h
 *
 *  Created on: 25/05/2011
 *      Author: jorge
 */

#ifndef CONDITIONNODES_H_
#define CONDITIONNODES_H_

#include "libbt/BehaviorTree.h"

using namespace BehaviorTree;

namespace logic {

	class CModeBT;
	class CNormalMode;

	/**
	 * Clase Condicion.
	 *
	 * Chequea si se cumple una condicion y
	 * devuelve el valor booleano correspondiente
	 */
	class CCondition {
	public:
		~CCondition() {}

		virtual bool check(void*) = 0;
	};

	class CIntroEndCondition : public CCondition {
	public:
		bool check(void*);
	};

	class CEndModeCondition : public CCondition {
	public:
		bool check(void*);
	};

	class CEndCamerasCondition : public CCondition {
	public:
		bool check(void*);
	};

	class CRegisteredAnswer : public CCondition  {
	public:
		bool check(void*);
	};

	/**
	 * Clase Condicion de Modo de Juego.
	 *
	 * Comprueba si se cumple una condicion en un modo de juego.
	 */
	class CModeCondition : public CCondition {
	public:
		CModeCondition(CModeBT* mode) : _mode(mode) {}
		virtual bool check(void*) = 0;
	protected:
		CModeBT*	_mode;
	};

	class CNoMoreRoundsCondition : public CModeCondition {
	public:
		CNoMoreRoundsCondition(CModeBT* mode) : CModeCondition(mode) {}
		bool check(void*);
	};

	class CEndTimeCondition : public CModeCondition {
	public:
		CEndTimeCondition(CModeBT* mode) : CModeCondition(mode) {}
		bool check(void*);
	};

	class CCorrectAnswer : public CModeCondition {
	public:
		CCorrectAnswer(CModeBT* mode) : CModeCondition(mode) {}
		bool check(void*);
	};

	/**
	 * Comprueba si se cumple una condicion en
	 * el modo de juego normal.
	 */
	class CNormalModeCondition : public CModeCondition {
	public:
		CNormalModeCondition(CNormalMode* mode) :
			CModeCondition((CModeBT*)mode),
			_normalMode(mode) {}
		virtual bool check(void*) = 0;
	protected:
		CNormalMode*	_normalMode;
	};

	class CNoMorePlayersCondition : public CNormalModeCondition {
	public:
		CNoMorePlayersCondition(CNormalMode* mode) : CNormalModeCondition(mode) {}
		bool check(void*);
	};

//-----------------------------------------------------------------------------------------------------------------------------------

	/**
	 * El nodo contiene un arbol que ejecuta mientras
	 * no se cumpla la condicion establecida.
	 *
	 * Si se cumple la condicion devuelve EXITO
	 * Si no devuelve el estatus del nodo hijo
	 */
	class CInterruptibleByConditionNode : public BehaviorTreeInternalNode {
	public:
		CInterruptibleByConditionNode(CCondition* condition):
			_condition(condition),
			_init(false)
		{}

		~CInterruptibleByConditionNode()
		{ delete _condition; }

		BEHAVIOR_STATUS				execute(void*);
		void						init(void*);
		BehaviorTreeInternalNode*	addChild(BehaviorTreeNode*);

	private:

		CCondition* _condition;
		bool		_init;
	};

	/**
	 * El nodo contiene un arbol que no ejecuta a menos que
	 * se cumpla la condicion.
	 *
	 * Devuelve fallo si al ejecutarse no se cumple la condicion,
	 * si se cumple devuelve el resultado de ejcutar al hijo
	 */
	class CConditionNode : public BehaviorTreeInternalNode {
	public:

		CConditionNode(CCondition* condition) :
			_condition(condition),
			_init(false)
		{};

		virtual ~CConditionNode(void)
		{
			delete _condition;
		};

		virtual BEHAVIOR_STATUS 	execute(void*);
		void 						init(void*);
		BehaviorTreeInternalNode* 	addChild(BehaviorTreeNode*);

	protected:

		CCondition* _condition;
		bool 		_init;

	};

	class CInvertedConditionNode : public CConditionNode {
	public:
		CInvertedConditionNode(CCondition* condition) : CConditionNode(condition)
		{};

		virtual ~CInvertedConditionNode(void)
		{}

		virtual BEHAVIOR_STATUS 	execute(void*);

	};

//-----------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
// FUNCIONES EXTERNAS
//--------------------------------------------------------------------------------------------

	BehaviorTreeNode* getDoNothingWhileCondition(CCondition* condition);

	BehaviorTreeNode* getDoWhileCondition(CCondition* condition, BehaviorTreeNode* behavior);

	BehaviorTreeNode* getIfThenElse(CCondition* condition, BehaviorTreeNode* bt1, BehaviorTreeNode* bt2);

}

#endif /* CONDITIONNODES_H_ */
