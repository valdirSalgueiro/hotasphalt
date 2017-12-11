#include "Block.h"
#include "Road.h"
#include "Player.h"
#include "Enemy.h"
#include "core/gl2d.h"
#include <sstream>

#include "font/PreCompile.h"
#include "font/FontAtlas.h"
#include "font/FTBitmapFont.h"
#include "font/FTBitmapChar.h"

#include "sound/sound.h"
sound* engine;
sound* go;

bool soundPlaying;

extern FontAtlas* m_pFontAtlas;

std::string speedStr;

std::string players[5];


Road::Road(int selectedPlayer_,int world_, int track_){
	goBack=false;
	frames=0;
	largadaFrame=0;

	setLevel(world_,track_);
	world = world_;

	blockId=0;
	playerStartingPosition=0;
	//
	soundPlaying=false;

	engine=new sound("../res/engine2_.ogg",80,100,50);
	go=new sound("../res/go.wav",80,100,50);	

	players[0]="THUNNER";
	players[1]="BURRINI";
	players[2]="BRAND´S";
	players[3]="IVANOVICHI";
	players[4]="TAYKUN";

	alreadyStarted=false;
	raceOver_=false;
	paintSinal=false;

	blocks = std::vector<Block*>(BLOCKS_BUFFER);
	bufferHelper = std::vector<Block*>(BLOCKS_BUFFER);
	blocks[0] = new Block();
	blocks[0]->setBlockAttributes(0,0, level[0]);
	blockId = 1;
	//initializes each block
	for(int i = 1; i < BLOCKS_BUFFER; i++){
		blocks[i] = new Block(blocks[i-1]);
		int block = blockId%(sizeof(level) / sizeof(int));
		blocks[i]->setBlockAttributes(blockId,0, level[block]);
		blockId++;
	}
	blocks[0]->setPrevious(blocks[BLOCKS_BUFFER-1]);

	blocks[STARTING_LINE_BLOCK]->setPaintStartLine(true);


	camera = std::vector<int>(3);
	camera[Y] = CAMERA_DISTANCE_Y;

	//initializes the car
	player = new Player(selectedPlayer_);
	player->setPosition(blocks[CAR_BLOCK]->getPosition());
	playerStartingPosition = player->position[Z];

	//initializes the side enemies
	enemies = std::vector<Enemy*>(4);	

	short ia[4];
	int speedInc = 0;

	//if(world == Resources.RADIOATIVE_SUNRISE){
	//
	if(true){
		/*
		* tracks -> 0,1,2 
		* 3 Fools and 1 Heuristic 
		* enemies speed between (Player.Max - 5, Player.Max)
		*
		* tracks -> 3,4,5 
		* 3 Fools and 1 Heuristic 
		* enemies speed between (Player.Max - 4, Player.Max+1)
		*/
		ia[0] = Enemy::FOOL;
		ia[1] = Enemy::FOOL;
		ia[2] = Enemy::FOOL;
		ia[3] = Enemy::HEURISTIC;			

		if(track_ >= 3){ //tracks -> 3,4,5
			speedInc = 1;
		}
	}


	int enemyIndex = 0;

	std::vector<int> iaRandom = Util::generateRandomArray(5);		

	for (int i = 0; i < 5; i++) {
		if(i != selectedPlayer_){
			enemies[enemyIndex] = new Enemy(i,Player::MAX_SPEED-5+speedInc,Player::MAX_SPEED+speedInc,ia[iaRandom[enemyIndex]]);				
			enemyIndex++;
		}
	}

	//enemy's grid position
	std::vector<int> posEnemyOne(3); 
	posEnemyOne[0]=25;
	posEnemyOne[1]=0;
	posEnemyOne[2]=150;
	enemies[0]->setPosition(Enemy::ROAD_X_POINTS[3],posEnemyOne);
	std::vector<int> posEnemyTwo(3);
	posEnemyTwo[0]=-25;
	posEnemyTwo[1]=0;
	posEnemyTwo[2]=200;
	enemies[1]->setPosition(Enemy::ROAD_X_POINTS[1],posEnemyTwo);
	std::vector<int> posEnemyThree(3);
	posEnemyThree[0]=20;
	posEnemyThree[1]=0;
	posEnemyThree[2]=250;
	enemies[2]->setPosition(Enemy::ROAD_X_POINTS[3],posEnemyThree);		
	std::vector<int> posEnemyFour(3);
	posEnemyFour[0]=-20;
	posEnemyFour[1]=0;
	posEnemyFour[2]=300;
	enemies[3]->setPosition(Enemy::ROAD_X_POINTS[1],posEnemyFour);		

	player->setNumberOfNitros(3);

	alreadyStarted = true;
	update(0);
	alreadyStarted = false;

	levelStartTime = 0;
	pausedTime = 0;

	updateRaceRank();
}

