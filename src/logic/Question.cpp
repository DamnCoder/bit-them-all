/*
 * Pregunta.cpp
 *
 *  Created on: 15/02/2010
 *      Author: jorge
 */

#include "Question.h"

#include <iostream>

namespace logic {

	CQuestion::~CQuestion() {
		_answers.clear();
	}

	std::ostream& operator<<(std::ostream& output, const CQuestion& p){
		output << p._question << "\n";

		std::vector<std::string>::const_iterator it, end;
		end = p._answers.end();
		for(it=p._answers.begin(); it!=end; ++it)
			output << (*it) << "\n";

		return output;
	}



}
