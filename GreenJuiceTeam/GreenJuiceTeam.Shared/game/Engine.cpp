#include "Engine.h"
#include <time.h>
#include "sound/sound.h"

#include "Vector2D.h"

#include "GameController.h" 

#include "Tween.h"

//fonts
#include "fonts/2k12.h"
#include "fonts/AGENCYB.h"
#include "fonts/arial.h"

#include "font/FTBitmapChar.h"

glImage* backtela2;
glImage* backtela;
glImage* backpista;
glImage* garage;
glImage* about;
glImage* bandeira;
glImage* menu3;
glImage* menu2;
glImage* areyousure;
glImage* caixastransparentecashandupgrade;
glImage* terrain3;
glImage* terrain2;
glImage* terrain1;
glImage* go;
glImage* caixatransparenteladoesquerdo;
glImage* garageselection;
glImage* semdinheiro;
glImage* radial;
glImage* selecao;
glImage* caixatransparenteladodireito;
glImage* menu1;
glImage* menugarage;
glImage* onoffbackground;
glImage* carro2;
glImage* carro1;
glImage* escapamento;
glImage* leo;
glImage* shadow;
glImage* l2;
glImage* l3;
glImage* lapnumber;
glImage* shade11;
glImage* rank1;
glImage* botaoupgrade;
glImage* shade12;
glImage* menu;
glImage* l1;
glImage* caixasvaziasupgrade;
glImage* joypad;
glImage* niveldeupgrade;
glImage* options1;
glImage* rank_;
glImage* offon;
glImage* engrenagens;
glImage* onon;
glImage* offoff;
glImage* botaoback;
glImage* barrinhabranca;
glImage* onoff;

Tween myTween;
Tween myTween2;
Tween myTween3;
Tween myTween4;

Vector2D<int> buttonPlaySize(500, 50);
Vector2D<int> buttonGarageSize(500, 50);
Vector2D<int> buttonOptionsSize(50, 50);

std::vector< Vector2D<int> > buttonsPlay;
std::vector< Vector2D<int> > buttonsGarage;
std::vector< Vector2D<int> > buttonsOptions;

int selectedOption;
float alpha2;

extern void altInit(int, int);

float timerGANG;

int selectSND;
sound* soundSelect;

FontAtlas* m_pFontAtlas;

const char* szLetters = " !\"#&'()*,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ\\_abcdefghijklmnopqrstuvwxyzÁáÉéÍíÑñÓóÚú";

const unsigned int texcoords_1[] = {
	0,0,800,480, // 0        backtela2.png
	802,0,800,480, // 1      backtela.png
	0,482,800,480, // 2      backpista.png
	0,964,800,415, // 3      garage.png
	0,1381,800,192, // 4     about.png
	0,1575,799,160, // 5     bandeira.png
	802,482,662,424, // 6    menu3.png
	0,1737,648,58, // 7      menu2.png
	0,1797,577,62, // 8      areyousure.png
	0,1861,577,33, // 9      caixastransparentecashandupgrade.png
	1466,482,512,228, // 10  terrain3.png
	802,908,512,228, // 11   terrain2.png
	1316,908,512,228, // 12  terrain1.png
	0,1896,492,149, // 13    go.png
	1604,0,438,263, // 14    caixatransparenteladoesquerdo.png
	1604,265,438,43, // 15   garageselection.png
	1604,310,435,28, // 16   semdinheiro.png
	802,1138,420,402, // 17  radial.png
	1604,340,411,43, // 18   selecao.png
	802,1542,343,354, // 19  caixatransparenteladodireito.png
	1466,712,340,146, // 20  menu1.png
	1224,1138,312,186, // 21         menugarage.png
	1604,385,306,36, // 22   onoffbackground.png
	802,1898,272,118, // 23  carro2.png
	1538,1138,272,117, // 24         carro1.png
	1224,1326,257,150, // 25         escapamento.png
	1224,1478,256,316, // 26         leo.png
	579,1797,221,221, // 27  shadow.png
	1808,712,217,179, // 28  l2.png
	1830,908,215,179, // 29  l3.png
	1604,423,194,49, // 30   lapnumber.png
	1812,1138,167,148, // 31         shade11.png
	579,2020,167,18, // 32   rank1.png
	650,1737,145,37, // 33   botaoupgrade.png
	1076,1898,142,93, // 34  shade12.png
	1224,1796,133,176, // 35         menu.png
	1482,1478,128,179, // 36         l1.png
	1912,385,118,20, // 37   caixasvaziasupgrade.png
	1980,482,50,189, // 38   joypad.png
	1800,423,48,48, // 39    niveldeupgrade.png
	494,1896,46,137, // 40   options1.png
	1850,423,34,18, // 41    rank.png
	1850,443,32,26, // 42    offon.png
	2017,340,29,31, // 43    engrenagens.png
	1912,407,27,26, // 44    onon.png
	1884,443,26,18, // 45    offoff.png
	1886,423,22,15, // 46    botaoback.png
	1147,1542,21,277, // 47  barrinhabranca.png
	1912,435,19,18, // 48    onoff.png
};
glImage tex_1[49];


