#include <stdio.h>
#include <vector>
#include "Util.h"

class Enemy;
class Block;

class Player {
public:

	/**
	 * Attributes that represents the three axis
	 */
	static const int X = 0;
	static const int Y = 1;
	static const int Z = 2;

	/**
	 * Player car attributes
	 * TODO: must be set according to car selection
	 */
	static const int ACCEL;
	static const int BREAK;
	static const int MAX_SPEED;
	static const int MAX_NITRO_SPEED;
	static const int MAX_CURVE_SPEED;
	static const int MAX_OUT_OF_ROAD_SPEED;

	int spriteWidth;

	/**
	 * this is used to set the minimum speed that the car should be to do a curve
	 */
	const static int MIN_CURVE_SPEED = 5;

	/**
	 * this is used to set the speed inc during the nitro
	 */
	const static int NITRO_SPEED_INC = 1;

	/**
	 * Horizontal speed of the car, when the player clicks the button
	 * to move right or left.
	 * TODO: it must take into consideration the current speed 
	 */
	static const int HORIZONTAL_SPEED = 6;

	/**
	 * Current speed of the player car on Z Axis
	 */
	int speedAxisZ;

	/**
	 * Current speed of the player car on X Axis
	 */
	int speedAxisX;

	/**
	 * Number of frames of the car image
	 */
	static const int CAR_IMAGE_NUM_FRAMES = 5;

	static const int CAR_IMAGE_NUM_SPRITES = 3;



	/**
	 * Car sprite
	 */
	//Sprite[] carSprite;
	//

	int spriteIndex;

	/**
	 * Position of the player car in the 3D
	 */
	std::vector<int> position;
	 
	/**
	 * Position of the player car, in relation to the camera
	 */
	std::vector<int> view;

	/**
	 * Position of the player car in the 2D
	 */
	std::vector<int> refPos;

	/**
	 * Place of the car on the race e.g: 1st, 2nd...
	 */
	int place;

	/**
	 * number of completed laps
	 */
	int laps;

	int curve;

	int balance;

	//dash -- nitro

	bool nitro;

	//Sprite dash;
	//
	int dashFrame;

	int nitroTime;

	static const int NITRO_MAX_TIME = 50;

	int numberOfNitros;



	int selectedChar;

	/**
	 * Class constructor
	 */
	Player(int car);

	void useNitro();

	void checkNitro();

	void checkCurve();

	void doBalanceEffect();

	bool isOutOfRoad(int futureX, int futureZ, std::vector<Block*> blocks);

	Enemy* hasCollisionWithEnemies(int futureX, int futureZ, std::vector<Enemy*> enemies);

	/**
	 * Reads the input and updates the player position
	 * @param keyPressing
	 * @param carBlock 
	 */
	void update(int keyPressing,std::vector<Enemy*> enemies,	std::vector<Block*> blocks);

	/**
	 * Sets the player position
	 * @param position
	 */
	void setPosition(const std::vector<int>& position);

	/**
	 * Changes the player position on the 3D
	 * @param axis
	 * @param value
	 */
	void setPosition(int axis, int value);

	/**
	 * Projects the current position of the 3D in the 2D plane
	 * @param camera
	 */
	void projectPosition(const std::vector<int>& camera);

	/**
	 * Basic projection calculation
	 * @param v
	 * @param z
	 * @return
	 */
	int project(int v, int z);

	/**
	 * Paints the player on screen
	 * @param g
	 */
	void paint();

	int getPlace();

	void setPlace(int place);

	void addLap();

	int getLaps();

	int getNumberOfNitros();

	void setNumberOfNitros(int numberOfNitros);

	int getPlayerRefX();

	int getSelectedChar();
};
