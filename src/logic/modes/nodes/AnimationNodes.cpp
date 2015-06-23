/*
 * AnimationNodes.cpp
 *
 *  Created on: 27/05/2011
 *      Author: jorge
 */

#include "AnimationNodes.h"

#include "logic/Game.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Scenario.h"

#include "GameNodes.h"

namespace logic {
//-----------------------------------------------------------------------------------------------------------------------------------

	CAnimateAvatarNode::CAnimateAvatarNode(
			CAvatar* avatar,
			const std::string& legsAnimation,
			const std::string& torsoAnimation,
			const std::string& headAnimation):
			_legsAnimation(legsAnimation),
			_torsoAnimation(torsoAnimation),
			_headAnimation(headAnimation),
			_avatar(avatar)
	{}

	BEHAVIOR_STATUS CAnimateAvatarNode::execute(void* agent){
		_avatar->setLegsAnimation(_legsAnimation);
		_avatar->setTorsoAnimation(_torsoAnimation);
		_avatar->setHeadAnimation(_headAnimation);
		//std::cout<<"[CAnimateAvatarNode::execute] Establecida animacion personaje: "<<_avatar->id()<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CAnimateCurrentPlayerNode::CAnimateCurrentPlayerNode(
			const std::string& legsAnimation,
			const std::string& torsoAnimation,
			const std::string& headAnimation):
				CAnimateAvatarNode(0, legsAnimation, torsoAnimation, headAnimation)
	{}

	BEHAVIOR_STATUS CAnimateCurrentPlayerNode::execute(void* agent){
		_data = (tGameData*)agent;
		_avatar = _data->game->currentPlayer();
		_avatar->setLegsAnimation(_legsAnimation);
		_avatar->setTorsoAnimation(_torsoAnimation);
		_avatar->setHeadAnimation(_headAnimation);
		//std::cout<<"[CAnimateAvatarNode::execute] Establecida animacion personaje: "<<_avatar->id()<<"\n";
		return BT_SUCCESS;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	CRandomAnimationNode::CRandomAnimationNode(CAvatar* avatar):
		CAnimateAvatarNode(avatar, "", "", "")
	{}

	BEHAVIOR_STATUS CRandomAnimationNode::execute(void* agent){
		_avatar->setLegsAnimation(_legsAnimation);
		_avatar->setTorsoAnimation(_torsoAnimation);
		_avatar->setHeadAnimation(_headAnimation);

		//std::cout<<"[CRandomAnimationNode::execute] Establecida animacion personaje: "<<_avatar->id()<<"\n";
		//std::cout<<"[CRandomAnimationNode::execute] Piernas: "<<_avatar->legsAnimation()<<"\n";
		//std::cout<<"[CRandomAnimationNode::execute] Torso: "<<_avatar->torsoAnimation()<<"\n";
		//std::cout<<"[CRandomAnimationNode::execute] Cabeza: "<<_avatar->headAnimation()<<"\n";
		return BT_SUCCESS;
	}

	void CRandomAnimationNode::init(void* agent){
		_data = (tGameData*)agent;
		//std::cout<<"Iniciando nodo Animar Aleatoriamente Avatar\n";
		TAnimationsMap animationsMap = _data->game->animationsMap();

		std::string id = splitAndReturnFirstPart('@', _avatar->id());
		//std::cout<<"Animacion aleatoria para : "<<id<<"\n";
		CAnimationManager* animManager = animationsMap[id];

		_legsAnimation = animManager->_legsAnimations.getRandom();
		_torsoAnimation = animManager->_torsoAnimations.getRandom();
		_headAnimation = animManager->_headAnimations.getRandom();
	}

//-----------------------------------------------------------------------------------------------------------------------------------

	COrientAvatarNode::COrientAvatarNode(CAvatar* avatar, float orientation):
			_firstTime(true),
			_orientation(orientation),
			_accumOrient(0),
			_speed(0),
			_avatar(avatar)
	{}

	COrientAvatarNode::COrientAvatarNode(CAvatar* avatar, float orientation, float speed):
			_firstTime(true),
			_orientation(orientation),
			_accumOrient(0),
			_speed(speed),
			_avatar(avatar)
	{}

	BEHAVIOR_STATUS COrientAvatarNode::execute(void* agent){
		_data = (tGameData*)agent;

		if(_speed == 0) {
			_avatar->setOrientation(_orientation);
			return BT_SUCCESS;
		}

		if(_firstTime){
			_originalOrientation = _accumOrient = _avatar->orientation();
			_firstTime = false;
		}

		_accumOrient += _speed * _data->dt;
		_avatar->setOrientation(_accumOrient);
		//std::cout<<"[COrientAvatarNode::execute] Orientacion "<<_accumOrient<<" grados\n";

		if((_accumOrient - _originalOrientation)>_orientation){
			_accumOrient = 0;
			//std::cout<<"[COrientAvatarNode::execute] Personaje rotado!\n";
			return BT_SUCCESS;
		}

		return BT_RUNNING;
	}

	void COrientAvatarNode::init(void* agent){
		//std::cout<<"Iniciando nodo Orientar Avatar\n";
		_firstTime = true;
		//_originalOrientation = _accumOrient = _avatar->orientation();
	}


//-----------------------------------------------------------------------------------------------------------------------------------

	COrientFromGivenOrientation::COrientFromGivenOrientation(
			CAvatar* avatar,
			float initOrientation,
			float endOrientation,
			float speed):
				_firstTime(true),
				_initOrientation(initOrientation),
				_endOrientation(endOrientation),
				_accumOrient(0),
				_speed(speed),
				_avatar(avatar)
	{}

	BEHAVIOR_STATUS COrientFromGivenOrientation::execute(void* agent){
		_data = (tGameData*)agent;

		if(_firstTime){
			_avatar->setOrientation(_initOrientation);

			_accumOrient = _avatar->orientation();
			_firstTime = false;
		}

		_accumOrient += _speed * _data->dt;

		_avatar->setOrientation(_accumOrient);
		//std::cout<<"[COrientAvatarNode::execute] Orientacion "<<_accumOrient<<" grados\n";

		if(endOrientCondition()){
			_accumOrient = 0;
			//std::cout<<"[COrientAvatarNode::execute] Personaje rotado!\n";
			return BT_SUCCESS;
		}

		return BT_RUNNING;
	}

	void COrientFromGivenOrientation::init(void* agent){
		//std::cout<<"Iniciando nodo Orientar Avatar\n";
		_firstTime = true;
		//_originalOrientation = _accumOrient = _avatar->orientation();
	}

	bool COrientFromGivenOrientation::endOrientCondition() {
		bool result = false;
		if(_initOrientation < _endOrientation){
			result = _accumOrient >= _endOrientation;
		}
		else if(_initOrientation > _endOrientation){
			result = _accumOrient <= _endOrientation;
		}
		return result;
	}

//-----------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------
// FUNCIONES EXTERNAS
//-----------------------------------------------------------------------------------------------------------------------------------

	BehaviorTreeNode* getIdleAnimNode(CAvatar* avatar) {
		BehaviorTreeInternalNode* animation;

		CGameNode* idle = new CRandomIdleNode(1000.0f);

		(animation = new SequentialNode())
				->addChild(idle)
				//->addChild(new CAnimateAvatarNode(avatar,"LEGS_IDLE","TORSO_SWINGING","HEAD_SWINGING"))
				->addChild(new CRandomAnimationNode(avatar))
				;

		return animation;
	}

	BehaviorTreeNode* getIdleAnimNode(
			CAvatar* avatar,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs)
	{
		BehaviorTreeInternalNode* animation;
		(animation = new SequentialNode())
				->addChild(new CRandomIdleNode(maxMillisecs))
				->addChild(new CAnimateAvatarNode(
						avatar,
						legs, torso,head))
				;

		return animation;
	}

	/**
	 * Devuelve un arbol que ejecuta todos sus nodos en paralelo.
	 * Cada nodo anima un avatar de la audiencia con
	 * un retraso aleatorio antes de que se ejecute.
	 * La animacion es aleatoria.
	 *
	 * @param scenario
	 * @return
	 */
	BehaviorTreeNode* randomAudienceAnimation(const CScenario* scenario) {
		BehaviorTreeInternalNode* animation = new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL);
		vector<CAvatar*> audience = scenario->audience();
		vector<CGameNode*> audienceAnimNodes;
		audienceAnimNodes.reserve(audience.size());

		vector<CAvatar*>::iterator audIT(audience.begin()), audEnd(audience.end());
		for(; audIT!=audEnd; ++audIT){
			animation->addChild(getIdleAnimNode((*audIT)));
		}
		return animation;
	}