void Engine::init(int width_, int height_) {
	width = width_;
	height = height_;
	dir = 0;
	wcsound_start();

	startBatch(width, height);
	
	srand(time(NULL));
	reset();

	gameController = new GameController(this);

	//gameState=LOGO1;
	gameState = MENU1;
	//gameState=PLAYING;
	//gameController->startGame(0, 0, 0);

	loadTextures();

	soundSelect = new sound("select.wav", 80, 100, 50);
}

Engine::~Engine() {
	//deinit();
	//wcsound_end();
}

void Engine::reset() {
	frames = 0;
	timerGANG = 0;
	loadingScreen = 0;
	gameState = MENU1;

	alpha2 = 0.0f;
	selectedOption = 0;

	buttonsPlay.clear();
	Vector2D<int> BUTTON(400, 185);
	buttonsPlay.push_back(BUTTON);
	BUTTON = Vector2D<int>(400, 235);
	buttonsPlay.push_back(BUTTON);
	BUTTON = Vector2D<int>(400, 282);
	buttonsPlay.push_back(BUTTON);
	BUTTON = Vector2D<int>(400, 330);
	buttonsPlay.push_back(BUTTON);


	buttonsGarage.clear();
	BUTTON = Vector2D<int>(0, 178);
	buttonsGarage.push_back(BUTTON);
	BUTTON = Vector2D<int>(0, 219);
	buttonsGarage.push_back(BUTTON);
	BUTTON = Vector2D<int>(0, 260);
	buttonsGarage.push_back(BUTTON);
	BUTTON = Vector2D<int>(0, 301);
	buttonsGarage.push_back(BUTTON);
	BUTTON = Vector2D<int>(0, 342);
	buttonsGarage.push_back(BUTTON);

	buttonsOptions.clear();
	BUTTON = Vector2D<int>(470, 205);
	buttonsOptions.push_back(BUTTON);
	BUTTON = Vector2D<int>(470, 256);
	buttonsOptions.push_back(BUTTON);
	BUTTON = Vector2D<int>(470, 302);
	buttonsOptions.push_back(BUTTON);
	BUTTON = Vector2D<int>(635, 205);
	buttonsOptions.push_back(BUTTON);
	BUTTON = Vector2D<int>(635, 256);
	buttonsOptions.push_back(BUTTON);
	BUTTON = Vector2D<int>(635, 302);
	buttonsOptions.push_back(BUTTON);

	myTween.setup(240, 0, -100, Easing::BounceEaseOut);
	myTween.play();
	//myTween2.setup(120, 0, -512, Easing::CircEaseIn);
	myTween2.setup(120, 0, -1, Easing::CubicEaseOut);
	myTween2.play();
	myTween3.setup(120, 0, 319, Easing::LinearEaseOut);
	myTween3.play();
	myTween4.setup(60, 0.95f, -0.05f, Easing::SineEaseInOut, 0, 2);
	myTween4.play();
}

