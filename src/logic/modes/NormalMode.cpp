/*
 * NormalMode.cpp
 *
 *  Created on: 21/06/2011
 *      Author: jorge
 */

#include "NormalMode.h"

#include "logic/modes/nodes/GameNodes.h"
#include "logic/modes/nodes/CameraNodes.h"
#include "logic/modes/nodes/AudioNodes.h"
#include "logic/modes/nodes/UiNodes.h"
#include "logic/modes/nodes/ConditionNodes.h"
#include "logic/modes/nodes/AnimationNodes.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Label.h"
#include "logic/ui/TextLayout.h"

#include "logic/QuestionManager.h"

#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Host.h"

namespace logic {

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------

	CNormalMode::CNormalMode(
			const std::string& explanation,
			CGame* game,
			BehaviorTreeNode* bt):
		CModeBT(explanation, game, bt),
		_currentPlayer(0)
	{
		/*
		_intros.push_back(
				new CIntroBehavior(game, presentationSequence(), endPresentationSequence()));

		_intros.push_back(
				new CIntroBehavior(game, questionsIntroSequence(), 0));
		 */
		_intros.push_back(presentationSequence());
		_intros.push_back(questionsIntroSequence());

		_behaviors.push_back(roundsSequence());

	}

	bool CNormalMode::checkAnswer() {
		if(_answers.empty()) return false;
		int answer = _answers[_currentQuestion];
		return _questions[_currentQuestion]->isCorrect(answer);
	}

	void CNormalMode::stop() {
		_status = BT_RUNNING;
	}

	void CNormalMode::registerAnswer(int answer) {
		CModeBT::registerAnswer(answer);
		//std::cout<<"[CNormalMode::registerAnswer] Vamos por el jugador "<<_currentPlayer<<"\n";
	}

