#include "Enemy.h"
#include "Block.h"
#include "Util.h"
#include "Player.h"
#include "Road.h"
#include "sound/sound.h"

#include <algorithm>
#include <iostream>

int Enemy::ROAD_X_POINTS[5]={-40,-20,0,20,40};

extern sound* collisionSND;


Enemy::Enemy(int id_,int minSpeed_,int maxSpeed_, short ia_){
	nitro=false;

	futureRelativeXPosition=0;
	ia=0;
	id=0;
	spriteWidth=0;
	minSpeed=0;
	maxSpeed=0;
	speedAxisZ=0;
	speedAxisX=0;
	relativeXPosition=0;
	numberOfNitros=0;
	timeWithNitro=0;
	currentFrame=0;
	//


	position = std::vector<int>(3);
	view = std::vector<int>(3);
	refPos =  std::vector<int>(2);

	currentFrame=0;

	id = id_;
	minSpeed = minSpeed_;
	maxSpeed = maxSpeed_;
	ia = ia_;

	//nitro
	numberOfNitros = 3;
	timeWithNitro = 0;
	nitro = false;


}

void Enemy::useNitro(){
	if(timeWithNitro < NITRO_DURATION){
		timeWithNitro++;
	}
	else{
		nitro = false;
		timeWithNitro = 0;
	}
}

void Enemy::tryNitro(Player* player){
	if(numberOfNitros > 0){

		int nitroRandom = 0;

		if(ia == FOOL){
			//the player is too far away(the nitro possibility is higher)
			if((player->position[Z] - FAR_AWAY) > position[Z] ){
				nitroRandom = Util::random(0, MAX_RANDOM_NITRO_NUMBER/2); //2%
			}
			else{
				nitroRandom = Util::random(0, MAX_RANDOM_NITRO_NUMBER); //1%
			}
		}
		else if(ia == HEURISTIC){
			//just use the nitro is he's behind the player
			if(player->position[Z] > position[Z]){
				//the player is too far away(the nitro possibility is higher)
				if((player->position[Z] - FAR_AWAY) > position[Z] ){
					nitroRandom = Util::random(0, MAX_RANDOM_NITRO_NUMBER/10); //10%
				}
				else{
					nitroRandom = Util::random(0, MAX_RANDOM_NITRO_NUMBER/4); //4%
				}
			}
		}
		//if the random is equal 1, use the nitro. This value was chosen arbitrarily
		//to give an impression of randomness 
		if(nitroRandom == 1){ 
			numberOfNitros--;
			nitro = true;
		}
	}
}

int Enemy::randomizeSpeedInc(Player* player){
	//calculate a random increment to add on the enemy minimum speed.
	int randomSpeedInc = 0;

	if(ia == FOOL){
		//case1 - The player is behind and too far away. The random increment
		//should be small
		if(player->position[Z] < (position[Z] - FAR_AWAY) ){
			randomSpeedInc = Util::random(0, (maxSpeed - minSpeed) - SPEED_VARIATION);
		}
		//case2 - The Enemy is behind the player and too far away. The random
		//increment should be bigger.
		else if(player->position[Z] > (position[Z] + FAR_AWAY)){
			randomSpeedInc = Util::random(0, (maxSpeed - minSpeed) + SPEED_VARIATION);
		}
		//case3 - The enemy is on a "normal" distance from player-> The random
		//increment should be normal. (minimum + inc <= maxSpeed)
		else{
			randomSpeedInc = Util::random(0, (maxSpeed - minSpeed));
		}
	}
	else if(ia == HEURISTIC){
		//If the enemy is near the player or the player is too far away, higher speed should have higher probability
		if((player->position[Z]+SAFE_DISTANCE) > position[Z]){
			randomSpeedInc = Util::random(((maxSpeed - minSpeed)/2), (maxSpeed - minSpeed));
		}
		//Otherwise just randomize
		else{
			randomSpeedInc = Util::random(0, (maxSpeed - minSpeed));
		}
	}
	return randomSpeedInc;
}

int Enemy::getSpeedAxisX(std::vector<Block*> blocks, int oldZ){
	//to calculate the new X position I need to know in witch block the enemy is.
	for(int i = 0; i < blocks.size(); i++){

		std::vector<int> pos =  blocks[i]->getPosition();

		if(position[Z] <= pos[Z]){
			//the enemy is in this block

			//this method will calculate the new X
			int xVariation = blocks[i]->getX(position[X],position[Z],oldZ,relativeXPosition);
			return xVariation;
		}
	}
	return 0;
}