void Engine::loadTextures() {
	glLoadSpriteset("1.png", tex_1, 2048, 2048, 49, texcoords_1, 0);

	backtela2 = &tex_1[0];
	backtela = &tex_1[1];
	backpista = &tex_1[2];
	garage = &tex_1[3];
	about = &tex_1[4];
	bandeira = &tex_1[5];
	menu3 = &tex_1[6];
	menu2 = &tex_1[7];
	areyousure = &tex_1[8];
	caixastransparentecashandupgrade = &tex_1[9];
	terrain3 = &tex_1[10];
	terrain2 = &tex_1[11];
	terrain1 = &tex_1[12];
	go = &tex_1[13];
	caixatransparenteladoesquerdo = &tex_1[14];
	garageselection = &tex_1[15];
	semdinheiro = &tex_1[16];
	radial = &tex_1[17];
	selecao = &tex_1[18];
	caixatransparenteladodireito = &tex_1[19];
	menu1 = &tex_1[20];
	menugarage = &tex_1[21];
	onoffbackground = &tex_1[22];
	carro2 = &tex_1[23];
	carro1 = &tex_1[24];
	escapamento = &tex_1[25];
	leo = &tex_1[26];
	shadow = &tex_1[27];
	l2 = &tex_1[28];
	l3 = &tex_1[29];
	lapnumber = &tex_1[30];
	shade11 = &tex_1[31];
	rank1 = &tex_1[32];
	botaoupgrade = &tex_1[33];
	shade12 = &tex_1[34];
	menu = &tex_1[35];
	l1 = &tex_1[36];
	caixasvaziasupgrade = &tex_1[37];
	joypad = &tex_1[38];
	niveldeupgrade = &tex_1[39];
	options1 = &tex_1[40];
	rank_ = &tex_1[41];
	offon = &tex_1[42];
	engrenagens = &tex_1[43];
	onon = &tex_1[44];
	offoff = &tex_1[45];
	botaoback = &tex_1[46];
	barrinhabranca = &tex_1[47];
	onoff = &tex_1[48];

	Util::textures[Util::IMG_TERRAIN1_1] = terrain1;
	Util::textures[Util::IMG_TERRAIN1_2] = terrain2;
	Util::textures[Util::IMG_TERRAIN1_3] = terrain3;
	Util::textures[Util::IMG_GAME_BACKGROUND1] = backpista;
	Util::textures[Util::COP_CAR1_1] = carro1;
	Util::textures[Util::COP_CAR1_2] = carro2;
	Util::textures[Util::COP_CAR1_3] = carro1;
	//Util::textures[Util::IMG_PARALLAX_1]=on_on;

	Util::textures[Util::LARGADA1] = l1;
	Util::textures[Util::LARGADA2] = l2;
	Util::textures[Util::LARGADA3] = l3;
	Util::textures[Util::GO] = go;
	Util::textures[Util::BANDEIRA] = bandeira;

	Util::textures[Util::SHADE1] = shade11;
	Util::textures[Util::SHADE2] = shade12;
	Util::textures[Util::RANK] = rank_;
	Util::textures[Util::RANK1] = rank1;

	Util::textures[Util::JOYPAD] = joypad;
	Util::textures[Util::LAPS] = lapnumber;
	Util::textures[Util::SHADOW] = shadow;

	Util::textures[Util::RADIALBLUR] = radial;
	Util::textures[Util::LEO] = leo;

	m_pFontAtlas = new FontAtlas();
	m_pFontAtlas->AddFont(AGEN, AGEN_size, 36, szLetters);
	m_pFontAtlas->AddFont(AGEN, AGEN_size, 16, szLetters);
	m_pFontAtlas->AddFont(AGEN, AGEN_size, 48, szLetters);
	m_pFontAtlas->AddFont(AGEN, AGEN_size, 24, szLetters);

	m_pFontAtlas->AddFont(_k12_ttf, _k12_ttf_size, 18, szLetters);
	m_pFontAtlas->AddFont(arial_ttf, arial_ttf_size, 15, szLetters);

	m_pFontAtlas->AddFont(AGEN, AGEN_size, 30, szLetters);
	m_pFontAtlas->CreateAtlas();


}

void Engine::deinit() {
}

