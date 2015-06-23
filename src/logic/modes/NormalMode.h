/*
 * NormalMode.h
 *
 *  Created on: 21/06/2011
 *      Author: jorge
 */

#ifndef NORMALMODE_H_
#define NORMALMODE_H_

#include "ModeBT.h"

namespace logic {

	//----------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------

	class CNormalMode : public CModeBT {
	public:
		CNormalMode(
				const std::string& explanation,
				CGame* game,
				BehaviorTreeNode* bt);
		virtual ~CNormalMode(){}

		bool checkAnswer();

		const int	currentPlayerIndex()	const { return _currentPlayer%4; }

		void nextPlayer() { ++_currentPlayer; }

		virtual void stop();

		virtual void registerAnswer(int answer);

	private:
		// Secuencias de acciones

		/// Presentacion del modo de juego
		BehaviorTreeNode* presentationSequence() const;
		BehaviorTreeNode* endPresentationSequence();

		/// Presentacion de la ronda de preguntas
		BehaviorTreeNode* questionsIntroSequence();

		/// Secuencia que engloba una ronda completa
		BehaviorTreeNode* roundsSequence();

		BehaviorTreeNode* answerSequence();
		BehaviorTreeNode* askQuestionToPlayerSequence(int player, int round);

		/// Secuencia para el repaso de puntuaciones
		BehaviorTreeNode* scoreReviewSequence();

		/// Secuencia para el turno de respuesta
		BehaviorTreeNode* answeringTimeSequence();

		/// Secuencias para los posibles sucesos en el turno de respuesta
		BehaviorTreeNode* endOfTimeSequence();
		BehaviorTreeNode* questionSuccessSequence();
		BehaviorTreeNode* questionFailureSequence() const;

		//BehaviorTreeNode* endIntroSequence();

		int 				_currentPlayer;

	};

}

#endif /* NORMALMODE_H_ */
