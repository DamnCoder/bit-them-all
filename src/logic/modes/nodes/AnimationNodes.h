/*
 * AnimationNodes.h
 *
 *  Created on: 27/05/2011
 *      Author: jorge
 */

#ifndef ANIMATIONNODES_H_
#define ANIMATIONNODES_H_

#include "BtNode.h"



namespace logic {

	class CAvatar;
	class CScenario;

	//---------------------------------------------------------------------------------------------------------------------

	class CAnimateAvatarNode : public CGameNode {
	public:
		CAnimateAvatarNode(
				CAvatar* avatar,
				const std::string& legsAnimation,
				const std::string& torsoAnimation,
				const std::string& headAnimation);
		~CAnimateAvatarNode(){}

		BEHAVIOR_STATUS execute(void* agent);

	protected:
		std::string	_legsAnimation;
		std::string	_torsoAnimation;
		std::string	_headAnimation;
		CAvatar*	_avatar;

	};

	class CAnimateCurrentPlayerNode : public CAnimateAvatarNode {
	public:
		CAnimateCurrentPlayerNode(
				const std::string& legsAnimation,
				const std::string& torsoAnimation,
				const std::string& headAnimation);
		~CAnimateCurrentPlayerNode(){}

		BEHAVIOR_STATUS execute(void* agent);
	};

	class CRandomAnimationNode : public CAnimateAvatarNode {
	public:
		CRandomAnimationNode(CAvatar* avatar);
		~CRandomAnimationNode(){}

		BEHAVIOR_STATUS execute(void* agent);
		void 			init(void* agent);

	};

	//---------------------------------------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------

	class COrientAvatarNode : public CGameNode {
	public:
		COrientAvatarNode(CAvatar* avatar, float orientation);
		COrientAvatarNode(CAvatar* avatar, float orientation, float speed);
		~COrientAvatarNode(){}

		BEHAVIOR_STATUS execute(void* agent);
		void 			init(void* agent);

	private:

		bool		_firstTime;

		float		_orientation;
		float		_originalOrientation;
		float		_accumOrient;
		float		_speed;
		CAvatar*	_avatar;
	};

	//---------------------------------------------------------------------------------------------------------------------

	class COrientFromGivenOrientation : public CGameNode {
	public:
		COrientFromGivenOrientation(
				CAvatar* avatar,
				float initOrientation,
				float endOrientation,
				float speed);
		~COrientFromGivenOrientation(){}

		BEHAVIOR_STATUS execute(void* agent);
		void 			init(void* agent);

	private:

		bool endOrientCondition();

		bool		_firstTime;

		float		_initOrientation;
		float		_endOrientation;

		float		_accumOrient;
		float		_speed;
		CAvatar*	_avatar;
	};


	//---------------------------------------------------------------------------------------------------------------------
	// FUNCIONES EXTERNAS
	//-------------------------------------------------------------------------------
	BehaviorTreeNode* getIdleAnimNode(CAvatar* avatar);
	BehaviorTreeNode* getIdleAnimNode(
			CAvatar* avatar,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs);


	BehaviorTreeNode* randomAudienceAnimation(const CScenario* scenario);
	BehaviorTreeNode* setAudienceAnimation(
			const CScenario* scenario,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs);

	BehaviorTreeNode* setAllPlayersAnimation(
			const CScenario* scenario,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs);

}

#endif /* ANIMATIONNODES_H_ */
