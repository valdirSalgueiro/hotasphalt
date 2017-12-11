#include "Background.h"
#include "Player.h"
//#include "TextureQuad.h"
#include "Util.h"
#include "core/gl2d.h"

float lightangle[]={0.1,0,-0.1};
bool lightdirection[]={true,false,true};


Background::Background(int world_, int horizon_) {
	isStarted=false;
	parallaxWidth=0;
	parallaxHeight=0;
	playerX=0;
	horizon=0;
	parallaxPos=0;
	terrainFrame=0;
	terrainTick=0;
	color=0;
	world=0;

	//background = Resources.getBufferedImage(BACKGROUDS_IMG[world]);
	//
	world = world_;


	horizon = horizon_;
	//Image imageTerrain = Resources.getBufferedImage(TERRAINS_IMG[world]);
	//terrain = new Sprite(imageTerrain, imageTerrain.getWidth()/3, imageTerrain.getHeight());
	//
	parallaxPos = horizon_;
	//backgroundParallax = Resources.getBufferedImage(PARALLAX_IMG[world]);
	//
	//parallaxWidth = backgroundParallax.getWidth();
	//parallaxHeight = backgroundParallax.getHeight();
	//


	color = TERRAIN_COLORS[world];
}

void Background::update(){
	for(int i=0;i<3;i++){
		if(lightdirection[i]){
			if(lightangle[i]<0.6)	
				lightangle[i]+=0.03f;
			else
				lightdirection[i]=false;
		}
		else{
			if(lightangle[i]>-0.6)	
				lightangle[i]-=0.03f;
			else
				lightdirection[i]=true;
		}
	}
}

void Background::paint(Player* player,bool isPaused){
	//g.drawImage(background, MainCanvas.SCREEN_WIDTH/2,Road::TRANSLATE_Y, Graphics.BOTTOM | Graphics.HCENTER);
	//

	
	spriteBatchDraw(0, 350, GL2D_BOTTOM,Util::textures[Util::IMG_GAME_BACKGROUND1]);

	//spriteBatchDraw(300, 350, 64, 512,Util::textures[Util::LIGHTS1],GL2D_CENTER,true,lightangle[0]);
	//spriteBatchDraw(500, 350, 256, 512,Util::textures[Util::LIGHTS2],GL2D_CENTER,true,lightangle[1]);
	//spriteBatchDraw(600, 350, 128, 512,Util::textures[Util::LIGHTS3],GL2D_CENTER,true,lightangle[2]);


	if(isPaused){
		//g.setColor(color);
		//g.fillRect(0, background.getHeight(), MainCanvas.SCREEN_WIDTH, MainCanvas.SCREEN_HEIGHT-background.getHeight());
		//
	}
	else{
		paintParallax();
		paintTerrain(player,isPaused);

	}
}

void Background::loadImages(){
	//Image imageTerrain = Resources.getBufferedImage(TERRAINS_IMG[world]);
	//terrain = new Sprite(imageTerrain, imageTerrain.getWidth()/3, imageTerrain.getHeight());
	//backgroundParallax = Resources.getBufferedImage(PARALLAX_IMG[world]);
	//
}

void Background::freeImages(){
	//backgroundParallax = NULL;
	//terrain = NULL;
	//
}

void Background::paintParallax() {
	int parallaxPosition = playerX/5;

	//int xPos = (parallaxWidth - MainCanvas.SCREEN_WIDTH)/2;
	//
	int xPos = (parallaxWidth - 240)/2;


	if(parallaxPosition > 0){
		if(parallaxPosition > xPos){
			parallaxPosition = xPos;
		}
	}
	else{
		if(parallaxPosition < ((-1) * xPos)){
			parallaxPosition = -xPos;
		}
	}
	xPos += parallaxPosition;
	//g.drawRegion(backgroundParallax, xPos, 0, MainCanvas.SCREEN_WIDTH, parallaxHeight, Sprite.TRANS_NONE, 0, parallaxPos+2/*background.getHeight()+33*/,Graphics.BOTTOM | Graphics.LEFT/* Graphics.BOTTOM | Graphics.LEFT*/);
	//

}
void Background::paintTerrain(Player* player,bool isPaused) {

	Util::Sprites terrainSprite;
	if(terrainFrame==0)
		terrainSprite=Util::IMG_TERRAIN1_1;
	else if(terrainFrame==1)
		terrainSprite=Util::IMG_TERRAIN1_2;
	else
		terrainSprite=Util::IMG_TERRAIN1_3;

	spriteBatchDraw(0, horizon+224, GL2D_BOTTOM,Util::textures[terrainSprite]);
	spriteBatchDraw(512, horizon+224,GL2D_FLIP_H|GL2D_BOTTOM,Util::textures[terrainSprite]);
	

	if(!isPaused && isStarted){
		terrainTick += player->speedAxisZ;

		if(terrainTick >= (3*Player::MAX_SPEED)){
			terrainTick = terrainTick - (3*Player::MAX_SPEED);
		}
		terrainTick++;

		terrainFrame = terrainTick/Player::MAX_SPEED;
	}
}

void Background::setPlayerX(int x){
	playerX = x;
}
void Background::setHorizon(int horizon_){
	horizon = horizon_;
}
void Background::setMoveTerrain(bool isStarted_){
	isStarted = isStarted_;
}