Road::~Road(){
	
	#ifdef WIN32
	//delete engine;
	//delete go;
	#else	
	//destroy engineSND;
	#endif
	

	FreeAll(blocks);
	FreeAll(bufferHelper);
	FreeAll(camera);
	FreeAll(enemies);

	delete player;
}

/**
* Update method, called before each frame painting
* @param keyPressing
*/
void Road::update(int keyPressing){
	if(keyPressing & UP){
		endGame();
	}

	if(frames%60){
		updateRaceRank();
	}

	//
	if(false){
	}
	else{
		if(!raceOver_){			
			if(alreadyStarted){
				if(!soundPlaying){
					//engine->play(true);
					soundPlaying=true;
				}
				//wcsound_set_pitch(0,(player->speedAxisZ * 6)/165.0f + 0.5f);

				//wcsound_set_pitch(0,(player->speedAxisZ * 6)/300.0f + 0.5f);
				//printf("%d\n",player->speedAxisZ * 6);

				//updates the player
				player->update(keyPressing,enemies,blocks);

				//changes the position of the camera according to the player
				camera[Z] = player->position[Z] - (CAR_BLOCK+1)*Block::BLOCK_LENGTH;
				//camera[Z] = player->position[Z] - 100;
				camera[X] = player->position[X];
				camera[Y] = player->position[Y] + CAMERA_DISTANCE_Y;


				//calculates the angle of each block
				for(int i = 0; i < BLOCKS_BUFFER; i++){
					blocks[i]->updateAngle(i);
				}

				//checks if new blocks are necessary
				checkNewBlocks(); 

				if(!raceOver_){

					//updates each block of the buffer
					for(int i = 0; i < BLOCKS_BUFFER; i++){
						blocks[i]->update(camera);
					}

					//projects the player position on the 2D plane
					player->projectPosition(camera);

					//blocks[CAR_BLOCK].addPlayerToPaint(player);

					//calculate the car place checking enemies Z pos.
					int playerPlace = 1;

					//updates the enemies and projects it on the 2D plane and put it's index on the block
					//that it is to be painted on the block paint
					for(int i = 0; i < enemies.size(); i++){
						enemies[i]->update(camera,blocks,player,enemies);

						int enemyZpos = enemies[i]->position[Z];

						if(player->position[Z] < enemyZpos){
							playerPlace++;
						}

						for(int j = 0; j < BLOCKS_BUFFER-1; j++){
							if(enemyZpos <= blocks[j]->getPosition()[Z]){
								blocks[j]->addIdEnemyToPaint(i);
								break;
							}
						}

					}

					updateSinalPosition();
					player->setPlace(playerPlace);

				}
			}
			else{
				//keeping adding the cars to their respective blocks to be painted.
				addCarsToPaint();

				if(pausedTime > 0){
					levelStartTime += pausedTime;
					pausedTime = 0;
				}
				//long time = System.currentTimeMillis() - levelStartTime;
				long time = (frames*66.0f);//aprox de frames*1000/fps 

				paintSinal = true;

				if(time < (STAGE_STARTING_TIME_MILLISECONDS/6)){ //luz 1
					largadaFrame=0;
					//sinal.setFrame(0);
					//
				}
				else if(time < (2*STAGE_STARTING_TIME_MILLISECONDS/6)){ //luz 2 
					largadaFrame=1;
					//sinal.setFrame(1);
					//
				}
				else if(time < (3*STAGE_STARTING_TIME_MILLISECONDS/6)){// luz 3
					largadaFrame=2;
					//sinal.setFrame(2);
					//
				}
				else if(time < (4*STAGE_STARTING_TIME_MILLISECONDS/6)){// luz 4
					if(largadaFrame==2){
						#ifdef WIN32
							//go->play(false);
						#endif
					}
					largadaFrame=3;
					
					//sinal.setFrame(3);
					//
				}
				else{
					alreadyStarted = true;
				}
			}
		}
	}
	frames++;
}

void Road::updateSinalPosition() {
	//check the player position to change the sinal frame and give the
	//movement impression
	if(paintSinal && (player->position[Z]>(playerStartingPosition + 25) )){
		if(player->position[Z] < 125){
			//sinal.setFrame(4);
			//
		}
		else if(player->position[Z] < (playerStartingPosition + 50)){
			//sinal.setFrame(5);
			//
		}
		else if(player->position[Z] < (playerStartingPosition + 100)){
			//sinal.setFrame(6);
			//
		}
		else{
			paintSinal = false;
			//sinal = NULL;
			//
			//Resources.freeBufferedImage(Resources.IMG_START_SINAL);
			//
		}
	}
}

