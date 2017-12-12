#include "Player.h"
#include "Road.h"
#include "Block.h"
#include "Enemy.h"
//#include "TextureQuad.h"
#include "Util.h"
#include "core/gl2d.h"

#include "sound/sound.h"

#include <algorithm>
#include <iostream>

const int Player::ACCEL = 2;
const int Player::BREAK = 1;
const int Player::MAX_SPEED = 25;
const int Player::MAX_NITRO_SPEED = 35;
const int Player::MAX_CURVE_SPEED = 23;
const int Player::MAX_OUT_OF_ROAD_SPEED = 10;

sound* skid;
sound* nitroSND;
sound* collisionSND;

Player::Player(int car){
	nitro=false;

	spriteWidth=0;
	speedAxisZ=0;
	speedAxisX=0;
	spriteIndex=0;
	place=0;
	laps=0;
	curve=0;
	balance=0;
	dashFrame=0;
	nitroTime=0;
	numberOfNitros=0;
	selectedChar=0;

	skid=new sound("skid.wav",80,100,50);
	nitroSND=new sound("nitro.wav",100,100,50);
	collisionSND=new sound("collision.wav",80,100,50);
	position = std::vector<int>(3);
	view = std::vector<int>(3);
	refPos =  std::vector<int>(2);

	selectedChar = car;
	//

	speedAxisZ = 0;
	laps = 0;
}

void Player::useNitro(){

	if(!nitro && numberOfNitros <= 0
		//&& (dash != NULL)
		//
		){
			//deallocate the nitro image and variable to optimize performance.
			//dash = NULL;
			//Resources.freeBufferedImage(Resources.IMG_DASH);
			//
	}
	else{
		if(!nitro && numberOfNitros > 0){ //check if there's nitros availables
			nitroSND->play();
			nitro = true; //set this bool to indicate that the car is using nitro
			numberOfNitros--; //reduce the number of available nitros
		}
	}

	//
}

void Player::checkNitro(){
	if(nitro){
		if(nitroTime == NITRO_MAX_TIME){ //check if the nitroTime hit the max value, if yes the nitro should stop
			nitro = false;
			nitroTime = 0;
		}
		else{
			nitroTime++; //inc the nitro time

			dashFrame = nitroTime%3; //set the dash frame
			//dash.setFrame(dashFrame);
			//

			if(speedAxisZ < MAX_NITRO_SPEED){
				speedAxisZ += NITRO_SPEED_INC; //inc the speed on Z Axis
			}
		}
	}
}

void Player::checkCurve(){
	if(curve < 0){
		spriteIndex = 0;
	}
	else if (curve > 0){
		spriteIndex = 2;
	}else{
		spriteIndex = 1;
	}
}

void Player::doBalanceEffect(){
	if(speedAxisZ > 0){
		balance++;
		if(balance == 5) {
			balance = 0;
		}
	}
}

bool Player::isOutOfRoad(int futureX, int futureZ, std::vector<Block*> blocks){
	bool out = false;

	for (int i = Road::CAR_BLOCK; i < blocks.size(); i++) {

		int blockZ = blocks[i]->position[Z];

		if(futureZ <= blockZ){ //the block where the car will be
			int left = blocks[i]->position[X] - (blocks[i]->getWidth()/2);
			int right = blocks[i]->position[X] + (blocks[i]->getWidth()/2);

			//[Pedro] bug fix. On the first time this method is called, the values (left,right) are equal zero
			//this check is to avoid 
			if(left != right){
				if( futureX < left){ //check if the player if out left
					out = true;
					position[X] = left;
				}
				else if(futureX > right){ //or right
					out = true;
					position[X] = right;
				}
				else if( (futureX == left) || (futureX == right) ){
					//check if the player is on limit zone, this is used to
					//set the speed to MAX_OUT_OF_ROAD_SPEED. 
					out = true;
				}
			}
			break;
		}
	}
	return out;
}

