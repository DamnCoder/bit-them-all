/*
 * QuestionManager.cpp
 *
 *  Created on: 15/11/2010
 *      Author: jorge
 */

#include "QuestionManager.h"

#include "persist/DaoServer.h"
#include "persist/daos/QuestionXmlDao.h"

#include <cstdlib>
#include <ctime>


namespace logic {

	CQuestionManager::CQuestionManager() {
	}

	CQuestionManager::~CQuestionManager() {
		deleteVector(_questions);
	}

	std::vector<CQuestion*> CQuestionManager::getRandomQuestions(int numQuestions){

		// Controlamos que el numero de preguntas pedidas no sea mayor que las que hay en la lista
		if(numQuestions > (int)_questions.size()){
			numQuestions = (int)_questions.size();
		}

		std::vector<int> intList = getNonRepeatedRandomNumbers(numQuestions, (int)_questions.size());	// Lista de numeros aleatorios sin repeticion
		std::vector<CQuestion*> randomQuestionList;	// Lista de preguntas ordenada aleatoriamente

		// La lista de numeros aleatorios nos sirve para recuperar preguntas aleatorias e insertarlas en la lista de
		// preguntas que vamos a devolver
		for(uint i=0; i<intList.size(); ++i)
			randomQuestionList.push_back(_questions[intList[i]]);

		return randomQuestionList;
	}

	bool CQuestionManager::load(const std::string& path){
		persist::IDao<logic::CQuestion*>* qdao = persist::CDaoServer::instance().getQuestionDAO(path);

		_questions = qdao->read();

		//cout<<"[CQuestionManager::load] Numero de preguntas cargadas: "<<_questions.size()<<"\n";

		return true;
	}

}
