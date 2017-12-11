#include <stdio.h>
#include <vector>

class Player;
class Block;
class Enemy;

class Road {
public:

	/**
	 * Attributes to represent the three axis
	 */
	static const int X = 0;
	static const int Y = 1;
	static const int Z = 2;

	/**
	 * Translation graphics Y, for drawing in the perspective mode
	 */
	//public static final int TRANSLATE_Y = MainCanvas.SCREEN_HEIGHT/2;
	//
	//android 800x480
	//static const int TRANSLATE_Y = 240;
	static const int TRANSLATE_Y = 240;

	/**
	 * Number of blocks in the array buffer
	 */
	static const int BLOCKS_BUFFER = 16;

	/**
	 * Represents the distance of the projection plane to the camera.
	 * It is used for changing the size of the elements, according to the front end.
	 */
	//static const int PROJECTION_PLANE = 920;
	static const int PROJECTION_PLANE = 920;


	/**
	 * Block position that the player car will be by default
	 */
	static const int CAR_BLOCK = 2;

	/**
	 * Distance in the y axis that the camera will be from the player
	 */
	static const int CAMERA_DISTANCE_Y = 20;

	/**
	 * Camera position
	 */
	std::vector<int> camera;

	/**
	 * Block buffer
	 */
	std::vector<Block*> blocks;


	/**
	 * Extra buffer that helps when new blocks are inserted
	 */
	std::vector<Block*> bufferHelper;

	/**
	 * The player car
	 */
	Player* player;


	/**
	 * Last inserted block id
	 */
	int blockId;

	/**
	 * Level information
	 */
	std::vector<int> level;

	/**
	 * Enemies
	 */
	std::vector<Enemy*> enemies;	

	/**
	 * race control
	 */
	static const int STAGE_STARTING_TIME_MILLISECONDS = 6000;

	long levelStartTime;

	long pausedTime;

	bool alreadyStarted;

	bool raceOver_;


	//positioning

	//private Sprite playerPosition;
	//

	static const int POS_IMAGE_NUM_FRAMES = 9;
	static const int positionAllign = 2;


	//nitro

	//private Image playerNitro;
	//

	static const int ICONS_ALIGNMENT = 2;

	//private Image lapsIcon;
	//

	//private Sprite numbers;
	//

	static const int NUMBERS_IMAGE_NUM_FRAMES = 12;

	static const int NUMBER_OF_LAPS = 3;
	//private static final int NUMBER_OF_LAPS = 1;

	static const int STARTING_LINE_BLOCK = 8;

	//softkey right indicator

	//private Image rightSoftkey;
	//

	//start sinal

	//private Sprite sinal;
	//

	static const int NUMBER_FRAMES_SINAL = 7;

	bool paintSinal;

	int playerStartingPosition;

	std::vector<int> raceEndRank;

	int world;

	//kilometer image
	//private Image kilometer;
	//

	/**
	 * Class constructor
	 */
	Road(int selectedPlayer,int world, int track);
	~Road();

	/**
	 * Update method, called before each frame painting
	 * @param keyPressing
	 */
	void update(int keyPressing);

	void updateSinalPosition();


	/**
	 * Checks if it is necessary to insert new blocks
	 */
	void checkNewBlocks();

	void endGame();

	void updateRaceRank();

	/**
	 * Returns the current Y position of the player
	 * @return
	 */
	int getCurrentY();


	/**
	 * Paints the game on the screen
	 * @param g
	 */
	void paint(bool isPaused);

	void paintRightSoftkey();

	void paintNitros();

	void paintLaps();

	/*
	 * removed.
	 */

	void paintPlayerSpeed();

	/**
	 * Temporary method that constructs the level
	 */
	void setLevel(int world, int track);

	void addCarsToPaint();

	int getPlayerX();

	int getHorizon();

	bool isGameStarted();

	bool raceOver();

	Player* getPlayer();

	std::vector<int> getRaceEndRanking();

	void freeGameObjects();
	
	
	//pog
	int frames;

	int largadaFrame;

	bool goBack;

};