Enemy* Player::hasCollisionWithEnemies(int futureX, int futureZ, std::vector<Enemy*> enemies){
	Enemy* enemy = NULL;

	for (int i = 0; i < enemies.size(); i++) {

		int enemyZ = enemies[i]->position[Z];
		int playerZ = position[Z];

		//check if the player is behind the enemy
		if(playerZ < enemyZ){
			int futureEnemyZ = enemyZ + enemies[i]->speedAxisZ;
			//the player is behind the enemy less than 25 on Z axis (collision on Z axis)
			if( (futureZ + Util::CAR_LENGTH) >= futureEnemyZ ){ //30 is uses as the car length

				int enemyX = enemies[i]->getSpriteRefX();
				int playerX = 0; 

				//case1.1 - The enemy is on the player's right
				if(playerX < enemyX){
					//check is the player x position plus the sprite width is bigger than
					//the enemy x position. if yes, collision.
					if( (playerX + spriteWidth) >= enemyX ){
						enemy = enemies[i];
						break;
					}
				}
				//case1.2 - The enemy is on the player's left
				else{
					//check is the player x position less the sprite width is smaller than
					//the enemy x position. if yes, collision.
					if( (playerX - spriteWidth) <= enemyX ){
						enemy = enemies[i];
						break;
					}
				}
			}
		}

	}
	return enemy;
}

/**
* Reads the input and updates the player position
* @param keyPressing
* @param carBlock 
*/
void Player::update(int keyPressing,std::vector<Enemy*> enemies,std::vector<Block*> blocks){

	//printf("%d\n",keyPressing);
	//int nitro_button=keyPressing & (1 << KeyEvent.KEYCODE_DPAD_CENTER), left=keyPressing & (1 << KeyEvent.KEYCODE_DPAD_LEFT), right=keyPressing & (1 << KeyEvent.KEYCODE_DPAD_RIGHT), down=keyPressing & (1 << KeyEvent.KEYCODE_DPAD_DOWN);
	int nitro_button=keyPressing & UP;
	int left=keyPressing & LEFT;
	int right=keyPressing & RIGHT;
	int down=keyPressing & DOWN;

	if(nitro_button!=0){ //nitro
		useNitro();
	}

	if(left!=0){ //curve to left
		//just let a curve be done if the car speed on axis Z is bigger than the minimum
		if(speedAxisZ > MIN_CURVE_SPEED){
			//variable to represent witch sprite will be set to represent the curve
			if(curve > -(CAR_IMAGE_NUM_SPRITES-1)){
				curve--;
			}
			//if the curve tick is less than zero, it means that the curve started. So, you can move the car on
			//the X axis.
			if(curve<0){
				if(curve == -1){
					speedAxisX = -HORIZONTAL_SPEED/2;
				}
				else{
					speedAxisX = -HORIZONTAL_SPEED;
				}
			}
			skid->play();
		}

	}
	else if(right!=0){ //curve to right
		if(speedAxisZ > MIN_CURVE_SPEED){ //don't let the car do a curve if the speed is less than 5

			if(curve < (CAR_IMAGE_NUM_SPRITES-1)){
				curve++;
			}
			//if the curve tick is bigger than zero, it means that the curve started. So, you can move the car on
			//the X axis.
			if(curve>0){
				if(curve == 1){
					speedAxisX = HORIZONTAL_SPEED/2;
				}
				else{
					speedAxisX = HORIZONTAL_SPEED;
				}
			}
			skid->play();
		}
	}
	else{

		//this the player isn't pressing left or right keys, the curve tick should go to zero.
		if(curve > 0){
			curve--;
		}
		else if(curve<0){
			curve++;
		}

		//while the player isn't making a curve,
		//change the Y position to give the balance effect.
		doBalanceEffect();
	}

	if(!nitro && down!=0){ //break
		speedAxisZ = std::max(0,speedAxisZ-BREAK);
	}
	else{ //acceleration effect
		if(!nitro){//if the nitro isn't activate, what some rules to accelerate.
			if(curve == 0){ //take normal acceleration if not doing a curve, 
				//break if the speed is bigger than the maximum
				//set on MAX_SPEED, or accelerate until that value.
				if(speedAxisZ > MAX_SPEED){
					speedAxisZ -= BREAK;
				}
				else{
					speedAxisZ = std::min(MAX_SPEED, speedAxisZ+ACCEL);
				}
			}
			else{
				//if doing a curve, break if the speed is bigger than the maximum
				//set on MAX_CURVE_SPEED, or accelerate until that value.
				if(speedAxisZ > MAX_CURVE_SPEED){
					speedAxisZ -= BREAK;
				}
				else{
					speedAxisZ = std::min(MAX_CURVE_SPEED, speedAxisZ+ACCEL);
				}
			}
		}
		else{ //if the nitro is activate, accelerate no matter what
			speedAxisZ = std::min(MAX_NITRO_SPEED, speedAxisZ+ACCEL);
		}
	}

	//give the effect of curve to the car
	//1. estimate the force that throw the car out of road 
	//double force = (1.4 * speedAxisZ) / Player.MAX_SPEED;
	double force = 0;
	if(nitro){
		force = (4 * speedAxisZ) / Player::MAX_CURVE_SPEED;
	}
	else{
		force = (3 * speedAxisZ) / Player::MAX_CURVE_SPEED;
	}

	//2. and throw on the opposite direction of the curve
	speedAxisX -= (blocks[Road::CAR_BLOCK]->angle * force);


	checkNitro(); //check things relative to nitro (e.g time, speed inc, frames..)
	checkCurve(); //check things relative to the curve effect (e.g frames)

	int futureX = position[X] + speedAxisX;
	int futureZ = position[Z] + speedAxisZ;

	//check if the player collides with some enemy
	Enemy* collisionEnemy = hasCollisionWithEnemies(futureX,futureZ,enemies);

	if(collisionEnemy != NULL){
		collisionSND->play();
		//position[Z] = collisionEnemy->position[Z] - Util::CAR_LENGTH;
		position[Z] = collisionEnemy->position[Z] - 25;
		//update the axis positions
		position[X] += speedAxisX;
	}
	else{
		//check if the next position will be out of road
		bool outOfRoad = isOutOfRoad(futureX,futureZ,blocks);
		if(outOfRoad){
			position[Z] += speedAxisZ;

			if(speedAxisZ > MAX_OUT_OF_ROAD_SPEED){
				//if out of road, take off the acceleration effect given before
				//and put the break effect.
				speedAxisZ = speedAxisZ - ACCEL - BREAK;
				if(speedAxisZ < MAX_OUT_OF_ROAD_SPEED){
					speedAxisZ = MAX_OUT_OF_ROAD_SPEED;
				}
			}

		}
		else{
			//update the axis positions
			position[Z] += speedAxisZ;
			position[X] += speedAxisX;
		}
	}

	//the speed on X axis should be zero on the next update tick
	speedAxisX = 0;
}

