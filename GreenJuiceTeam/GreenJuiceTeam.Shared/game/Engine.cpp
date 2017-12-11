#include "Engine.h"

Engine::Engine() {
}

void Engine::init(int width_, int height_)
{
	width = width_;
	height = height_;
	startBatch(width_, height_);
	logo = new glImage();
	glLoadSprite("logo.png", logo);


	//
	controlBasePos.x = 100;
	controlBasePos.y = 380;
	controlKnobPos = controlBasePos;
}

Engine::~Engine() {
	delete logo;
	spriteBatchDestroy();
}

void Engine::update(float time)
{
}

void Engine::render(float time) {
	spriteBatchBegin();
	glSprite(0, 0, GL2D_NONE, logo);

#ifdef __APPLE__ || __ANDROID_API__
	glSprite(controlBasePos.x, controlBasePos.y, GL2D_NO_SCALE | GL2D_CENTER, controlBase, false, 0, 1, 1, 1.0f, 1.0f, 1.0f, 0.5f);
	glSprite(controlKnobPos.x, controlKnobPos.y, GL2D_NO_SCALE | GL2D_CENTER, controlKnob, false, 0, 1, 1, 1.0f, 1.0f, 1.0f, 0.5f);
#endif

	spriteBatchEnd();
}

void Engine::setTouch(bool _touch)
{
	touch = _touch;
}
