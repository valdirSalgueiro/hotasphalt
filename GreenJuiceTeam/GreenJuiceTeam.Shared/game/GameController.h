#include <vector>

class Background;
class Road;
class Engine;


class GameController {
public:

	/**
	 * Possible values to GameState attribute
	 */

	/**
	 * GAME_STATE_PLAYING_IDLE Constant.
	 */
	static const int GAME_STATE_PLAYING_IDLE = 0;

	/**
	 * GAME_STATE_PAUSED Constant.
	 */
	static const int GAME_STATE_PAUSED = 1;

	/**
	 * The available difficulty levels in the game
	 */
	static const int GAME_DIFFICULTY_EASY = 1;
	static const int GAME_DIFFICULTY_NORMAL = 2;
	static const int GAME_DIFFICULTY_HARD = 3;

	/**
	 * The internal state of the game.
	 */
	int gameState;

	/**
	 * Indicates the game state to next resume. It stores the last game state
	 * before paused. It can be PLAYING, PLAYER_DIED or PLAYER_WON.
	 */
	int resumeGameState;

	static const int FIXED_POINT_ADJUSTER = 8;
	static const int FIXED_POINT_ADJUSTER_MULTIPLIER = 1 << FIXED_POINT_ADJUSTER;

	Road* road;
	Background* background;
	

	bool gameOver;

	/**
	 * Constructor
	 */
	GameController(Engine* engine);

	/**
	 * Free the memory allocated the internal game controller instance.
	 */
	void freeInstance() ;

	/**
	 * Loads all game resource Loads the level indicated at levelNum parameter
	 * Starts the game
	 * 
	 * @param levelNum
	 *            indicating the number of the level to be loaded
	 */
	void startGame(int world,int track, int player);

	/**
	 * Put the game in the paused state.
	 */
	void pauseGame();

	/**
	 * Put the game in the playing state.
	 */
	void resumeGame();

	/**
	 * Updates the game state. It's called each game clock milliseconds.
	 * 
	 * @param keyStates
	 *            The state of device keys.
	 */
	void update(int keyCode);

	/**
	 * Process the update of the playing state.
	 * 
	 * @param keyStates
	 *            The state of device keys.
	 */
	void processGameUpdate(int keyCode);




	bool isGameOver();

	/**
	 * Paints the current game frame in the g graphics object. It's called each
	 * game clock milliseconds.
	 * 
	 * @param g
	 *            the Graphics object where the game frame must be painted.
	 */
	void paint();
	
	int getPlayerSelectedChar();

	/**
	 * Returns the state of the game.
	 * 
	 * @return The game´s state
	 */
	int getState();
	
	int getPlayerPosition();
	
	std::vector<int> getRaceEndRanking();

	Engine* engine;
};