void Engine::update(float time) {

	if (gameState == LOGO1) {
		if (frames > 90) {
			frames = 0;
			gameState = LOGO2;
		}
	}

	if (gameState == LOGO2) {
		if (frames > 90) {
			frames = 0;
			gameState = MENU1;
		}
	}
	if (gameState == MENU1) {
		myTween2.update();
		myTween3.update();
		if (!myTween2.finished()) {
			if (alpha2 < 1)
				alpha2 += 0.005f;
		}
		else {
			frames = 0;
			gameState = MENU2;
		}
	}
	if (gameState == MENU2 || gameState == ABOUT) {
		myTween.update();
		myTween4.update();

	}
	if (gameState == GANG1) {
		timerGANG += time;
		if (timerGANG > 4) {
			timerGANG = 0;
			gameState = PLAYING;
			gameController->startGame(0, 0, 0);
		}
		if (frames % ((rand() % 10) + 1) == 0) {
			if (loadingScreen < 3) {
				loadingScreen++;
			}
			else {
				loadingScreen = 0;
			}
		}

	}

	if (gameState == PLAYING || gameState == GAMEOVER)
		playing(time);

	frames++;

}

void Engine::render(float time) {

	if (gameState == LOGO1) {
		spriteBatchBegin();
		//glSpriteNoScale(0,0,0,tela1);
		spriteBatchEnd();

	}
	else if (gameState == LOGO2) {
		spriteBatchBegin();
		//glSpriteNoScale(0,0,0,tela2);
		spriteBatchEnd();

	}
	else if (gameState == MENU1) {
		spriteBatchBegin();
		glSprite(0, 0, 0, backtela, false, 0, 1, 1, 1, 1, 1, alpha2);
		glSprite(0, 0, 0, backtela2);
		//glSprite(400,240,GL2D_CENTER,menu3,false,0,1-myTween2.num,1-myTween2.num);
		glSprite(400, 240, GL2D_CENTER, menu3, false, 0, 1 - myTween2.num, 1 - myTween2.num);
		glSprite(400 + myTween3.num, 90, GL2D_CENTER_HORIZONTAL, menu2);
		glSprite(400 + myTween3.num, 480 - 90, GL2D_CENTER_HORIZONTAL | GL2D_BOTTOM | GL2D_FLIP_V, menu2);
		spriteBatchEnd();
	}
	else if (gameState == MENU2) {
		spriteBatchBegin();
		glSprite(0, 0, 0, backtela);
		glSprite(0, 0, 0, backtela2, false, 0, 1, 1, 1, 1, 1, myTween4.num);
		glSprite(400, myTween.num, GL2D_CENTER_HORIZONTAL, menu1);
		glSprite(400, 240, GL2D_CENTER, menu3);
		glSprite(400, 90, GL2D_CENTER_HORIZONTAL, menu2);
		glSprite(400, 480 - 90, GL2D_CENTER_HORIZONTAL | GL2D_BOTTOM | GL2D_FLIP_V, menu2);
		glSprite(buttonsPlay[selectedOption].x, buttonsPlay[selectedOption].y, GL2D_CENTER, selecao);
		glSprite(400, 260, GL2D_CENTER, menu);
		spriteBatchEnd();
	}
	else if (gameState == GARAGE || gameState == GARAGE_CONFIRM) {
		spriteBatchBegin();
		glSprite(0, 0, 0, garage);
		glSprite(0, 132, 0, caixatransparenteladoesquerdo);
		glSprite(448, 40, 0, caixatransparenteladodireito);
		glSprite(497, 80, 0, escapamento);
		glSprite(480, 332, 0, botaoupgrade);

		glSprite(653, 55, 0, caixasvaziasupgrade);
		glSprite(638, 42, 0, niveldeupgrade);

		glSprite(37, 12, 0, botaoback);
		m_pFontAtlas->GetFont(3)->DrawString(70, -1, "BACK", WHITE);

		m_pFontAtlas->GetFont(5)->DrawString(461, 245, "UPGRADES HERE CAN MAKE MASSIVE", 0x00ffff);
		m_pFontAtlas->GetFont(5)->DrawString(461, 270, "DIFFERENCE TO YOUR CAR’S SPEED", 0x00ffff);
		m_pFontAtlas->GetFont(5)->DrawString(461, 295, "AND PERFORMANCE LEVEL.", 0x0ff0f0);
		m_pFontAtlas->GetFont(3)->DrawString(518, 331, "UPGRADE", 0xffffff);

		m_pFontAtlas->GetFont(4)->DrawString(497, 15, "CHOOSE UPGRADE", 0xffffff);

		glSprite(751, 10, 0, engrenagens);

		glSprite(buttonsGarage[selectedOption].x, buttonsGarage[selectedOption].y, GL2D_CENTER_VERTICAL, garageselection);
		glSprite(10, 260, GL2D_CENTER_VERTICAL, menugarage);

		if (gameState == GARAGE_CONFIRM) {
			glSprite(0, 407, 0, areyousure);
		}
		spriteBatchEnd();

	}

	else if (gameState == ABOUT) {
		spriteBatchBegin();
		glSprite(0, 0, 0, backtela);
		glSprite(0, 0, 0, backtela2, false, 0, 1, 1, 1, 1, 1, myTween4.num);
		glSprite(400, 240, GL2D_CENTER, about);
		spriteBatchEnd();

	}
	else if (gameState == GANG1) {
		spriteBatchBegin();
		glSprite(0, 0, 0, backpista);
		glSprite(200, 240, GL2D_CENTER, leo);
		m_pFontAtlas->GetFont(Util::AGENCY_2K12_18)->DrawString(100, 370, "LEO THUNNER", WHITE);

		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(330, 200, "LEO THUNNER ASSUMUI A GANG BLACK AND WHITE, QUANDO SEU IRMAO MARKUS ", WHITE);
		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(330, 230, "THUNNER MORREU DURANTE UMA DISPUTA DE CORRIDA CONTRA OS VIBORAS.", WHITE);
		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(330, 260, "AMANTES DOS MOTORES V12. A GANG BLACK AND WHITE NÃO ECONOMIZA EM ", WHITE);
		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(330, 290, "INVESTIMENTOS EM SEUS CARROS.", WHITE);
		if (loadingScreen == 0) {
			m_pFontAtlas->GetFont(Util::AGENCY_48)->DrawString(300, 400, "LOADING", WHITE);
		}
		else if (loadingScreen == 1) {
			m_pFontAtlas->GetFont(Util::AGENCY_48)->DrawString(300, 400, "LOADING.", WHITE);
		}
		else if (loadingScreen == 2) {
			m_pFontAtlas->GetFont(Util::AGENCY_48)->DrawString(300, 400, "LOADING..", WHITE);
		}
		else if (loadingScreen == 3) {
			m_pFontAtlas->GetFont(Util::AGENCY_48)->DrawString(300, 400, "LOADING...", WHITE);
		}
		spriteBatchEnd();
	}
	else if (gameState == OPTIONS) {
		spriteBatchBegin();
		glSprite(0, 0, 0, garage);

		glSprite(37, 430, 0, botaoback);
		m_pFontAtlas->GetFont(3)->DrawString(70, 416, "BACK", WHITE);

		glSprite(80, 99, 0, barrinhabranca);
		glSprite(260, 408, 0, barrinhabranca, true, DEG2RAD(-90));


		m_pFontAtlas->GetFont(6)->DrawString(144, 178, "SOUND", 0xffffff);
		m_pFontAtlas->GetFont(6)->DrawString(144, 228, "MUSIC", 0xffffff);
		m_pFontAtlas->GetFont(6)->DrawString(144, 278, "ACCELEROMETER", 0xffffff);

		glSprite(734, 255, GL2D_CENTER_VERTICAL, options1);

		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(337, 370, "TAP TO CHANGE OPTIONS", 0xffffff);
		m_pFontAtlas->GetFont(Util::AGENCY_16)->DrawString(474, 315, "TILT YOUR PHONE TO CONTROL THE CAR", 0xffffff);

		glSprite(buttonsOptions[0].x + 95, buttonsOptions[0].y, GL2D_CENTER, onoffbackground);
		glSprite(buttonsOptions[1].x + 95, buttonsOptions[1].y, GL2D_CENTER, onoffbackground);
		glSprite(buttonsOptions[2].x + 95, buttonsOptions[2].y, GL2D_CENTER, onoffbackground);

		glSprite(buttonsOptions[0].x, buttonsOptions[0].y, GL2D_CENTER, Util::sound ? onon : onoff);
		glSprite(buttonsOptions[1].x, buttonsOptions[1].y, GL2D_CENTER, Util::music ? onon : onoff);
		glSprite(buttonsOptions[2].x, buttonsOptions[2].y, GL2D_CENTER, Util::accel ? onon : onoff);

		glSprite(buttonsOptions[3].x, buttonsOptions[0].y, GL2D_CENTER, !Util::sound ? offon : offoff);
		glSprite(buttonsOptions[4].x, buttonsOptions[1].y, GL2D_CENTER, !Util::music ? offon : offoff);
		glSprite(buttonsOptions[5].x, buttonsOptions[2].y, GL2D_CENTER, !Util::accel ? offon : offoff);
		spriteBatchEnd();

	}
	else {
		spriteBatchBegin();
		gameController->paint();
		spriteBatchEnd();
	}
}


