/*
 * CGame.cpp
 *
 *  Created on: 22/01/2010
 *      Author: jorge
 */

#include "Game.h"

#include "gui/WindowManager.h"

#include "logic/modes/nodes/GameNodes.h"
#include "logic/modes/nodes/CameraNodes.h"
#include "logic/modes/nodes/AudioNodes.h"
#include "logic/modes/nodes/UiNodes.h"
#include "logic/modes/nodes/ConditionNodes.h"
#include "logic/modes/nodes/AnimationNodes.h"

#include "logic/modes/NormalMode.h"

#include "logic/ui/Frame.h"
#include "logic/ui/Label.h"
#include "logic/ui/TextLayout.h"

#include "QuestionManager.h"

#include "logic/entitys/Scenario.h"
#include "logic/entitys/Player.h"
#include "logic/entitys/Screen.h"
#include "logic/entitys/Host.h"

namespace logic {

	CGame::CGame():
			ILogicFacade(),
			_nMode(0),
			_currentMode(0),
			_currentBehavior(0),
			_cameraActive(false),
			_endIntro(false),
			_endModeIntro(false)
	{
	}

	CGame::~CGame() {
		if(isInit())
			finish();
	}

	/**
	 * Inicia todos los elementos que participan en el juego y los deja
	 * preparados para iniciar la partida.
	 */
	void CGame::init(){

		//cout<<"[CGame::init] inicio\n";

		//----------------------------------------------------
		// Creacion de las vistas del juego
		//----------------------------------------------------
		// Creamos el escenario logico
		_scenario->screen()->setVisible(false);
		addGfx(_scenario);

		// Creamos los jugadores (el resto se ha creado en los menus)
		emitPlayers();

		// Creamos el Gui del juego
		createGui();
		createHostTextBox();
		createScoreHud();
		createQuestionsHud();

		//----------------------------------------------------
		// Inicializacion de elementos del juego
		//----------------------------------------------------
		_nMode = 0;
		_playing = true;
		_init = true;

		for(int i=0; i<4; ++i) _players[i]->setPoints(0);

		//----------------------------------------------------
		// Inicializacion de las rondas
		//----------------------------------------------------

		// Cargamos las intros del juego
		BehaviorTreeNode* intro = getIntroSequence();
		BehaviorTreeNode* end = getEndIntroSequence();
		_intros.push_back(new CIntroBehavior(this, intro, end));

		intro = getPlayersPresentationSequence();
		end = getEndPlayersPresentation();
		_intros.push_back(new CIntroBehavior(this, intro, end));

		intro = getEndModeSequence();
		end = getEndEndModeSequence();
		_intros.push_back(new CIntroBehavior(this, intro, end));

		intro = getEndDemoSequence();
		end = getEndEndDemoSequence();
		_intros.push_back(new CIntroBehavior(this, intro, end));


		// Cargamos los modos de concurso
		_modes.push_back(new CNormalMode(
				"En esta ronda todo el mundo debe responder, por turnos, a las preguntas que se le hagan.",
				this,
				0));

		_currentMode = _modes[_nMode];

		//cout<<"[CGame::init] fin\n";
	}

	void CGame::finish(){

		deleteVector(_modes);
		deleteVector(_intros);

		//cout<<"[CGame::finish] Elimino graficos animados\n";
		destroyAnimatedEntitys();
		//cout<<"[CGame::finish] Elimino graficos escenario\n";
		deleteGfx(_scenario);

		destroyGui();
		destroyHostTextBox();
		destroyScoreHud();
		destroyQuestionsHud();

		_playing = false;
		_init = false;

		//cout<<"[CGame::finish] fin\n";
	}

	void CGame::actualize(float dt){
		if(!playing() || !isInit()) return;

		// 1 - Procesamos cola de comandos del jugador con turno
		processCommandQueue();

		// 2 - Actualizamos entidades
		actualizeEntitys(dt);

		// 3 - Actualizamos el comportamiento del juego
		actualizeBehavior(dt);

		// 4 - Procesamos cola de eventos
		processEventQueue();

	}

	void CGame::actualizeEntitys(float dt){
		_scenario->actualizeEntitys(dt);
	}

	void CGame::actualizeBehavior(float dt){
		if(_currentBehavior) _currentBehavior->actualize(dt);
	}

	void CGame::insertCommand(tCommand command){
		_commandQueue.push_back(command);
	}