/**
* Checks if it is necessary to insert new blocks
*/
void Road::checkNewBlocks(){
	//checa quantos blocos foram ultrapassados pela camera e devem sair do array de blocos.
	int blocksOff = 0;
	while(blocks[blocksOff]->position[Z] < camera[Z]){
		blocksOff++;
	}

	if(blocksOff > 0){
		//copia todos os blocos ultrapassados pela camera
		for(int i = 0; i < blocksOff; i++){

			if(blocks[i]->isPaintStartLine()){
				blocks[i]->setPaintStartLine(false);

				//correção para o contador de voltas do player nao ultrapassar o total
				//quando ele completar a ultima volta.
				if(player->getLaps()< NUMBER_OF_LAPS){
					player->addLap();
				}
				else{
					endGame();
				}
			}

			bufferHelper[i] = blocks[i];
		}

		//desloca os blocos que nao foram ultrapassados pela camera para o inicio do array
		int deslocatedBlocks = BLOCKS_BUFFER-blocksOff;
		for(int i = 0; i < deslocatedBlocks; i++){
			blocks[i] = blocks[i+blocksOff];

		}
		//bloco novos blocos no fim do array para substituir os que foram retirados
		for(int i = deslocatedBlocks; i < BLOCKS_BUFFER; i++){
			blocks[i] = bufferHelper[i-deslocatedBlocks];
			int block = blockId%level.size();
			blocks[i]->setBlockAttributes(blockId,0, level[block]);
			blocks[i]->updateAngle(i-deslocatedBlocks);
			if(block == STARTING_LINE_BLOCK){
				blocks[i]->setPaintStartLine(true);
			}

			blockId++;
		}
	}
}

void Road::endGame() 
{
	frames=0;
	raceOver_ = true;
	//generate the race rank
	updateRaceRank();

	//free game objects
	freeGameObjects();
}

void Road::updateRaceRank() {
	raceEndRank = std::vector<int>(5);
	raceEndRank[0] = player->position[Z];
	for (int j = 1; j < raceEndRank.size(); j++) {
		raceEndRank[j] = enemies[j-1]->position[Z];			
	}
	Util::quickSort(&raceEndRank);		

	//TODO: [Pedro]se os Z forem iguais, irá repetir um jogador.
	for (int j = 0; j < raceEndRank.size(); j++) {
		int z = raceEndRank[j];
		if(z == player->position[Z]){
			raceEndRank[j] = player->getSelectedChar();
		}
		else{
			for (int k = 0; k < enemies.size(); k++) {
				if(z == enemies[k]->position[Z]){
					raceEndRank[j] = enemies[k]->id;
					break;
				}
			}
		}
	}

	std::vector<int> temp(raceEndRank.size());
	for (int j = 0; j < temp.size(); j++) {
		temp[j] = raceEndRank[temp.size() -1 -j];
	}
	raceEndRank = temp;
}

/**
* Returns the current Y position of the player
* @return
*/
int Road::getCurrentY(){
	return blocks[3]->getPosition()[1];
}


