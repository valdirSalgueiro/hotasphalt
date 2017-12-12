#include "Road.h"
#include "Background.h"
#include "GameController.h"
#include "Player.h"
#include "core/gl2d.h"
#include "Engine.h"

bool FLAG_COLOR = true;

/**
* Constructor
*/
GameController::GameController(Engine* engine) {
	this->engine = engine;
	gameState = GAME_STATE_PAUSED;
	resumeGameState = GAME_STATE_PLAYING_IDLE;
}

/**
* Free the memory allocated the internal game controller instance.
*/
void GameController::freeInstance() {
	// TODO free all resources acquired by controller
	//Resources.freeBufferedImages();
	//

	//Resources.forceGC();
	//
}

/**
* Loads all game resource Loads the level indicated at levelNum parameter
* Starts the game
* 
* @param levelNum
*            indicating the number of the level to be loaded
*/
void GameController::startGame(int world,int track, int player) {
	gameState = GAME_STATE_PLAYING_IDLE;

	road = new Road(player,world,track);
	background = new Background(world,road->getHorizon());


	gameOver = false;
}

/**
* Put the game in the paused state.
*/
void GameController::pauseGame() {
	// Evaluate the state to next game resume.
	if (gameState != GAME_STATE_PAUSED) {

		resumeGameState = gameState;
		gameState = GAME_STATE_PAUSED;
	}
}

/**
* Put the game in the playing state.
*/
void GameController::resumeGame() {
	gameState = resumeGameState;
}

void GameController::setKeypress(int keyCode) {
	keyPressing = keyCode; 
}

/**
* Updates the game state. It's called each game clock milliseconds.
* 
* @param keyStates
*            The state of device keys.
*/
void GameController::update() {
	processGameUpdate();
}

/**
* Process the update of the playing state.
* 
* @param keyStates
*            The state of device keys.
*/
void GameController::processGameUpdate() {

	switch(gameState){
	case GAME_STATE_PLAYING_IDLE:
		if(!road->goBack){
			background->update();
			road->update(keyPressing);
	
			background->setPlayerX(road->getPlayerX());
			background->setHorizon(road->getHorizon());
			background->setMoveTerrain(road->isGameStarted() && !road->raceOver());
		}else{
			delete road;
			delete background;
			//startGame(0,0,0);
			engine->gameState=Engine::MENU1;

		}
	case GAME_STATE_PAUSED:
		break;
	}
}




bool GameController::isGameOver() {
	return gameOver;
}

/**
* Paints the current game frame in the g graphics object. It's called each
* game clock milliseconds.
* 
* @param g
*            the Graphics object where the game frame must be painted.
*/
void GameController::paint() {
	switch (gameState) {
	case GAME_STATE_PLAYING_IDLE:
		background->paint(road->getPlayer(),false);

		road->paint(false);
		
		break;

	case GAME_STATE_PAUSED:
		//Keep painting the game while the game is paused.

		background->paint(road->getPlayer(),true);
		road->paint(true);

		break;
	}

}

int GameController::getPlayerSelectedChar(){
	return road->getPlayer()->getSelectedChar();
}

/**
* Returns the state of the game.
* 
* @return The game´s state
*/
int GameController::getState() {
	return gameState;
}

int GameController::getPlayerPosition(){
	return road->getPlayer()->getPlace();
}

std::vector<int> GameController::getRaceEndRanking(){
	return road->getRaceEndRanking();
}