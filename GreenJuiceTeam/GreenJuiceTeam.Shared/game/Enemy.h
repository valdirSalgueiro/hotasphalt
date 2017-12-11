#include <vector>
#include "Util.h"

class Player;
class Block;

class Enemy {
public:

	/**
	 * Axis constants
	 */
	static const int X = 0;
	static const int Y = 1;
	static const int Z = 2;

	/**
	 * Enemy's Constants
	 */

	static const int FAR_AWAY = 400;
	static const int SAFE_DISTANCE = 150;
	static const int SPEED_VARIATION = 2;
	static const int MAX_RANDOM_NITRO_NUMBER = 100;

	static const int BLOCK_SPRITE_FRAME_1 = 5;
	static const int BLOCK_SPRITE_FRAME_2 = 6;
	static const int BLOCK_SPRITE_FRAME_3 = 8;
	static const int BLOCK_SPRITE_FRAME_4 = 12;

	static int ROAD_X_POINTS[5];

	int futureRelativeXPosition;
	/**
	 * Enemy IA
	 */

	static const short FOOL = 0;
	static const short HEURISTIC = 1;

	short ia;

	/**
	 * enemy id
	 */

	int id;

	int spriteWidth;

	/**
	 * Enemies speed
	 */
	int minSpeed;

	int maxSpeed;

	int speedAxisZ;

	int speedAxisX;

	static const int ACCEL = 1;

	static const int NITRO_SPEED_INC = 5;

	static const int HORIZONTAL_SPEED = 3;

	/**
	 * Number of frames in the car sprite
	 */
	static const int CAR_IMAGE_NUM_FRAMES = 5;



	/**
	 * Enemy car sprite
	 */
	//Sprite carSprite;
	//

	/**
	 * Position of the enemy on 3D
	 */
	std::vector<int> position;


	/**
	 * X Position of the enemy on 3D according to first block
	 */
	int relativeXPosition;

	/**
	 * Position of the enemy in relation to the camera
	 */
	std::vector<int> view;

	//nitro
	short numberOfNitros;

	int timeWithNitro;

	static const int NITRO_DURATION = 50;

	bool nitro;

	int currentFrame;

	/**
	 * Position of the enemy on 2D
	 */
	std::vector<int> refPos;

	/**
	 * Class constructor
	 */
	Enemy(int id,int minSpeed,int maxSpeed, short ia);

	void useNitro();

	void tryNitro(Player* player);

	int randomizeSpeedInc(Player* player);

	int getSpeedAxisX(std::vector<Block*> blocks, int oldZ);

	void updateView(const std::vector<int>& camera);

	void updateSprite();

	void updateRelativeXPosition();

	void update(const std::vector<int>& camera, std::vector<Block*> blocks, Player* player,std::vector<Enemy*> enemies);

	int hasCollisionWithPlayer(Player* player);

	int predictCollisionWithPlayer(Player* player);


	void setPosition(int relativePos, const std::vector<int>& position);


	int project(int v, int z);

	void paint() ;

	int getSpriteRefX();

	void freeSprite();

	void loadSprite();
};
