/*
 * UiNodes.cpp
 *
 *  Created on: 23/05/2011
 *      Author: jorge
 */

#include "UiNodes.h"

#include "logic/ui/CUiComponent.h"
#include "logic/ui/Button.h"
#include "logic/ui/TextLayout.h"
#include "logic/Game.h"

#include "utilitys/utils.h"

namespace logic {

	CBlinkingUiNode::CBlinkingUiNode(CUiComponent* uiComp, float speed):
			_uiComponent(uiComp),
			_speed(speed),
			_alpha(1.0f)
	{
	}

	CBlinkingUiNode::~CBlinkingUiNode()
	{
		safeDelete(_uiComponent);
	}

	BEHAVIOR_STATUS CBlinkingUiNode::execute(void* agent){

		if ((_alpha <= 0.0f) || (1.0f <= _alpha) )
			_speed = -_speed;

		_alpha += _speed * _data->dt;
		_uiComponent->setAlpha(_alpha);

		return BT_RUNNING;
	}

	void CBlinkingUiNode::init(void* agent){
		//std::cout<<"Iniciando nodo Ui parpadeante\n";
		_data = (tGameData*)agent;
		_alpha = 1.0f;
		_speed = -0.0009f;
	}

//--------------------------------------------------------------------------------------------

	CFadeUiNode::CFadeUiNode(CUiComponent* uiComp, float speed):
			_uiComponent(uiComp),
			_speed(speed),
			_alpha(1.0f)
	{
	}

	CFadeUiNode::~CFadeUiNode()
	{
	}

	BEHAVIOR_STATUS CFadeUiNode::execute(void* agent){
		//std::cout<<"[CFadeUiNode::execute] Alpha: "<<_alpha<<"\n";
		if(_speed == 0) return BT_SUCCESS;

		if (endCondition() ){
			_speed = 0;
			//std::cout<<"[CFadeUiNode::execute] Fade completo\n";
			return BT_SUCCESS;
		}

		_alpha += _speed * _data->dt;
		_uiComponent->setAlpha(_alpha);

		return BT_RUNNING;
	}

	void CFadeUiNode::init(void* agent){
		//std::cout<<"Iniciando nodo Ui fade\n";
		_data = (tGameData*)agent;
		if(_speed < 0)
			_alpha = 1.0f;
		if(_speed > 0)
			_alpha = 0;

		_uiComponent->setAlpha(_alpha);

		//_speed = -0.0009f;
	}

	const bool CFadeUiNode::endCondition() const {
		if(_speed < 0)
			return _alpha <= 0.0f;
		if(_speed > 0)
			return _alpha >= 1.0f;
	}

//--------------------------------------------------------------------------------------------

	CSetAlphaNode::CSetAlphaNode(CUiComponent* uiComp, float alpha):
			_uiComponent(uiComp),
			_alpha(1.0f)
	{}

	CSetAlphaNode::~CSetAlphaNode(){}

	BEHAVIOR_STATUS CSetAlphaNode::execute(void* agent){
		//std::cout<<"[CFadeUiNode::execute] Alpha: "<<_alpha<<"\n";
		_uiComponent->setAlpha(_alpha);

		return BT_RUNNING;
	}

//--------------------------------------------------------------------------------------------

	CUiComponentVisibility::CUiComponentVisibility(CUiComponent* uiComp, bool visible):
			_uiComp(uiComp),
			_visible(visible)
	{
	}

	BEHAVIOR_STATUS CUiComponentVisibility::execute(void* agent){
		_uiComp->setVisible(_visible);
		//std::cout<<"[CUiComponentVisibility::execute] Visibilidad de "<<_uiComp->id()<<" a "<<(_visible?"true":"false")<<"\n";
		return BT_SUCCESS;
	}

//--------------------------------------------------------------------------------------------

	CTextBoxNode::CTextBoxNode(CTextBox* textBox, const std::string& text):
			_textBox(textBox),
			_text(text)
	{
	}

	BEHAVIOR_STATUS CTextBoxNode::execute(void* agent){
		_textBox->setText(_text);
		//std::cout<<"[CTextBoxNode::execute] Text Box: "<<_textBox->id()<<" con mensaje: "<<_textBox->text()<<"\n";
		return BT_SUCCESS;
	}

//--------------------------------------------------------------------------------------------

