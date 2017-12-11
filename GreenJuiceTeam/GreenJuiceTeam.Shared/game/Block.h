#include <iostream>
#include <vector>
#include <list>

class Polygon;
class Player;
class Enemy;

class Block {
public:


	/**
	 * Flag used during the blocks creation
	 */
	static bool FLAG_COLOR;

	/**
	 * Dinamic drawings colors
	 */
	static const int COLOR_1 = 0x112222;
	static const int COLOR_2 = 0x223344;
	static const int LINE_COLOR = 0xFFCE42;
	static const int BORDER_COLOR = 0xFFF7E7;
	static const int BG_COLOR_1 = 0x6B6352;
	static const int BG_COLOR_2 = 0x7B7B52;

	/**
	 * Width of the road line
	 */
	static const int LINE_WIDTH = 3;

	/**
	 * Axis constants
	 */
	static const int X = 0;
	static const int Y = 1;
	static const int Z = 2;

	/**
	 * Poligon borders constants
	 */
	static const int TOP_LEFT = 0;
	static const int TOP_RIGHT = 1;
	static const int BOTTOM_LEFT = 2;
	static const int BOTTOM_RIGHT = 3;

	/**
	 * Length of each block in the Z
	 */
	static const int BLOCK_LENGTH = 25;
	

	/**
	 * Id of the block
	 */
	int id;

	/**
	 * Block road color
	 */
	int color;

	/**
	 * Flag indicating if this block should have a line
	 */
	bool drawLine;

	/**
	 * Block curvature angle
	 */
	int angle;

	/**
	 * Block 3D position
	 */
	std::vector<int> position;

	/**
	 * Block width
	 */
	int width;

	/**
	 * Block number of segments (related to the number of lines on the road)
	 * TODO: currently only suports 2 segments (1 line)
	 */
	int numSegments;

	/**
	 * Previous block on list
	 */
	Block* previous;

	/**
	 * Next block on list
	 */
	Block* next;

	/**
	 * Flag that indicates if this block should be painted
	 */
	bool paint_;

	/**
	 * Position of the block in relation to the camera
	 */
	std::vector<int> view;

	/**
	 * Accumulated angle that helps to give the impression of the road curve
	 */
	int accumulatedAngle;

	/**
	 * Road poligon
	 */
	std::vector< std::vector<int> > poligonPoints;
	Polygon* polygon;
	//private Polygon polygon = new Polygon(poligonPoints);

	/**
	 * Road dividing line points
	 */
	std::vector< std::vector<int> > linesPoints;
	Polygon* lines;
	//private Polygon lines = new Polygon(linesPoints);

	std::vector<int> idEnemiesToPaint;

	Player* player;
	
	
	bool paintStartLine;


	/**
	 * Class constructor
	 */
	Block();
	~Block();

	/**
	 * Class constructor passing the previous block as parameter
	 * @param previous
	 */
	Block(Block* previous);

	/**
	 * Sets the previous block in the list
	 * @param previous
	 */
	void setPrevious(Block* previous);

	/**
	 * Sets the block starting attributes
	 * @param id
	 * @param y - relates to the height of the block
	 * @param angle - curvature angle
	 */
	void setBlockAttributes(int id, int y, int angle);


	/**
	 * Updates the street in relation to the camera
	 * @param camera
	 */
	void update(const std::vector<int>& camera);

	/**
	 * Updates the angle of the block
	 * @param pos - the position of the block in the array
	 */
	void updateAngle(int pos);



	/**
	 * Returns the position of the block, related to its angle and the Z position
	 * @param positionZ
	 * @return
	 */
	int getY(int positionZ);

	int getX(int oldEnemyX,int newEnemyZ,int oldEnemyZ,int relativeX);


	/**
	 * Basic projection calculation
	 * @param v
	 * @param z
	 * @return
	 */
	int project(int v, int z);

	/**
	 * Paints the block on the screen
	 * @param g
	 * @param objs
	 */
	void paint(const std::vector<Enemy*>& enemies,bool isPaused);

	/**
	 * Returns the position of the Block::
	 * @return
	 */
	std::vector<int> getPosition();

	Polygon* getPoligon();
	std::vector<int> getIdEnemiesToPaint();

	void setIdEnemiesToPaint(const std::vector<int>& idEnemiesToPaint);
	void addIdEnemyToPaint(int idEnemiesToPaint);

	void addPlayerToPaint(Player* player);
	int getWidth();
	int getTop();
	
	bool isPaintStartLine();

	void setPaintStartLine(bool paintStartLine);

	void init();
};