	/**********************************************************************************/
	/** METODOS DE ACCESO A LA LOGICA DE LA APLICACION POR PARTE DEL RESTO DE CAPAS **/

	const bool CGame::endRound() const {
		return true;
	}

	const string CGame::roundExplanation() const {
		return _currentMode->explanation();
	}

	CScreen* CGame::screen() const {
		return _scenario->screen();
	}

	CQuizShowHost* CGame::host() const{
		return _scenario->host();
	}

	vector<CPlayer*> CGame::players() const {
		return _scenario->players();
	}

	CPlayer* CGame::currentPlayer() const {
		return _players[_currentMode->currentPlayerIndex()];
	}

	vector<CAvatar*> CGame::audience() const {
		return _scenario->audience();
	}

	void CGame::setAnimationMap(TAnimationsMap animationsMap){
		_animationsMap = animationsMap;
	}

	void CGame::setScenario(CScenario* scenario){
		_scenario = scenario;
		_players = _scenario->players();
	}

	void CGame::setPlayerAppearance(int numPlayer, const string& id, const string& skin, const string& name, int scale){
		_players[numPlayer]->setId(id);
		_players[numPlayer]->setSkin(skin);
		_players[numPlayer]->setScale(scale);
		_players[numPlayer]->setName(name);
		//cout<<"[CGame::setPlayerAppearance] ID: "<<_players[numPlayer]->getId()<<" Nombre: "<<_players[numPlayer]->getName()<<"\n";
	}

	void CGame::setAudienceAppearance(int numPlayer, const string& id, const string& skin, int scale){
		_scenario->setAudienceAppearance(numPlayer, id, skin, scale);
		//cout<<"[CGame::setPlayerAppearance] ID: "<<_players[numPlayer]->getId()<<" Nombre: "<<_players[numPlayer]->getName()<<"\n";
	}

	void CGame::addCamera(const string& camID){
		_cameraActive = true;
		emitAddCamera(camID);
	}

	void CGame::clearCameras(){
		emitClearCameras();
	}

	/**********************************************************************************/

	//---------------------------------------------------------
	// CONTROL DEL HUD
	//---------------------------------------------------------

	void CGame::setTimeInHud(const std::string& time) {
		_bClock->setText(time);
	}

	void CGame::setHostTextBoxVisibility(const bool visible) {
		_tbHost->setVisible(visible);
	}
	void CGame::setPlayerHudVisibility(const bool visible, const int numPlayer) {
		_fIconFrame[numPlayer]->setVisible(visible);
		_fPlayerIcon[numPlayer]->setVisible(visible);
		_bPlayerPoints[numPlayer]->setVisible(visible);
		_bPlayerName[numPlayer]->setVisible(visible);
	}

	void CGame::setPlayerPointsInHud(const int numPlayer, const int numPoints) {
		const std::string points = data2string(numPoints);
		_bPlayerPoints[numPlayer]->setText(points);
	}

	void CGame::setQuestionInHud(CQuestion* question) {
		_bQuestion->setText(question->getQuestion());

		for(int i=0; i<4; ++i)
			_bAnswers[i]->setText(question->getAnswers().at(i));
	}

	void CGame::cleanHud() {
		_lQuit->setVisible(false);
		_tbHost->setVisible(false);
		_fQuestionsGui->setVisible(false);

		for(int i=0; i<4; ++i) {
			_scoreGui->setVisible(false);
			setPlayerHudVisibility(false, i);
		}

	}
	//---------------------------------------------------------
	// METODOS PROTEGIDOS
	//---------------------------------------------------------