void Engine::playing(float time) {
	if (frames % 4 == 0) 
	{
		gameController->setKeypress(dir);
		gameController->update();
	}
}

void Engine::control(int type, int x, int y) {
	float scaleX;
	float scaleY;

	scaleX = width / 800.0f;
	scaleY = height / 480.0f;

	x /= scaleX;
	y /= scaleY;

	if (x > 0 && x < 220 && y>150 && y < 350) {
		dir = LEFT;
	}
	else if (x > 580 && x < 800 && y>150 && y < 350) {
		dir = RIGHT;
	}
}

void Engine::setDir(int dir) 
{
	if(gameController!=NULL && gameState == Engine::PLAYING)
	{
		this->dir = dir;
	}
}


void Engine::handleInput(int type, int x, int y) {
	float scaleX;
	float scaleY;

	scaleX = width / 800.0f;
	scaleY = height / 480.0f;

	x /= scaleX;
	y /= scaleY;

	if (gameState == MENU2) {
		//center
		x += buttonPlaySize.x / 2;
		y += buttonPlaySize.y / 2;
		for (int i = 0; i < buttonsPlay.size(); i++) {
			if ((x > buttonsPlay[i].x && x < buttonsPlay[i].x + buttonPlaySize.x) && (y > buttonsPlay[i].y && y < buttonsPlay[i].y + buttonPlaySize.y)) {
				if (i == selectedOption) {
					if (i == 0) {
						gameState = GANG1;
					}
					if (i == 1) {
						selectedOption = 0;
						gameState = GARAGE;
					}
					if (i == 2) {
						gameState = OPTIONS;
					}
					if (i == 3) {
						gameState = ABOUT;
					}
					soundSelect->play();
				}
				else {
					selectedOption = i;
				}
				break;
			}
		}
	}

	else if (gameState == OPTIONS) {
		if ((x > 0 && x < 150) && (y > 380 && y < 480)) {
			reset(); //back menu
			soundSelect->play();
			return;
		}

		x += buttonOptionsSize.x / 2;
		y += buttonOptionsSize.y / 2;
		for (int i = 0; i < buttonsOptions.size(); i++) {
			if ((x > buttonsOptions[i].x && x < buttonsOptions[i].x + buttonOptionsSize.x) && (y > buttonsOptions[i].y && y < buttonsOptions[i].y + buttonOptionsSize.y)) {
				if (i == 0) {
					Util::sound = true;
				}
				if (i == 1) {
					Util::music = true;
				}
				if (i == 2) {
					Util::accel = true;
				}
				if (i == 3) {
					Util::sound = false;
				}
				if (i == 4) {
					Util::music = false;
				}
				if (i == 5) {
					Util::accel = false;

				}
				soundSelect->play();
				break;
			}
		}
	}

	else if (gameState == GARAGE || gameState == GARAGE_CONFIRM) {
		if ((x > 450 && x < 650) && (y > 330 && y < 380)) {
			gameState = GARAGE_CONFIRM;
			soundSelect->play();
			return;
		}

		if ((x > 0 && x < 150) && (y > 0 && y < 150)) {
			reset(); //back menu
			soundSelect->play();
			return;
		}

		//center
		//x+=buttonGarageSize.x/2;
		y += buttonGarageSize.y / 2;
		for (int i = 0; i < buttonsGarage.size(); i++) {
			if ((x > buttonsGarage[i].x && x < buttonsGarage[i].x + buttonGarageSize.x) && (y > buttonsGarage[i].y && y < buttonsGarage[i].y + buttonGarageSize.y)) {
				if (i == selectedOption) {
					if (i == 0) {
					}
					if (i == 1) {

					}
					if (i == 2) {

					}
					if (i == 3) {

					}
					if (i == 4) {

					}

				}
				else {
					gameState = GARAGE;
					selectedOption = i;
				}
				break;
			}
		}

	}
	else if (gameState == ABOUT) {
		reset();
	}
}