void Enemy::updateView(const std::vector<int>& camera){
	//updating the view
	view[Z] = position[Z] - camera[Z];
	view[Y] = camera[Y] - position[Y];
	view[X] = position[X] - camera[X];
}

void Enemy::updateSprite() {
	//chose witch sprite should be painted according to the distance of the camera
	if(view[Z] < Block::BLOCK_LENGTH * BLOCK_SPRITE_FRAME_1){
		currentFrame = 0;
	} 
	else if(view[Z] < Block::BLOCK_LENGTH * BLOCK_SPRITE_FRAME_2){
		currentFrame = 1;
	} 
	else if(view[Z] < Block::BLOCK_LENGTH * BLOCK_SPRITE_FRAME_3){
		currentFrame = 2;
	}
	else if(view[Z] < Block::BLOCK_LENGTH * BLOCK_SPRITE_FRAME_4){
		currentFrame = 3;
	}
	else {
		currentFrame = 4;
	}

}

void Enemy::updateRelativeXPosition(){

	if(relativeXPosition != futureRelativeXPosition){
		if(relativeXPosition < futureRelativeXPosition){
			relativeXPosition++;
		}
		else{
			relativeXPosition--;
		}
	}
	else{
		int random = Util::random(1,10);

		if(relativeXPosition == ROAD_X_POINTS[0]){
			if(random == 1){ //10%
				futureRelativeXPosition = 0;
			}
			else if(random <= 4){ //30%
				futureRelativeXPosition = -20;
			}
			//60% ficar onde está
		}
		else if(relativeXPosition == ROAD_X_POINTS[1]){
			if(random <= 2){ //20%
				futureRelativeXPosition = -40;
			}
			else if(random <= 4){ //20%
				futureRelativeXPosition = 0;
			}
			else if(random<=5){ //10%
				futureRelativeXPosition = 20;
			}
			//50% fica onde está
		}
		else if(relativeXPosition == ROAD_X_POINTS[2]){
			if(random <= 1){ //10%
				futureRelativeXPosition = -40;
			}
			else if(random <= 3){ //20%
				futureRelativeXPosition = -20;
			}
			else if(random <=5){ // 20%
				futureRelativeXPosition = 20;
			}
			else if(random <= 6){ //10%
				futureRelativeXPosition = 40;
			}
		}
		else if(relativeXPosition == ROAD_X_POINTS[3]){
			if(random <= 2){ //20%
				futureRelativeXPosition = 40;
			}
			else if(random <= 4){ //20%
				futureRelativeXPosition = 0;
			}
			else if(random<=5){ //10%
				futureRelativeXPosition = -20;
			}
			//50% fica onde está
		}
		else if(relativeXPosition == ROAD_X_POINTS[4]){
			if(random == 1){ //10%
				futureRelativeXPosition = 0;
			}
			else if(random <= 4){ //30%
				futureRelativeXPosition = 20;
			}
			//60% ficar onde está
		}
	}
}

void Enemy::update(const std::vector<int>& camera, std::vector<Block*> blocks, Player* player,std::vector<Enemy*> enemies){

	//calculates the player speed on Z axis
	if(speedAxisZ < minSpeed){
		speedAxisZ += ACCEL;
	}
	else{
		int randomSpeedInc = randomizeSpeedInc(player);
		speedAxisZ = std::min(maxSpeed, minSpeed+randomSpeedInc);
	}

	//check nitro conditions
	if(!nitro){
		tryNitro(player);
	}
	else{
		useNitro();
		speedAxisZ = maxSpeed + NITRO_SPEED_INC;
	}

	//record the old z position. It will be required to calculate the new x axis
	//position.
	int oldZ = position[Z];

	//update relative X position according to IA implementation settings
	updateRelativeXPosition();

	int collision = 0;

	if(ia == HEURISTIC){
		collision = predictCollisionWithPlayer(player);
	}
	else if (ia == FOOL){
		collision = hasCollisionWithPlayer(player);
	}

	if(collision != 0){//has collision
		//printf("colidiu..\n");
		//collisionSND->play(false);
		
		position[Z] = player->position[Z] - Util::CAR_LENGTH;//25 is the car length		
		//make the enemy try to pass the player
		if(collision > 0){
			//make the enemy's car try to pass using the right
			position[X] += HORIZONTAL_SPEED;
			if(futureRelativeXPosition != ROAD_X_POINTS[4]){
				futureRelativeXPosition = ROAD_X_POINTS[4];
			}
		}
		else{
			//make the enemy's car try to pass using the left
			position[X] -= HORIZONTAL_SPEED;
			if(futureRelativeXPosition != ROAD_X_POINTS[0]){
				futureRelativeXPosition = ROAD_X_POINTS[0];
			}
		}
	}
	else{
		//update the z axis position using the speed already calculated
		position[Z] += speedAxisZ;
	}

	//calculate the speed on X axis
	speedAxisX = getSpeedAxisX(blocks, oldZ);

	//update the x axis position
	position[X] += speedAxisX;

	//update view informations
	updateView(camera);

	//projecting
	refPos[X] = project(view[X], view[Z]);
	refPos[Y] = project(view[Y], view[Z]);

	updateSprite();
}

