/*
 * QuestionXmlDao.cpp
 *
 *  Created on: 15/11/2010
 *      Author: jorge
 */

#include "QuestionXmlDao.h"

#include "logic/Question.h"

#include <iostream>
namespace persist {

	CQuestionXmlDao::CQuestionXmlDao() : CXmlDao<logic::CQuestion*>() {}

	CQuestionXmlDao::~CQuestionXmlDao() {}

	std::vector<logic::CQuestion*> CQuestionXmlDao::read(){
		std::vector<logic::CQuestion*> questionList;

		// Obtenemos la etiqueta questions

		while(_pElem){
			//std::cout<<"Raiz: "<<_pElem->Value()<<"\n";

			TiXmlElement* pQuestionElement = _hRoot.FirstChild("question").Element();

			while(pQuestionElement){

				logic::CQuestion* question = getQuestion(pQuestionElement);

				//std::cout<<*question;

				questionList.push_back(question);
				pQuestionElement = pQuestionElement->NextSiblingElement("question");
			}

			_pElem = _pElem->NextSiblingElement();
			//std::cout<<std::flush;
		}


		return questionList;
	}

	logic::CQuestion* CQuestionXmlDao::getQuestion(TiXmlElement* pQuestionElement){

		/*
		std::cout << "*************************************\n";
		std::cout << "[CQuestionXmlDao::getQuestion]\n";
		 */
		// Recuperamos el id de la pregunta
		int qID = 0;
		pQuestionElement->QueryIntAttribute("id", &qID);

		// Recuperamos la dificultad
		int difficulty = 0;
		pQuestionElement->QueryIntAttribute("difficulty", &difficulty);

		// Recuparamos la respuesta correcta
		int correctAnswer = 0;
		pQuestionElement->QueryIntAttribute("correct", &correctAnswer);

		// Recuperamos el tipo de pregunta
		std::string type = pQuestionElement->Attribute("type");

		// Recuperamos la pregunta en si
		TiXmlHandle hQuestion(pQuestionElement);
		TiXmlElement* pQuestionTextElement = hQuestion.FirstChild("question_text").Element();
		std::string question_text=pQuestionTextElement->GetText();

		//std::cout<<pQuestionTextElement->Value()<<": "<<pQuestionTextElement->GetText()<<"\n";

		// Recuperamos las respuestas
		std::vector<std::string> answersList;

		TiXmlElement* pAnswerElement = hQuestion.FirstChild("answer").Element();
		while(pAnswerElement){
			//std::cout<<"tag: "<<pQuestionElement->Value()<<": "<<pAnswerElement->GetText()<<"\n";
			answersList.push_back(pAnswerElement->GetText());
			pAnswerElement = pAnswerElement->NextSiblingElement("answer");
		}

		return new logic::CQuestion(qID,question_text, answersList, correctAnswer-1, difficulty);
	}

}
