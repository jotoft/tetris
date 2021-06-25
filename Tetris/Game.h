#pragma once
#include "pieces.h"

enum DIRECTION { LEFT, RIGHT, DOWN };
enum ROTDIRECTION { CLOCKWISE, CCLOCKWISE};

//Handles all game state
class Game
{
public:
	Game(void);
	~Game(void);
public:
	void tick(void);
	void update(float dt);
	bool movePiece(DIRECTION dir);
	void rotatePiece(ROTDIRECTION dir);
	float getPieceProgress(void);
	void getPiecePosition(int &row, int &col) {row = py; col = px;}
	Piece *getCurrentPiece() {return piece;}
	Piece *getNext() {return nextPiece[0];}
	int getScore() {return score;}
	int getLevel() {return level;}

	bool isGameOver();
	void copyBoard(int copy[][10]);

private:
	void eliminateRow(int row);
	void tagRow(int row);
	void putDownPiece();
	bool isPositionPossible(int x, int y);
	int checkRows();
	int score;
	int m_board[20][10];
	int rotation;
	Piece *piece;
	//upcoming pieces
	Piece *nextPiece[2];


	int px,py;
	int level;
	int delay;
	bool gameOver;

	float elapsedTime;
	float prevTick;
	long curTick;
};

