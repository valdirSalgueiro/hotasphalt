#ifndef ENGINE_H
#define ENGINE_H

#ifdef _WIN32

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#elif __ANDROID__

#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>

#endif

#include "font/PreCompile.h"
#include "font/FontAtlas.h"
#include "font/FTBitmapFont.h"

class GameController;

#include "Util.h"

class Engine {
public:

	~Engine();
	void init(int width, int height);
	void deinit();
	void update(float time);
	void render(float time);
	void playing(float time);

	void handleInput(int type, int x, int y);
	void control(int type, int x, int y);

	void setDir(int dir);
	void setTouch(bool _touch) 
	{
		touch = touch;
	}

	void reset();

	//specific methods

	void loadGLTexture(Util::Sprites sprite);

	void loadTextures();
	//

	enum STATE {
		LOGO1,
		LOGO2,
		MENU1,
		MENU2,
		OPTIONS,
		ABOUT,
		GARAGE,
		GARAGE_CONFIRM,
		GANG1,
		TITLE,
		PLAYING,
		GAMEOVER
	};

	STATE gameState;
	int width, height;
	int dir;

	bool touch;

private:
	int frames;

	int loadingScreen;

	//specific
	GameController* gameController;
	long lastTimeMillis;

};

#endif