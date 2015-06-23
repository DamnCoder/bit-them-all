/*
 * BtNode.h
 *
 *  Created on: 09/04/2011
 *      Author: jorge
 */

#ifndef BTNODE_H_
#define BTNODE_H_

#include "libbt/BehaviorTree.h"

#include <string>
#include <iostream>

using namespace BehaviorTree;

namespace logic{

	class CGame;

	struct tGameData {
		CGame*		game;
		float		dt;
	};

	class CGameNode : public BehaviorTreeNode {
	public:

		virtual ~CGameNode(){}

		virtual BEHAVIOR_STATUS execute(void* agent) = 0;

		virtual void init(void* agent) {}

	protected:

		std::string	_name;
		tGameData*	_data;
	};

}
#endif /* BTNODE_H_ */
