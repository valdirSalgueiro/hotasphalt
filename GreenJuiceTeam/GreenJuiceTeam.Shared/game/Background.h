#include <stdio.h>

class Player;

class Background {		
public:

	int parallaxWidth;
	int parallaxHeight;
	int playerX;
	//Sprite terrain;
	// 
	int horizon;
	int parallaxPos;
	int terrainFrame;
	int terrainTick;
	bool isStarted;
	int TERRAIN_COLORS[2];
	//={
	//		0x695B52,//terra
	//		0x19596C//agua
	//};
	int color;

	int world;

	int BACKGROUDS_IMG[2];
	//= {
		//Resources.IMG_GAME_BACKGROUND1,
		//Resources.IMG_GAME_BACKGROUND02
		//
	//	0,1,
	//};

	int TERRAINS_IMG[2];
	//= {
		//Resources.IMG_TERRAIN1,
		//Resources.IMG_TERRAIN3
		//
	//	0,1,
	//};

	int PARALLAX_IMG[2];
	//= {
		//Resources.IMG_CITY_PLX,
		//Resources.IMG_CITY_PLX2
		//
	//	0,1,
	//};

	Background(int world, int horizon) ;
	void paint(Player* player,bool isPaused);

	void update();

	void loadImages();

	void freeImages();

	void paintParallax() ;
	void paintTerrain(Player* player,bool isPaused);

	void setPlayerX(int x);
	void setHorizon(int horizon);
	void setMoveTerrain(bool isStarted);

};