int Enemy::hasCollisionWithPlayer(Player* player){

	int enemyZ = position[Z];
	int playerZ = player->position[Z];

	//check if the player is behind the enemy
	if(enemyZ < playerZ){
		int futureEnemyZ = enemyZ + speedAxisZ;
		//the player is behind the enemy less than 25 on Z axis (collision on Z axis)
		if( (futureEnemyZ + Util::CAR_LENGTH) >=  playerZ){ //25 is uses as the car length

			int enemyX = getSpriteRefX();
			int playerX = 0; 

			//case1.1 - The enemy is on the player's right
			if(playerX < enemyX){
				//check is the player x position plus the sprite width is bigger than
				//the enemy x position. if yes, collision.
				if( (playerX + spriteWidth) >= enemyX ){
					return 1;
				}
			}
			//case1.2 - The enemy is on the player's left
			else{
				//check is the player x position less the sprite width is smaller than
				//the enemy x position. if yes, collision.
				if( (playerX - spriteWidth) <= enemyX ){
					return -1;
				}
			}
		}
	}
	return 0;
}

int Enemy::predictCollisionWithPlayer(Player* player){

	int enemyZ = position[Z];
	int playerZ = player->position[Z];

	//check if the player is behind the enemy
	if(enemyZ < playerZ){
		int futureEnemyZ = enemyZ + speedAxisZ;
		//the player is behind the enemy less than SAFE_DISTANCE on Z axis (possibility of collision on Z axis)
		if( (futureEnemyZ + SAFE_DISTANCE) >=  playerZ){ 

			int enemyX = getSpriteRefX();
			int playerX = 0; 

			//case1.1 - The enemy is on the player's right
			if(playerX < enemyX){
				//check is the player x position plus the sprite width is bigger than
				//the enemy x position. if yes, collision.
				if( (playerX + spriteWidth) >= enemyX ){
					return 1;
				}
			}
			//case1.2 - The enemy is on the player's left
			else{
				//check is the player x position less the sprite width is smaller than
				//the enemy x position. if yes, collision.
				if( (playerX - spriteWidth) <= enemyX ){
					return -1;
				}
			}
		}
	}
	return 0;
}


void Enemy::setPosition(int relativePos, const std::vector<int>& position_){
	position[X] = position_[X];
	position[Y] = position_[Y];
	position[Z] = position_[Z];

	relativeXPosition = relativePos;
	futureRelativeXPosition = relativePos;
}


int Enemy::project(int v, int z){
	if(z <= 0){
		z = 1;
	}
	return v*Road::PROJECTION_PLANE/z;
}

void Enemy::paint() {

	if(view[Z] < Block::BLOCK_LENGTH * Road::BLOCKS_BUFFER)
	{	
		float scale;
		if(currentFrame==0){
			scale=1;
		}	
		else{
			scale=pow(0.7f,currentFrame);
		}
		

		spriteBatchDraw(400+refPos[X], (refPos[Y]+240), GL2D_CENTER_HORIZONTAL|GL2D_BOTTOM, Util::textures[CAR_SPRITES[0][0][1]],false,0,scale,scale);
	} 
}
int Enemy::getSpriteRefX(){
	//return carSprite.getRefPixelX();
	//
	return refPos[X];
}

void Enemy::freeSprite(){
	//carSprite = NULL;
	//Resources.freeBufferedImage(CAR_IMAGES[id][0]);
	//
}

void Enemy::loadSprite(){
	//Image carImage = Resources.getBufferedImage(CAR_IMAGES[id][0]);
	//carSprite = new Sprite(carImage, carImage.getWidth()/CAR_IMAGE_NUM_FRAMES, carImage.getHeight());
	//carSprite.defineReferencePixel(carSprite.getWidth()/2, carSprite.getHeight());
	//updateSprite();
	//
}