/**
* Sets the player position
* @param position
*/
void Player::setPosition(const std::vector<int>& position_){
	position[X] = position_[X] - 30; //grid pos
	position[Y] = position_[Y];
	position[Z] = position_[Z];
}

/**
* Changes the player position on the 3D
* @param axis
* @param value
*/
void Player::setPosition(int axis, int value){
	position[axis] = value;
}

/**
* Projects the current position of the 3D in the 2D plane
* @param camera
*/
void Player::projectPosition(const std::vector<int>& camera){
	view[Z] = position[Z] - camera[Z];
	view[Y] = camera[Y] - position[Y];
	view[X] = camera[X] - position[X];

	refPos[X] = project(view[X], view[Z]);
	refPos[Y] = project(view[Y], view[Z]);
}

/**
* Basic projection calculation
* @param v
* @param z
* @return
*/
int Player::project(int v, int z){
	if(z <= 0){
		z = 1;
	}
	return v*Road::PROJECTION_PLANE/z;
}

/**
* Paints the player on screen
* @param g
*/
void Player::paint() {

	int yPos = refPos[Y] + ((balance%5)/2);

	if(spriteIndex>0){
		spriteBatchDraw(400, (yPos+240), GL2D_FLIP_H | GL2D_CENTER_HORIZONTAL |GL2D_BOTTOM, Util::textures[CAR_SPRITES[selectedChar][0][spriteIndex]]);
	}else{
		spriteBatchDraw(400, (yPos+240), GL2D_CENTER_HORIZONTAL |GL2D_BOTTOM, Util::textures[CAR_SPRITES[selectedChar][0][spriteIndex]]);
	}
	
}

int Player::getPlace() {
	return place;
}

void Player::setPlace(int place_) {
	place = place_;
}

void Player::addLap(){
	laps++;
}

int Player::getLaps(){
	if(laps == 0) return 1;
	return laps;
}

int Player::getNumberOfNitros() {
	return numberOfNitros;
}

void Player::setNumberOfNitros(int numberOfNitros_) {
	numberOfNitros = numberOfNitros_;
} 

int Player::getPlayerRefX(){
	//return carSprite[spriteIndex].getRefPixelX();
	//
	//teste
	return 0;
}
int Player::getSelectedChar(){
	return selectedChar;
}