/**
* Paints the game on the screen
* @param g
*/
void Road::paint(bool isPaused){

	blocks[CAR_BLOCK]->addPlayerToPaint(player);

	for(int i = 0; i < enemies.size(); i++){
		int enemyZpos = enemies[i]->position[Z];

		for(int j = 0; j < BLOCKS_BUFFER-1; j++){
			if(enemyZpos <= blocks[j]->getPosition()[Z]){
				blocks[j]->addIdEnemyToPaint(i);
				break;
			}
		}
	}


	for(int i = BLOCKS_BUFFER-1; i >= 0; i--){
		blocks[i]->paint(enemies,	isPaused);			
		blocks[i]->setIdEnemiesToPaint(std::vector<int>());
	}

	if(!raceOver_ && !isPaused){
		
		if(player->nitro){
			spriteBatchDraw(400,240,GL2D_CENTER,Util::textures[Util::RADIALBLUR],false,0,6,6,1,1,1,0.2f);
		}

		if(!paintSinal){

			if(!Util::accel){
				spriteBatchDraw(0,240,GL2D_CENTER_VERTICAL| GL2D_FLIP_H, Util::textures[Util::JOYPAD],false,0,1,1,1,1,1,0.5f);
				spriteBatchDraw(800-64,240,GL2D_CENTER_VERTICAL, Util::textures[Util::JOYPAD], false,0,1,1,1,1,1,0.5f);
			}
			spriteBatchDraw(14, 30,0, Util::textures[Util::SHADE1]);
			spriteBatchDraw(800-14, 480-30,GL2D_BOTTOM | GL2D_RIGHT, Util::textures[Util::SHADE2]);

			spriteBatchDraw(89, 55, 0, Util::textures[Util::RANK]);
			spriteBatchDraw(597, 30,0, Util::textures[Util::LAPS]);

			int speed = player->speedAxisZ * 6;
			std::stringstream st;
			st << speed;
			speedStr=st.str();
#ifdef FONT
			m_pFontAtlas->GetFont(0)->DrawString(693,400, speedStr, WHITE);
#endif
			int playerPos;
			for(int i=0;i<5;i++){
				if(raceEndRank[i]==0)
					playerPos=i+1;
#ifdef FONT
				m_pFontAtlas->GetFont(1)->DrawString(100,(78+i*19), players[raceEndRank[i]],raceEndRank[i]==0?0x2489B0:WHITE);
#endif
				spriteBatchDraw(14,(82+i*19),0, Util::textures[Util::RANK1]);
			}
			st.str("");
			st << playerPos;
			speedStr=st.str();
#ifdef FONT
			m_pFontAtlas->GetFont(2)->DrawString(64,16, speedStr, WHITE);
#endif

			st.str("");
			st << player->getLaps();
			speedStr=st.str();
#ifdef FONT
			m_pFontAtlas->GetFont(2)->DrawString(640,16, speedStr, WHITE);
#endif

			
		}else{
			spriteBatchDraw(0,150,GL2D_CENTER_VERTICAL, Util::textures[Util::BANDEIRA]);
			if(largadaFrame==0){
				spriteBatchDraw(400,150,GL2D_CENTER, Util::textures[Util::LARGADA1]);
			}
			if(largadaFrame==1){
				spriteBatchDraw(400,150,GL2D_CENTER, Util::textures[Util::LARGADA2]);
			}
			if(largadaFrame==2){
				spriteBatchDraw(400,150,GL2D_CENTER, Util::textures[Util::LARGADA3]);
			}
			if(largadaFrame==3){
				spriteBatchDraw(400,150,GL2D_CENTER, Util::textures[Util::GO]);
			}
		}
	}
	else if(raceOver_){
		if(frames<40){
			std::stringstream st;
			st << player->getPlace();
			speedStr=st.str();
			spriteBatchDraw(200,240,GL2D_CENTER, Util::textures[Util::LEO]);
			m_pFontAtlas->GetFont(2)->DrawString(350,150, "FINAL POSITION: "+speedStr, 0xff0000);
			m_pFontAtlas->GetFont(2)->DrawString(350,250, "\"AS MINA PIRA VÉI\"", WHITE);
		}else{
			goBack=true;
		}
	}
}

/**
* Temporary method that constructs the level
*/
void Road::setLevel(int world, int track){

	//if(world == Resources.RADIOATIVE_SUNRISE){
	if(true){

		if(track == 0){ //Mundo 1 - Pista1
			level = std::vector<int>(400);
			for(int i = 0; i < level.size(); i++){
				if(i >= 70 && i < 160){
					level[i] = 2;
				} else if(i >= 260 && i < 350){
					level[i] = 2;
				} else {
					level[i] = 0;
				}
			}
		}
	}		
}

void Road::addCarsToPaint(){
	blocks[CAR_BLOCK]->addPlayerToPaint(player);
	for(int i = 0; i < enemies.size(); i++){
		int enemyZpos = enemies[i]->position[Z];

		for(int j = 0; j < BLOCKS_BUFFER-1; j++){
			if(enemyZpos <= blocks[j]->getPosition()[Z]){
				blocks[j]->addIdEnemyToPaint(i);
				break;
			}
		}
	}
}

int Road::getPlayerX(){
	return player->position[X];
}

int Road::getHorizon(){
	return TRANSLATE_Y + blocks[BLOCKS_BUFFER-1]->getTop();
}

bool Road::isGameStarted(){
	return alreadyStarted;
}

bool Road::raceOver(){
	return raceOver_;
}

Player* Road::getPlayer(){
	return player;
}

std::vector<int> Road::getRaceEndRanking(){
	return raceEndRank;
}

void Road::freeGameObjects(){
	//blocks = NULL;
	//bufferHelper = NULL;
	//camera = NULL;
	//player = NULL;


	/*enemies = NULL;
	playerPosition = NULL;
	sinal = NULL;
	numbers = NULL;
	lapsIcon = NULL;
	playerNitro = NULL;
	rightSoftkey = NULL;
	kilometer = NULL;*/
	//
}	