	/**
	 * Devuelve un arbol que ejecuta todos sus nodos en paralelo.
	 * Cada nodo anima un avatar de la audiencia con
	 * un retraso aleatorio antes de que se ejecute.
	 * La animacion es la misma para todos.
	 *
	 * @param scenario
	 * @param legs
	 * @param torso
	 * @param head
	 * @return
	 */
	BehaviorTreeNode* setAudienceAnimation(
			const CScenario* scenario,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs)
	{
		BehaviorTreeInternalNode* animation = new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL);
		vector<CAvatar*> audience = scenario->audience();
		vector<CGameNode*> audienceAnimNodes;
		audienceAnimNodes.reserve(audience.size());

		vector<CAvatar*>::iterator audIT(audience.begin()), audEnd(audience.end());
		for(; audIT!=audEnd; ++audIT){
			animation->addChild(
					getIdleAnimNode(
							(*audIT),
							legs, torso, head,
							maxMillisecs)
							);
		}
		return animation;
	}

	/**
	 * Devuelve un arbol que anima a todos a los jugadores
	 * @param scenario
	 * @param legs
	 * @param torso
	 * @param head
	 * @param maxMillisecs
	 * @return
	 */
	BehaviorTreeNode* setAllPlayersAnimation(
			const CScenario* scenario,
			const std::string& legs,
			const std::string& torso,
			const std::string& head,
			const float maxMillisecs)
	{
		BehaviorTreeInternalNode* animation = new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL);
		vector<CPlayer*> players = scenario->players();
		vector<CGameNode*> animNodes;
		animNodes.reserve(players.size());

		vector<CPlayer*>::iterator audIT(players.begin()), audEnd(players.end());
		for(; audIT!=audEnd; ++audIT){
			animation->addChild(
					getIdleAnimNode(
							(*audIT),
							legs, torso, head,
							maxMillisecs)
							);
		}
		return animation;
	}
}
