#include "Game.h"
#include <cstdlib>
#include <ctime>

const int TICKS_PER_SECOND = 120;
const int LEVEL_MAX = 33;

const int numPieces = 7;

Game::Game(void)
{
	gameOver = false;
	level = 1;
	delay = 0;
	score = 0;
	curTick = 0;
	px = 3;
	py = 19;
	elapsedTime = 0.0f;
	prevTick = 0.0f;
	
	//nollst�ll spelplanen
	for(int i = 0; i < 20; i++)
		for(int j = 0; j < 10; j++)
          m_board[i][j] = 0;

	//flytta detta kanske ... st�ll in seed f�r pseudo-randomgenerator
	std::srand(std::time(0));
	piece = getPiece(std::rand() % numPieces);
	for(int i = 0; i < 2; i++)
		nextPiece[i] = getPiece(rand() % numPieces);
}


Game::~Game(void)
{

}

bool Game::movePiece(DIRECTION dir)
{
	if(delay)
		return false;
	int pxn = px, pyn = py;
	switch(dir)
	{
	case LEFT:
		pxn--;
		break;
	case RIGHT:
		pxn++;
		break;
	case DOWN:
		pyn--;
	}
	if (isPositionPossible(pxn,pyn))
	{
		px = pxn;
		py = pyn;
		return true;
	}
	return false;
}

bool Game::isPositionPossible(int x, int y)
{
	//kolla f�r varje delbit
	for(int i = 0; i < 4; i++)
	{
		//g�r om fr�n m�nsterkoordinater till v�rldskoordinater
		int xworld = piece->pattern[i][0]+x;
		int yworld = piece->pattern[i][1]+y;
		if(xworld < 0 || xworld > 9)
			return false;
		if(yworld < 0 || yworld > 19)
			return false;

		if(m_board[yworld][xworld] != 0)
			return false;
	}
	return true;
}

void Game::update(float dt)
{
	elapsedTime+= dt;

	int ticks = TICKS_PER_SECOND*(elapsedTime-prevTick);
	if (ticks > 10)
		ticks = 10;

	for ( ; ticks > 0; ticks--)
	{
		tick();
		prevTick = elapsedTime;
	}
}     

bool Game::isGameOver()
{
	return gameOver;
}

//updates gamestate
void Game::tick(void)
{
	if(gameOver)
		return;

	//delay anv�nds f�r animationsf�rdr�jning
	if(!delay)
		curTick++;
	else
	{
		if(delay == 1)
		{
			for(int row = 19; row >= 0; row--)
			{
				if(m_board[row][0] == 10)
					eliminateRow(row);
			}
		}
		delay--;
		return;
	}
	if(curTick % (103-level*3) == 0)
	{
		// If it's impossible to move piece down, add it to the gameboard and generate new piece
		if(!movePiece(DOWN))
		{
			putDownPiece();
		}
	}

	level = score / 250 + 1;
	if(level > LEVEL_MAX)
		level = LEVEL_MAX;

}

void Game::putDownPiece()
{
	for(int i = 0; i < 4; i++)
	{
		int xworld = piece->pattern[i][0]+px;
		int yworld = piece->pattern[i][1]+py;

      m_board[yworld][xworld] = piece->id;
	}
	int elrows = checkRows();

	// s�tt ig�ng delay f�r att blinka raderna som eliminerades
	if(elrows)
		delay = 60;

	//tmp score calc
	score += elrows*elrows*level*10 + 4;

	piece = nextPiece[0];
	nextPiece[0] = nextPiece[1];
	nextPiece[1] = getPiece( rand() % numPieces );

	//add new piece to board
	for(int i = 19; i>17; i--)
	{
		gameOver = true;
		if(isPositionPossible(3, i))
		{
			px = 3;
			py = i;
			gameOver = false;
			break;
		}
	}
}

//returnerar ett v�rde mellan 0 och 1 som indikerar hur l�ngt kvar biten har kommit innan den tickar ner
float Game::getPieceProgress()
{
	const int ticksPerDrop = (102-level*3);
	const int ticksLeft = ticksPerDrop - curTick % ticksPerDrop;

	float thisTickElapsed = (elapsedTime - prevTick) * (1.0f / TICKS_PER_SECOND ) ;

	float fTicksLeft = ((float) ticksLeft - thisTickElapsed);

	float progress = (fTicksLeft / ticksPerDrop );

	//Sanity checks
	if(progress > 1.0f)
		progress = 1.0f;
	if(progress < 0.0f)
		progress = 0.0f;

	return progress;
}


void Game::copyBoard(int copy[][10])
{
	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 10; j++)
			copy[i][j] = m_board[i][j];
	}
}

void Game::rotatePiece(ROTDIRECTION dir)
{
	if(delay)
		return;
	Piece* rotated;
	Piece* original = piece;
	switch(dir)
	{
	case CCLOCKWISE:
		rotated = piece->cclockwise;
		break;
	case CLOCKWISE:
		rotated = piece->clockwise;
		break;
	}
	bool tooLow = false;
	bool corrected = false;

	piece = rotated;
	if(!isPositionPossible(px, py))
	{
		//Try position to left and right also
		if(isPositionPossible(px+1, py))
			px++;
		else if(isPositionPossible(px-1, py))
			px--;
		else
			piece = original;
	}
}

//checkRows for full rows when piece is put down
int Game::checkRows()
{
	int elimRows = 0;
	for(int row = 19; row >= 0; row--)
	{
		bool fullRow = true;
		for(int col = 0; col < 10; col++)
		{
			if (m_board[row][col] == 0)
				fullRow = false;
		}
		if(fullRow)
		{
			tagRow(row);
			elimRows++;
		}
	}
	return elimRows;
}

//eliminate a row and move everything down
void Game::eliminateRow(int row)
{
	for(int r = row; r<19; r++)
		for(int c = 0; c<10; c++)
		{
          m_board[r][c] = m_board[r+1][c];
		}
	// Insert empty space at top row
	for(int c = 0; c < 10; c++)
      m_board[19][c] = 0;
}

//tags a row to be eliminated
void Game::tagRow(int row)
{
	for(int col = 0; col < 10; col++)
      m_board[row][col] = 10;
}