	BehaviorTreeNode* CNormalMode::presentationSequence() const
	{
		BehaviorTreeInternalNode* modePresentation;

		(modePresentation = new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL))
				->addChild((new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL))
						->addChild(setAudienceAnimation(_scenario, "LEGS_STILL", "TORSO_STILL", "HEAD_SWINGING", 3000))
						->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_IDLE", "HEAD_DOUBLE_BLINKING"))
						->addChild(new COrientFromGivenOrientation(_scenario->host(), 0.0f, 180.0f, 0.07))
						)
				->addChild((new SequentialNode())
						->addChild(new CCameraNode("cam3"))
						->addChild(getDoNothingWhileCondition(new CEndCamerasCondition))
						->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING2"))
						->addChild(getTextBoxNodeWithVisibility(_game->hostTextBox(), _explanation, true))
						->addChild(new CIdleNode(2))
						->addChild(new CChangeStateNode("intro_preguntas"))
						);

		return modePresentation;
	}

	BehaviorTreeNode* CNormalMode::endPresentationSequence() {
		BehaviorTreeInternalNode* modePresentation;

		(modePresentation = new SequentialNode())
				->addChild(new COrientAvatarNode(_game->host(), 180.0f))
				->addChild(getTextBoxNodeWithVisibility(_game->hostTextBox(), _explanation, true))
				->addChild(new CIdleNode(2))
				->addChild(new CChangeStateNode("intro_preguntas"))
				;

		return modePresentation;
	}

	BehaviorTreeNode* CNormalMode::questionsIntroSequence(){
		BehaviorTreeInternalNode* sequence;

		/// Ideal para cuando ganas! ->addChild(new CAnimateAvatarNode(_players[0], "LEGS_NERVOUS", "TORSO_HARD_SWINGING", "HEAD_HARD_SWINGING"))

		(sequence = new SequentialNode())
					// APARICIÓN INTERFAZ
					->addChild(new CCleanHud)
					->addChild(new CTextBoxNode(_game->hostTextBox(), "Sin mas preambulos, que empiece el juego. Adelante!"))
					->addChild(new CSkinChangeNode(_scenario->screenMult(), "empieza_juego"))
					->addChild(new CCameraNode("cam1"))
					->addChild(new CPlayLoopSongNode(3))
					->addChild(new CPlayEffect(1))
					->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_BORED", "HEAD_TURN_LEFT"))
					->addChild(new COrientFromGivenOrientation(_scenario->host(), 135, 360, 0.07))
					->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_MOVING_FINGERS", "HEAD_DOUBLE_BLINKING"))
					->addChild(getDoNothingWhileCondition(new CEndCamerasCondition))
					->addChild(new CChangeStateNode("preguntando"))
				;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::roundsSequence(){
		BehaviorTreeInternalNode* sequence = new SequentialNode();
		for(int r=0; r<numRounds(); ++r){
			sequence->addChild(new CCleanHud);
			sequence->addChild(getPlayerHudVisbility(true));
			for(int p=0; p<numPlayers(); ++p) {
				sequence->addChild(new CResetTimeNode((CModeBT*)this));
				sequence->addChild(askQuestionToPlayerSequence(p, r));
				sequence->addChild(answerSequence());
				sequence->addChild(new CSetButtonStatus(_bColorAnswers[0], CButton::NORMAL));		/// Limpio las respuestas
				sequence->addChild(new CSetButtonStatus(_bColorAnswers[1], CButton::NORMAL));
				sequence->addChild(new CSetButtonStatus(_bColorAnswers[2], CButton::NORMAL));
				sequence->addChild(new CSetButtonStatus(_bColorAnswers[3], CButton::NORMAL));
				sequence->addChild(new CRandomIdleNode(1000));
				sequence->addChild(new CAnimateAvatarNode(_players[p], "LEGS_SWINGING", "TORSO_SWINGING", "HEAD_IDLE"));
				sequence->addChild(new CSetButtonStatus(_bPlayerName[p], CButton::NORMAL));
				sequence->addChild(new CNextQuestionNode(this));
				sequence->addChild(new CNextPlayerNode(this));
				//std::cout<<"Jugador: "<<p<<" Pregunta: "<<p+(numPlayers()*r)<<" en ronda: "<<r<<"\n";
			}

			sequence->addChild(new CNextRoundNode(this));

			sequence->addChild((new CConditionNode(new CEndModeCondition))		/// Compruebo si quedan rondas
					->addChild((new SequentialNode())
							->addChild(new CClearCameraQueue)
							->addChild(new COrientAvatarNode(_host,180.0f))
							->addChild(new CChangeStateNode("intro_ganadores"))
							)
					);
			sequence->addChild((new CInvertedConditionNode(new CEndModeCondition))
					->addChild(scoreReviewSequence())
					);
			sequence->addChild(new CClearCameraQueue);
		}

		return sequence;
	}

	BehaviorTreeNode* CNormalMode::answerSequence(){
		BehaviorTreeInternalNode* sequence;

		/// Secuencia de respuesta para un jugador
		(sequence = new SequentialNode())
									/// Secuencia de respuesta
				->addChild((new CInterruptibleByConditionNode(new CRegisteredAnswer))
						->addChild((new CInterruptibleByConditionNode(new CEndTimeCondition(this)))	/// Mientras haya tiempo
								->addChild((new RepeatNode(-1))
										->addChild(new CIncTimeNode(this))							/// Incrementamos el tiempo
										)
								)
				)/// Acciones en funcion de lo que ha pasado en el turno de pregunta
				->addChild((new CConditionNode(new CRegisteredAnswer))
						->addChild((new SequentialNode())
								->addChild(new CAnimateCurrentPlayerNode("LEGS_IDLE","TORSO_PUSH_BUTTON","HEAD_JUMP"))
								->addChild(new CIdleNode(1))
								)
						)
				->addChild((new CConditionNode(new CEndTimeCondition(this)))		/// Si se ha terminado el tiempo
						->addChild(endOfTimeSequence())								/// Establecemos una respuesta incorrecta
						)
				->addChild((new CConditionNode(new CCorrectAnswer(this)))			/// Si la respuesta es correcta
						->addChild(questionSuccessSequence())
						)
				->addChild((new CInvertedConditionNode(new CCorrectAnswer(this)))	/// Si la respuesta es incorrecta
						->addChild(questionFailureSequence())
						)
				->addChild(new CIdleNode(4)) 										/// Tiempo de espera en milisegundos
				;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::askQuestionToPlayerSequence(int player, int round) {

		std::string playerCam = (std::string)"player"+data2string(player+1)+(std::string)"cam1";
		BehaviorTreeInternalNode* sequence;
		(sequence = new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL))
				->addChild((new SequentialNode())
						->addChild(new CPlayDialogue(_questions[player+(numPlayers()*round)]->getId()))
						->addChild(new CAnimateAvatarNode(_players[player], "LEGS_NERVOUS", "TORSO_JUMP", "HEAD_HARD_SWINGING"))
						->addChild(new CSetButtonStatus(_bPlayerName[player], CButton::HIGHLIGHTED))
						->addChild(new CCameraNode(playerCam))		// Camara a jugador 1
						->addChild(new CSetQuestionInHud(_questions[player+(numPlayers()*round)]))
						->addChild(new CUiComponentVisibility(_game->questionsGui(), true))	// Muestra interfaz pregunta
						->addChild(new CFadeUiNode(_fQuestionsGui, 0.001f))					// Hacemos un fade para mostrar el interfaz
				)
				//->addChild(new CTimerButtonNode(_bClock, _timePerQuestion))
			;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::endOfTimeSequence()
	{
		BehaviorTreeInternalNode* sequence;
		// SECUENCIA DE ANIMACION FALLO PREGUNTA
		(sequence = new SequentialNode())
				->addChild(new CIncorrectAnswer(this))
				->addChild(new CAnimateCurrentPlayerNode("LEGS_STILL", "TORSO_AGREE", "HEAD_STILL"))
				->addChild(new CPlayEffect(3))			/// Sonido respuesta fallada
		;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::questionSuccessSequence()
	{
		BehaviorTreeInternalNode* sequence;
		// SECUENCIA DE ANIMACION FALLO PREGUNTA
		(sequence = new SequentialNode())
				->addChild(new CAnimateCurrentPlayerNode("LEGS_JUMP", "TORSO_HARD_SWINGING", "HEAD_TURN_RIGHT"))
				->addChild(new CPlayEffect(2))										/// Sonido victoria
				->addChild(new CSetPointsToCurrentPlayer(this))	/// Añadimos el tiempo al gui
		;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::questionFailureSequence() const
	{
		BehaviorTreeInternalNode* sequence;
		// SECUENCIA DE ANIMACION FALLO PREGUNTA
		(sequence = new SequentialNode())
				->addChild(new CAnimateCurrentPlayerNode("LEGS_STILL", "TORSO_AGREE", "HEAD_STILL"))
				->addChild(new CPlayEffect(3))			/// Sonido respuesta fallada
		;
		return sequence;
	}

	BehaviorTreeNode* CNormalMode::scoreReviewSequence() {
		BehaviorTreeInternalNode* sequence;

		(sequence = new SequentialNode)
				->addChild(new CCleanHud())
				->addChild(new COrientAvatarNode(_game->host(), 140.0f))
				->addChild(new CAnimateAvatarNode(
						_scenario->host(),
						"LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING"))	/// Establece animacion de presentador hablando
				->addChild(getTextBoxNodeWithVisibility(_game->hostTextBox(), "Esta ronda ha sido muy emocionante!", true))
				->addChild(new CCameraNode("cam9"))
				->addChild(new CIdleNode(4))
				->addChild(getTextBoxNodeWithVisibility(_game->hostTextBox(), "Repasemos las puntuaciones, para comprobar cómo está la situación.", true))
				->addChild(new CIdleNode(4))
				->addChild(new CCreateRankingDialogue(this))
				->addChild(new CIdleNode(4))
				->addChild(getDoNothingWhileCondition(new CEndCamerasCondition))
				;
		return sequence;
	}
}