	/**
	 * Los comandos con efecto antes del tick, se procesan directamente aqui.
	 * Si algun comando provoca cambios despues del tick, lanzamos evento
	 */
	void CGame::processCommandQueue(){
		TCommandIterator
			it(_commandQueue.begin()),
			end(_commandQueue.end());

		for(;it!=end; ++it){
			tCommand c = (*it);

			switch(c.id){

			case GAME_INTRO:	/// Da comienzo una intro del juego
				_currentBehavior = _intros[c.introId];
				break;
			case MODE_INTRO:	/// Da comienzo a la introduccion del modo de juego
				_currentBehavior = _currentMode = _modes[_nMode];
				_currentMode->setIntro(c.introId);
				break;

			case END_INTRO:		/// Termina la intro que haya en marcha
				//cout<<"[CGame::processCommandQueue] Fin de Intro TRUE\n";
				_endIntro = true;
				_eventQueue.push_back(TGameEvent(END_INTRO_EVENT));
				break;

			case INIT_QUESTIONS:
				_currentBehavior = _currentMode = _modes[_nMode];
				_currentMode->setBehavior(0);
				break;

			case END_CAMERAS:
				//std::cout<<"[CGame::processCommandQueue]Ya no hay mas camaras\n";
				_cameraActive = false;
				break;

			case P1_A:
				registerAnswer(3);
				break;

			case P1_B:
				registerAnswer(2);
				break;

			case P1_C:
				registerAnswer(1);
				break;

			case P1_D:
				registerAnswer(0);
				break;

			case END_PRESENTATION:
				//cout<<"[CGame::processCommandQueue] Fin de presentación a TRUE\n";
				_endModeIntro = true;
				break;

			default:
				break;
			}
		}

		_commandQueue.clear();
	}

	void CGame::processEventQueue(){
		TGameEventIterator
			it(_eventQueue.begin()),
			end(_eventQueue.end());

		for(;it!=end;++it){
			TGameEvent gameEvent = (*it);

			switch(gameEvent.id){

			case END_INTRO_EVENT:
				_currentBehavior->stop();
				break;

			default:
				break;
			}
		}

		_eventQueue.clear();
	}

	void CGame::nextMode(){
		++_nMode;
	}

	void CGame::registerAnswer(int answer){
		_currentMode->registerAnswer(answer);
	}

	void CGame::terminateIntro() {
		_eventQueue.push_back(END_INTRO_EVENT);
	}

	void CGame::changeState(const std::string& state) {
		emitChangeState(state);
	}

	void CGame::emitPlayers(){
		CScenario::TPlayerList players = _scenario->players();
		CScenario::TPlayerIterator
			it(players.begin()),
			end(players.end());

		for(;it!=end; ++it){
			this->addGfx(*it);
		}
	}

	void CGame::emitPublic() {
		CScenario::TAvatarList audience = _scenario->audience();
		CScenario::TAvatarIterator
			it(audience.begin()),
			end(audience.end());

		for(;it!=end; ++it){
			this->addGfx(*it);
		}
	}

	void CGame::destroyAnimatedEntitys(){
		CScenario::TAvatarList avatares = _scenario->avatares();
		CScenario::TAvatarIterator
			it(avatares.begin()),
			end(avatares.end());

		for(;it!=end; ++it){
			this->deleteGfx(*it);
		}

	}

	BehaviorTreeNode* CGame::getIntroSequence()
	{
		BehaviorTreeInternalNode* quizShowInitSequence;

		(quizShowInitSequence = new SequentialNode())
				->addChild((new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL))
						->addChild((new SequentialNode())
								->addChild(new CUiComponentVisibility(_fWhiteFlash, true))
								->addChild(initQuizShowCharactersAnimation())
								->addChild(new CFadeUiNode(_fWhiteFlash, -0.0009f))
								->addChild(new CScreenVideoNode(_scenario->screen(), "intro.mpg"))
								->addChild(new CPlayEffect(0))			/// Sonido aplausos
								->addChild(new CPlaySongNode(1))		/// Cancion de inicio
								//->addChild(new CRandomPathCameras(5))
								->addChild(new CCameraNode("cam6"))
								->addChild(new CCameraNode("cam5"))
								->addChild(new CCameraNode("cam2"))
								->addChild(new CCameraNode("cam1"))
								->addChild(new CCameraNode("cam4"))
								)
						->addChild(new COrientFromGivenOrientation(_scenario->host(), 360.0f, 0.0f, -0.03))
						)
				->addChild(getDoNothingWhileCondition(new CEndCamerasCondition))
				->addChild(new CFadeOutChannel(-1, 2000))
				->addChild(new CStopSongs())
				->addChild(new CChangeStateNode("intro_concursantes"));

