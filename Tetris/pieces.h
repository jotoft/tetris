#pragma once

enum PIECES { SQUAREBLOCK = 1, LBLOCK, RLBLOCK, LINEBLOCK, TBLOCK, SBLOCK, RSBLOCK };

struct Piece
{
	int id;
	int pattern[4][2];
	Piece *clockwise;
	Piece *cclockwise;
};

Piece *getPiece(int piece);

