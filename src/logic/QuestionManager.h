/*
 * QuestionManager.h
 *
 *  Created on: 15/11/2010
 *      Author: jorge
 */

#ifndef QUESTIONMANAGER_H_
#define QUESTIONMANAGER_H_

#include "utilitys/Singleton.h"

#include "logic/Question.h"

#include <vector>
#include <string>

namespace logic {

	class CQuestionManager : public CSingleton<CQuestionManager> {

	friend class CSingleton<CQuestionManager>;

	public:

		virtual ~CQuestionManager();

		std::vector<CQuestion*> getQuestions() const{
			return _questions;
		}

		std::vector<CQuestion*> getRandomQuestions(int numQuestions);

		bool load(const std::string& path);

	private:

		CQuestionManager();

		std::vector<CQuestion*> _questions;

	};

}

#endif /* QUESTIONMANAGER_H_ */
