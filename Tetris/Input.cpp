#include "Input.h"
#include "Game.h"


Input::Input(Game *game)
{
	this->game = game;
}


Input::~Input(void)
{
}


void Input::keyUp(KEYS key)
{

}


int Input::keyDown(KEYS key)
{
	switch(key)
	{
	case KEY_ROTATECC:
		game->rotatePiece(CCLOCKWISE);
		break;
	case KEY_ROTATECLOCK:
		game->rotatePiece(CLOCKWISE);
		break;
	case KEY_LEFT:
		game->movePiece(LEFT);
		break;
	case KEY_RIGHT:
		game->movePiece(RIGHT);
		break;
	case KEY_DOWN:
		//TODO: continous down movement when key down held
		game->movePiece(DOWN);
		break;
	case KEY_UP:
		game->rotatePiece(CLOCKWISE);
		break;
	};

	return 0;
}
