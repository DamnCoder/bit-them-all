/*
 * QuestionXmlDao.h
 *
 *  Created on: 15/11/2010
 *      Author: jorge
 */

#ifndef QUESTIONXMLDAO_H_
#define QUESTIONXMLDAO_H_

#include "persist/XmlDaoAdapter.h"

namespace logic{
	class CQuestion;
}

namespace persist {

	class CQuestionXmlDao :
		public CXmlDao<logic::CQuestion*>,
		public CPrivateSingleton<CQuestionXmlDao> {

	friend class CDaoServer;
	friend class CPrivateSingleton<CQuestionXmlDao>;

	public:
		logic::CQuestion*	read(int id) { return 0; }
		std::vector<logic::CQuestion*> read();

	protected:
		CQuestionXmlDao();
		~CQuestionXmlDao();

	private:
		logic::CQuestion* getQuestion(TiXmlElement* pQuestionElement);
	};

}

#endif /* QUESTIONXMLDAO_H_ */