		return quizShowInitSequence;
	}

	BehaviorTreeNode* CGame::getEndIntroSequence()
	{
		BehaviorTreeInternalNode* sequence;
		(sequence = new SequentialNode())
				->addChild(new CClearCameraQueue)
				->addChild(new CUiComponentVisibility(_fWhiteFlash, false))
				->addChild(new CFadeOutChannel(-1, 2000))
				->addChild(new CStopSongs())
				->addChild(new CChangeStateNode("intro_concursantes"))
				;
		return sequence;
	}

	BehaviorTreeNode* CGame::getPlayersPresentationSequence()
	{
		BehaviorTreeInternalNode* sequence;

		std::string playersPresentationSpeech = "Contamos hoy con la presencia de...\n";

		float idleBetweenPlayer = 2000;
		(sequence = new SequentialNode())
				->addChild(new CCameraNode("cam9"))
				->addChild(new CPlayLoopSongNode(2))		// Cancion de comienzo de ronda
				->addChild(new CUiComponentVisibility(_tbHost, true))
				->addChild(new CTextBoxNode(_tbHost, "Es hora de presentar a los concursantes de hoy!"))
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING"))	// Establece animacion de presentador hablando
				->addChild(new COrientFromGivenOrientation(_scenario->host(), 180.0f, 135.0f, -0.05))
				->addChild((new SequentialNode())
						->addChild(new CTextBoxNode(_tbHost, (playersPresentationSpeech+=_players[0]->name()+"!")))
						->addChild(new CPlayerHudVisibility(0, true))
						->addChild(new CIdleNode(idleBetweenPlayer))
						->addChild(new CTextBoxNode(_tbHost, (playersPresentationSpeech+=(std::string)"\n"+_players[1]->name()+"!")))
						->addChild(new CPlayerHudVisibility(1, true))
						->addChild(new CIdleNode(idleBetweenPlayer))
						->addChild(new CTextBoxNode(_tbHost, (playersPresentationSpeech+=(std::string)"\n"+_players[2]->name()+"!")))
						->addChild(new CPlayerHudVisibility(2, true))
						->addChild(new CIdleNode(idleBetweenPlayer))
						->addChild(new CTextBoxNode(_tbHost, (playersPresentationSpeech+=(std::string)"\ny "+_players[3]->name()+"!")))
						->addChild(new CPlayerHudVisibility(3, true))
						->addChild(new CIdleNode(idleBetweenPlayer))
						)
				->addChild(new CIdleNode(5))
				->addChild(new CUiComponentVisibility(_tbHost, false))
				->addChild(new CChangeStateNode("intro_modo"))
				;

		return sequence;
	}

	BehaviorTreeNode* CGame::getEndPlayersPresentation()
	{
		BehaviorTreeInternalNode* sequence;

		std::string playersPresentationSpeech = "Contamos hoy con la presencia de...";
		playersPresentationSpeech+=(std::string)"\n"+_players[0]->name()+"!";
		playersPresentationSpeech+=(std::string)"\n"+_players[1]->name()+"!";
		playersPresentationSpeech+=(std::string)"\n"+_players[2]->name()+"!";
		playersPresentationSpeech+=(std::string)"\ny "+_players[3]->name()+"!";
		(sequence = new SequentialNode())
				->addChild(new CClearCameraQueue)
				->addChild(new CUiComponentVisibility(_lQuit, false))
				->addChild((new SequentialNode())
						->addChild(new CPlayerHudVisibility(0, true))
						->addChild(new CPlayerHudVisibility(1, true))
						->addChild(new CPlayerHudVisibility(2, true))
						->addChild(new CPlayerHudVisibility(3, true))
						)
				->addChild(new CTextBoxNode(_tbHost, playersPresentationSpeech))
				->addChild(new CIdleNode(3))
				->addChild(new CUiComponentVisibility(_tbHost, false))
				->addChild(new CChangeStateNode("intro_modo"))
				;

		return sequence;
	}

	/**
	 * Secuencia que presenta al ganador o ganadores.
	 * @return
	 */
	BehaviorTreeNode* CGame::getEndModeSequence() {
		BehaviorTreeInternalNode* sequence;
		(sequence = new SequentialNode())
				->addChild(initQuizShowCharactersAnimation())
				->addChild(new CCleanHud)
				->addChild(new CCameraNode("cam9"))
				->addChild(new CSkinChangeNode(_scenario->screenMult(), "sobera_wants_you"))
				->addChild(new CPlaySongNode(4))
				->addChild(new CPlayEffect(0))
				->addChild(new COrientAvatarNode(_scenario->host(),135.0f))
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING"))	// Establece animacion de presentador hablando
				->addChild(new CWinnersHud())
				->addChild(new CIdleNode(6))
				->addChild(new CChangeStateNode("fin_demo"))
					;
		return sequence;
	}

	BehaviorTreeNode* CGame::getEndEndModeSequence() {
		BehaviorTreeInternalNode* sequence;
		(sequence = new SequentialNode())
				->addChild(initQuizShowCharactersAnimation())
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING"))	// Establece animacion de presentador hablando
				->addChild(new COrientAvatarNode(_scenario->host(),135.0f))
				->addChild(new CWinnersHud())
				->addChild(new CIdleNode(5))
					;
		return sequence;
	}

	BehaviorTreeNode* CGame::getEndDemoSequence() {
		BehaviorTreeInternalNode* sequence;
		(sequence = new SequentialNode())
				->addChild(new CCleanHud)
				->addChild(new CCameraNode("cam8"))
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_MOVING_FINGERS", "HEAD_BLINKING"))
				->addChild(new COrientFromGivenOrientation(_scenario->host(), 135.0f, 200.0f, 0.05))
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_EXPLAINING", "HEAD_SPEAKING"))	// Establece animacion de presentador hablando
				->addChild(getTextBoxNodeWithVisibility(_tbHost, "Esto solo ha sido una demo. ¡En breve se podrán disfrutar más modos de juego!", true))
				->addChild(new CIdleNode(4))
				->addChild(getTextBoxNodeWithVisibility(_tbHost, "¡Nos vemos pronto!", true))
				->addChild(new CIdleNode(3))
				->addChild(new CReturnToMenu)
				;
		return sequence;
	}

	BehaviorTreeNode* CGame::getEndEndDemoSequence() {
		BehaviorTreeInternalNode* sequence;
		(sequence = new SequentialNode())
				->addChild(new COrientAvatarNode(_scenario->host(),200.0f))
				->addChild(new CIdleNode(5))
					;
		return sequence;
	}

	///--------------------------------------------------------------------------
	/// HUD
	///--------------------------------------------------------------------------

	void CGame::createRankingFromScoreHud() {
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		float xPos = scrwidth / 32.0f;
		float yPos = scrheight / 32.0f;

		// Dialogo primer puesto
		_tiedPlayers = _currentMode->playersTiedForFirstPlace();
		_ranking = _currentMode->playersRanking();

		// Empates en el primer puesto
		std::string speech="";
		unsigned int nTiedPlayers = _tiedPlayers.size();
		if(nTiedPlayers>1){
			// Comprobamos si han empatado a cero
			if(nTiedPlayers == 4 && _tiedPlayers[0]->points() == 0)
				speech = "¡¡Increible, nadie ha acertado ni una pregunta!!\nNo hay ganadores.\nA todos nos invade una sensación de vergüenza.";
			else{
				// Si no, hay empate en el primer puesto
				speech = "Tenemos un empate en el primer puesto entre:\n";
				for(int i=0; i<(int)_tiedPlayers.size(); ++i){
					if(i == (int)_tiedPlayers.size()-1) speech +="y ";
					speech += _tiedPlayers[i]->name()+(std::string)" con "+data2string(_tiedPlayers[i]->points())+(std::string)" puntos \n";
				}
			}
		}
		else
			speech += "El ganador de esta ronda ha sido "+_tiedPlayers[0]->name();

		//cout<<speech;

		_tbHost->setVisible(true);
		_tbHost->setText(speech, xPos*15, yPos*26);

		int constant = 13;

		for(int i=0; i<4; ++i){
			_fIconFrame[i]->setPosition( xPos*24, yPos*((i*4)+constant));
			_fPlayerIcon[i]->setPosition( (xPos*24) + 8, (yPos*((i*4)+constant)) + 8 );
			_bPlayerPoints[i]->setPosition( xPos*27 - 16, yPos*((i*4)+constant) );
			_bPlayerName[i]->setPosition( xPos*27 - 16, yPos*((i*4)+constant) + 39 );

			_bPlayerPoints[i]->setText(data2string(_ranking[3-i]->points()));
			_fPlayerIcon[i]->setID(_ranking[3-i]->skin()+(std::string)"_icon");
			_bPlayerName[i]->setText(_ranking[3-i]->name());

			//cout<<"Jugador "<<i<<": "<<_ranking[i]->name()<<" con "<<_ranking[i]->points()<<" y el icono "<<_ranking[i]->skin()<<"\n";
		}

	}

	/**
	 * Establece las animaciones de el publico, de los jugadores y del presentador
	 * en la secuencia de inicio del juego.
	 * Las acciones ocurren todas a la vez.
	 * @return
	 */
	BehaviorTreeNode* CGame::initQuizShowCharactersAnimation() const {
		BehaviorTreeInternalNode* animation;

		(animation= new ParallelNode(FAIL_ON_ONE, SUCCEED_ON_ALL))
				->addChild(randomAudienceAnimation(_scenario))
				->addChild(setAllPlayersAnimation(_scenario, "LEGS_STILL", "TORSO_APPLAUSE", "HEAD_APPLAUSE", 1000))
				->addChild(new CAnimateAvatarNode(_scenario->host(), "LEGS_IDLE", "TORSO_MOVING_FINGERS", "HEAD_BLINKING"))
		;

		return animation;
	}

	void CGame::createGui(){
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		_lQuit = new CLabel(20, scrheight-40, "Pulsa cualquier tecla para continuar...");

		_fWhiteFlash = new logic::CFrame(0, 0, scrwidth, scrheight);
		_fWhiteFlash->setID("white_background");

		_fWhiteFlash->setVisible(false);

		_fLogoTv = new CFrame(0, 0, 122, 40);
		_fLogoTv->setPosition(scrwidth - 142, scrheight - 60);
		_fLogoTv->setID("logo_esi_tv_small");

		addGfx(_fLogoTv);
		addGfx(_lQuit);

		addGfx(_fWhiteFlash);

	}

	void CGame::destroyGui(){
		deleteGfx(_lQuit);
		deleteGfx(_fWhiteFlash);
		deleteGfx(_fLogoTv);

		delete _lQuit;
		delete _fWhiteFlash;
		delete _fLogoTv;
	}

	void CGame::createHostTextBox() {
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();
		_tbHost = new CTextBox(scrwidth-300, scrheight-200);
		_tbHost->setFont("arial.ttf");
		_tbHost->setFontSize(20);
		_tbHost->setVisible(false);
		_tbHost->setBackground(true);
		_tbHost->setID("tb_square_grey_medium");

		addGfx(_tbHost);
	}

	void CGame::destroyHostTextBox() {
		deleteGfx(_tbHost);
		delete _tbHost;
	}

	void CGame::createScoreHud() {
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		_scoreGui = new CFrame(0, 0, scrwidth, scrheight);

		{
			float xPos = scrwidth / 128.0f;
			float yPos = scrheight / 48.0f;

			// Creacion de los marcos de los iconos
			for(int i=0; i<4; ++i){
				_fIconFrame[i] = new CFrame(0, 0, 80, 80);
				_fIconFrame[i]->setID("tb_square_grey");
			}

			_fIconFrame[0]->setPosition(xPos*97, yPos);
			_fIconFrame[1]->setPosition(xPos*65, yPos);
			_fIconFrame[2]->setPosition(xPos*33, yPos);
			_fIconFrame[3]->setPosition(xPos, yPos );


			// Creacion de los iconos de los jugadores
			for(int i=0; i<4; ++i){
				_fPlayerIcon[i] = new CFrame(0, 0, 64, 64);
				_fPlayerIcon[i]->setID(_players[i]->skin()+(std::string)"_icon");
			}

			_fPlayerIcon[0]->setPosition((xPos*97)+8, yPos+8);
			_fPlayerIcon[1]->setPosition((xPos*65)+8, yPos+8);
			_fPlayerIcon[2]->setPosition((xPos*33)+8, yPos+8);
			_fPlayerIcon[3]->setPosition(xPos+8, yPos+8);

		}
		{
			float xPos = scrwidth / 128.0f;
			float yPos = (scrheight / 96.0f);

			// Creacion de los marcos de nombres
			for(int i=0; i<4; ++i){
				_bPlayerName[i] = new CButton(_players[i]->name(), 0, 0, 160, 40);
				_bPlayerName[i]->setStyle("dark_blue");
				_bPlayerName[i]->setFontSize(20);
			}

			_bPlayerName[0]->setPosition(xPos*107, yPos * 7);
			_bPlayerName[1]->setPosition(xPos*75, yPos * 7);
			_bPlayerName[2]->setPosition(xPos*43, yPos * 7);
			_bPlayerName[3]->setPosition(xPos*11, yPos * 7);

			_bPlayerName[0]->highlighted();

			// Creacion de los marcos de puntos
			for(int i=0; i<4; ++i){
				_bPlayerPoints[i] = new CButton(data2string(_players[i]->points()), 0, 0, 160, 40);
				_bPlayerPoints[i]->setStyle("dark_blue");
				_bPlayerPoints[i]->setFontSize(25);
			}

			_bPlayerPoints[0]->setPosition(xPos*107, yPos * 2);
			_bPlayerPoints[1]->setPosition(xPos*75, yPos * 2);
			_bPlayerPoints[2]->setPosition(xPos*43, yPos * 2);
			_bPlayerPoints[3]->setPosition(xPos*11, yPos * 2);

		}

		for(int i=0; i<4; ++i){
			addGfx(_fIconFrame[i]);
			addGfx(_fPlayerIcon[i]);
		}


		for(int i=0; i<4; ++i){
			_scoreGui->add(_bPlayerName[i]);
			_scoreGui->add(_bPlayerPoints[i]);
			setPlayerHudVisibility(false, i);
		}
		addGfx(_scoreGui);
	}

	void CGame::destroyScoreHud() {

		deleteGfx(_fIconFrame[0]);
		deleteGfx(_fIconFrame[1]);
		deleteGfx(_fIconFrame[2]);
		deleteGfx(_fIconFrame[3]);
		deleteGfx(_fPlayerIcon[0]);
		deleteGfx(_fPlayerIcon[1]);
		deleteGfx(_fPlayerIcon[2]);
		deleteGfx(_fPlayerIcon[3]);

		deleteGfx(_scoreGui);

		safeDelete(_fIconFrame[0]);
		safeDelete(_fIconFrame[1]);
		safeDelete(_fIconFrame[2]);
		safeDelete(_fIconFrame[3]);
		safeDelete(_fPlayerIcon[0]);
		safeDelete(_fPlayerIcon[1]);
		safeDelete(_fPlayerIcon[2]);
		safeDelete(_fPlayerIcon[3]);

		safeDelete(_scoreGui);

	}

	void CGame::createQuestionsHud() {
		float scrwidth = gui::CWindowManager::instance().width();
		float scrheight = gui::CWindowManager::instance().height();

		float xGridWidth = scrwidth / 32.0f;
		float yGridHeight = scrheight / 48.0f;

		_fQuestionsGui = new CFrame(0, 0, scrwidth, scrheight);

		for(int i=0; i<4; ++i){
			_bAnswers[i] = new CButton("", 0, 0, xGridWidth*10, yGridHeight*4);
			_bAnswers[i]->setStyle("metal_s");
			_bAnswers[i]->setPosition(19*xGridWidth, (10 + i*7)*yGridHeight);
			_bAnswers[i]->setFontSize(20);
			_fQuestionsGui->add(_bAnswers[i]);
		}

		for(int i=0; i<4; ++i){
			_bColorAnswers[i] = new CButton(0, 0, xGridWidth*2, yGridHeight*4);
			switch(i){
			case 0:
				_bColorAnswers[i]->setStyle("outline_blue");
				break;
			case 1:
				_bColorAnswers[i]->setStyle("outline_green");
				break;
			case 2:
				_bColorAnswers[i]->setStyle("outline_red");
				break;
			case 3:
				_bColorAnswers[i]->setStyle("outline_yellow");
				break;
			}
			_bColorAnswers[i]->setPosition(16*xGridWidth, (10 + i*7)*yGridHeight);
			_fQuestionsGui->add(_bColorAnswers[i]);
		}

		_bQuestion = new CButton("", 0, 0, xGridWidth*24, yGridHeight*4 );
		_bQuestion->setStyle("metal_s");
		_bQuestion->setPosition(5*xGridWidth, 38*yGridHeight);
		_bQuestion->setFontSize(23);
		_fQuestionsGui->add(_bQuestion);

		_bClock = new CButton("5", 0, 0, xGridWidth*3, yGridHeight*4 );
		_bClock->setStyle("metal_xs");
		_bClock->setPosition(xGridWidth, 38*yGridHeight);
		_fQuestionsGui->add(_bClock);

		_fQuestionsGui->setVisible(false);
		//_questionsGui->setAlpha(0);

		addGfx(_fQuestionsGui);

		_bClock->setFontSize(40);
	}

	void CGame::destroyQuestionsHud() {
		deleteGfx(_fQuestionsGui);
		safeDelete(_fQuestionsGui);
	}

}
