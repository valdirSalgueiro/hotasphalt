#include "Block.h"
#include "Polygon.h"
#include "Road.h"
#include "Enemy.h"
#include "Player.h"
#include "core/gl2d.h"

bool Block::FLAG_COLOR=true;

void Block::init()
{
	//POGGGG, culpa do C++ ;/

	drawLine=false;	 
	paint_=false;
	paintStartLine=false;

	id=0;
	color=0;
	angle=0;
	width=0;
	numSegments=0;
	accumulatedAngle=0;

	//

	player=NULL;
	position = std::vector<int>(3);
	view = std::vector<int>(3);

	poligonPoints = std::vector< std::vector<int> >(4, std::vector<int>(2));
	polygon = new Polygon(&poligonPoints);
	linesPoints = std::vector< std::vector<int> >(4, std::vector<int>(2));
	lines = new Polygon(&linesPoints);

	if(FLAG_COLOR){
		color = COLOR_1;
		drawLine = true;
	} else {
		color = COLOR_2;
		drawLine = false;
	}
	FLAG_COLOR = !FLAG_COLOR;

	
}

Block::~Block(){
	FreeAll(poligonPoints);
	FreeAll(linesPoints);
}

Block::Block(){
	init();
}

/**
* Class constructor passing the previous block as parameter
* @param previous
*/
Block::Block(Block* previous){
	init();
	setPrevious(previous);
}

/**
* Sets the previous block in the list
* @param previous
*/
void Block::setPrevious(Block* previous_){
	previous = previous_;
	previous->next = this;
}

/**
* Sets the block starting attributes
* @param id
* @param y - relates to the height of the block
* @param angle - curvature angle
*/
void Block::setBlockAttributes(int id_, int y, int angle_){
	id = id_;
	position[Y] = y;
	position[Z] = id*BLOCK_LENGTH;
	angle = angle_;
	width = 140;
	numSegments = 2;
}


/**
* Updates the street in relation to the camera
* @param camera
*/
void Block::update(const std::vector<int>& camera){
	paint_=true;

	//The view is the difference between the block position and the camera position
	view[Z] = position[Z] - camera[Z];
	view[Y] = camera[Y] - position[Y]; //The Y is inverted - it raises as it goes up in the screen
	view[X] = position[X] - camera[X];

	//The Poligon points represents the road of the block
	//The projected Y position is the same on the TOP_LEFT and TOP_RIGHT
	int topY = project(view[Y],view[Z]);
	poligonPoints[TOP_LEFT][Y] = topY;
	poligonPoints[TOP_RIGHT][Y] = topY;

	//The TOP_LEFT and TOP_RIGHT projected position is the position
	//of the view plus and minus (respectively) half the road width.
	poligonPoints[TOP_LEFT][X] = project(view[X]-width/2,view[Z]);
	poligonPoints[TOP_RIGHT][X] = project(view[X]+width/2,view[Z]);


	//It gets the bottom position from the previous top position
	poligonPoints[BOTTOM_LEFT][Y] = previous->poligonPoints[TOP_LEFT][Y];
	poligonPoints[BOTTOM_RIGHT][Y] = previous->poligonPoints[TOP_RIGHT][Y];
	poligonPoints[BOTTOM_LEFT][X] = previous->poligonPoints[TOP_LEFT][X];
	poligonPoints[BOTTOM_RIGHT][X] = previous->poligonPoints[TOP_RIGHT][X];

	//It shouldn't paint if the position is behind the camera or in the first
	//block (it's previous position is bigger it's current) 
	if(previous->position[Z] <= camera[Z] || position[Z] - previous->position[Z] <= 0){
		paint_= false;
		return;
		//It doesn't paint if the block is too much thin
	} else if(poligonPoints[TOP_LEFT][Y] - previous->poligonPoints[TOP_LEFT][Y] >= 0){
		paint_= false;
		return;
		//Calculates the line positions	
	} else if(drawLine){
		for(int i = 1; i < numSegments; i++){
			linesPoints[TOP_LEFT][X] = project(view[X]-LINE_WIDTH/2,view[Z]);
			linesPoints[TOP_LEFT][Y] = poligonPoints[TOP_LEFT][Y];

			linesPoints[TOP_RIGHT][X] = project(view[X]+LINE_WIDTH/2,view[Z]);
			linesPoints[TOP_RIGHT][Y] = poligonPoints[TOP_LEFT][Y];

			linesPoints[BOTTOM_LEFT][X] = project(previous->view[X]-LINE_WIDTH/2,previous->view[Z]);
			linesPoints[BOTTOM_LEFT][Y] = previous->poligonPoints[TOP_LEFT][Y];

			linesPoints[BOTTOM_RIGHT][X] = project(previous->view[X]+LINE_WIDTH/2,previous->view[Z]);
			linesPoints[BOTTOM_RIGHT][Y] = previous->poligonPoints[TOP_LEFT][Y];
		}

	}

}

