/*
 * Pregunta.h
 *
 *  Created on: 15/02/2010
 *      Author: jorge
 */

#ifndef QUESTION_H_
#define QUESTION_H_

#include <string>
#include <vector>

#include <iostream>

namespace logic {

	class CQuestion {

	public:

		CQuestion():
			_id(0),
			_question("PREGUNTA DE PRUEBA!"),
			_correctAnswerIndex(0),
			_difficulty(0)
		{
		}

		CQuestion(const std::string& pregunta): _question(pregunta), _correctAnswerIndex(0){
			_answers.push_back("A - Primera opcion");
			_answers.push_back("B - Segunda opcion");
			_answers.push_back("C - Tercera opcion");
			_answers.push_back("D - Cuarta opcion");
		}

		CQuestion(
				int id,
				const std::string& pregunta,
				const std::vector<std::string> respuestas,
				int respuestaCorrecta,
				int dificultad):
			_id(id),
			_question(pregunta),
			_answers(respuestas),
			_correctAnswerIndex(respuestaCorrecta),
			_difficulty(dificultad)
		{
		}

		virtual ~CQuestion();

		const int 					getId() 					const { return _id; }
		std::string 				getQuestion()				const { return _question; }
		std::vector<std::string>	getAnswers()				const { return _answers; }
		const bool 					isCorrect(int respuesta) 	const { return respuesta == _correctAnswerIndex; }
		const std::string 			getAnswer() 				const { return _answers[_correctAnswerIndex]; }
		const int 					correctAnswerIndex() 		const { return _correctAnswerIndex; }
		friend std::ostream& operator<<(std::ostream& output, const CQuestion& p);

	private:
		int 						_id;
		std::string 				_question;
		std::vector<std::string> 	_answers;
		int 						_correctAnswerIndex;
		int 						_difficulty;

	};

}

#endif /* QUESTION_H_ */