	CSetButtonStatus::CSetButtonStatus(CButton* button, int status):
			_button(button),
			_status(status)
	{
	}

	BEHAVIOR_STATUS CSetButtonStatus::execute(void* agent){
		//std::cout<<"[CSetButtonStatus::execute] Boton: "<<_button->text()<<" con estatus: "<<_button->status()<<"\n";
		switch(_status) {
		case CButton::NORMAL:
			_button->released();
			return BT_SUCCESS;
		case CButton::PRESSED:
			_button->pressed();
			return BT_SUCCESS;
		case CButton::HIGHLIGHTED:
			_button->highlighted();
			return BT_SUCCESS;
		}

		return BT_FAILURE;
	}

//--------------------------------------------------------------------------------------------

	CTimerButtonNode::CTimerButtonNode(CButton* button, int secs):
			_button(button),
			_currentSec(0),
			_maxSecs(secs),
			_elapsedMillisecs(0)
	{}

	BEHAVIOR_STATUS CTimerButtonNode::execute(void* agent){
		_data = (tGameData*)agent;
		_elapsedMillisecs = _data->game->currentMode()->elapsedTime();

		if(_elapsedMillisecs>1000.0f){
			++_currentSec;
			_button->setText(data2string(_currentSec));
			_elapsedMillisecs = 0.0f;
		}

		if(_currentSec >= _maxSecs ){
			//std::cout<<"[CTimerButtonNode::execute] Han transcurrido "<<_currentSec<<" segundos y habia que alcanzar "<<_maxSecs<<"\n";
			_elapsedMillisecs = 0.0f;
			_currentSec = 0.0f;
			return BT_SUCCESS;
		}

		//std::cout<< "[CTimerButtonNode::execute] Llevas "<<_elapsedMillisecs<<" que son "<<_currentSec<<"\n";

		return BT_SUCCESS;
	}

	void CTimerButtonNode::init(void* agent) {
		//_button->setText("0");
	}
//--------------------------------------------------------------------------------------------

	CPlayerHudVisibility::CPlayerHudVisibility(int numPlayer, bool visible):
			_numPlayer(numPlayer),
			_visible(visible)
	{
	}

	BEHAVIOR_STATUS CPlayerHudVisibility::execute(void* agent){
		tGameData* data = (tGameData*)agent;
		data->game->setPlayerHudVisibility(_visible, _numPlayer);
		//std::cout<<"[CPlayerHudVisibility::execute]\n";
		return BT_SUCCESS;
	}

//--------------------------------------------------------------------------------------------

	CSetQuestionInHud::CSetQuestionInHud(CQuestion* question):
			_question(question)
	{
	}

	BEHAVIOR_STATUS CSetQuestionInHud::execute(void* agent){
		tGameData* data = (tGameData*)agent;
		data->game->setQuestionInHud(_question);
		//std::cout<<"[CSetQuestionInHud::execute] pregunta en HUD\n";
		return BT_SUCCESS;
	}
//---------------------------------------------------------------------------------------------
// Funciones externas
//---------------------------------------------------------------------------------------------

	/**
	 * Devuelve un arbol secuencial donde primero se establece una visibilidad
	 * y despues se lanza un mensaje con texto
	 * @param textBox
	 * @param text
	 * @param visible
	 * @return
	 */
	BehaviorTreeNode* getTextBoxNodeWithVisibility(CTextBox* textBox, const std::string& text, const bool visible){
		BehaviorTreeInternalNode* tbNodes = new SequentialNode();

		tbNodes->addChild(new CUiComponentVisibility(textBox, visible));
		tbNodes->addChild(new CTextBoxNode(textBox, text));

		return tbNodes;
	}

	BehaviorTreeNode* getPlayerHudVisbility(const bool visible){
		BehaviorTreeInternalNode* tbNodes = new SequentialNode();

		for(int i=0; i<4; ++i) {
			tbNodes->addChild(new CPlayerHudVisibility(i, visible));
		}

		return tbNodes;
	}
}