/**
* Updates the angle of the block
* @param pos - the position of the block in the array
*/
void Block::updateAngle(int pos){
	int ref = Road::CAR_BLOCK;

	if(pos == ref){
		accumulatedAngle = (angle * (view[Z]%BLOCK_LENGTH))/BLOCK_LENGTH;
		position[X] = accumulatedAngle;
		//System.out.println("update:" + id + "-" + position[X]);
	} else if(pos > ref){
		accumulatedAngle = previous->accumulatedAngle + angle;
		position[X] = previous->position[X] + accumulatedAngle;
	} else {
		//TODO
		position[X] = 0;
	}
}



/**
* Returns the position of the block, related to its angle and the Z position
* @param positionZ
* @return
*/
int Block::getY(int positionZ){
	int py = position[Y];
	int ppy = previous->position[Y];
	int ppz = previous->position[Z];
	return ppy + ((py-ppy)*(positionZ-ppz))/BLOCK_LENGTH;
}

int Block::getX(int oldEnemyX,int newEnemyZ,int oldEnemyZ,int relativeX){
	int fator1;
	int fator2;
	int fator3;

	fator1 = newEnemyZ - oldEnemyZ;
	fator2 = (position[X] + relativeX) - oldEnemyX;
	fator3 = position[Z] - oldEnemyZ;

	if(fator3 == 0) return 0;

	return (fator1 * fator2)/fator3;
}


/**
* Basic projection calculation
* @param v
* @param z
* @return
*/
int Block::project(int v, int z){
	if(z <= 0){
		z = 1;
	}
	return v*Road::PROJECTION_PLANE/z;
}

/**
* Paints the block on the screen
* @param g
* @param objs
*/
void Block::paint(const std::vector<Enemy*>& enemies,bool isPaused){
	if(paint_){
		//Paints the road
		polygon->paint(color,true,isPaintStartLine(),next != NULL);//don't draw the border is it is the last block


		if(drawLine){
			//Paints the line
			lines->paint(LINE_COLOR, linesPoints[BOTTOM_RIGHT][X] - linesPoints[BOTTOM_LEFT][X] > 0,false,true);

		}

	}

	spriteBatchDraw(400, 280, GL2D_CENTER, Util::textures[Util::SHADOW],false,0,10,0.5f,1,1,1,0.12f);
	//TODO: need to check the Z axis for the right print order.
	//draw player and enemies
	if(!isPaused){
		if(player != NULL){
			player->paint();
			player = NULL;
		}	
		for (int i = 0; i < idEnemiesToPaint.size(); i++) {
			int id = idEnemiesToPaint[i];
			enemies[id]->paint();
		}

	}
}

/**
* Returns the position of the Block::
* @return
*/
std::vector<int> Block::getPosition(){
	return position;
}

Polygon* Block::getPoligon(){
	return polygon;
}
std::vector<int> Block::getIdEnemiesToPaint() {
	return idEnemiesToPaint;
}

void Block::setIdEnemiesToPaint(const std::vector<int>& idEnemiesToPaint_) {
	idEnemiesToPaint = idEnemiesToPaint_;
}
void Block::addIdEnemyToPaint(int idEnemiesToPaint_) {
	idEnemiesToPaint.push_back(idEnemiesToPaint_);
}

void Block::addPlayerToPaint(Player *player_) {
	player = player_;
}
int Block::getWidth() {
	return width;
}
int Block::getTop(){
	return poligonPoints[TOP_LEFT][Y];
}

bool Block::isPaintStartLine() {
	return paintStartLine;
}

void Block::setPaintStartLine(bool paintStartLine_) {
	paintStartLine = paintStartLine_;
}
