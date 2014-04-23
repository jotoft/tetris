#pragma once
#include <vector>
#include "Game.h"

enum KEYS {
	KEY_ROTATECLOCK,
	KEY_ROTATECC,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_UP
};

class Input
{
public:
	Input(Game *game);
	~Input(void);
	void keyUp(KEYS key);
	int keyDown(KEYS key);
	bool isKeyDown(KEYS key);

private:
	std::vector<KEYS> keysDown;
	Game *game;
};

