#include "pieces.h"

Piece square = 	{ SQUAREBLOCK, 
	{ 
		1, 0,
		2, 0,
		1, -1,
		2, -1, }, &square, &square };

Piece lblock[4] = 
{ 
	{
		LBLOCK,
		{ 
			0, 0,
			1, 0,
			2, 0,
			0, -1
		},
		&lblock[1],
		&lblock[3]
	},

	{
		LBLOCK,
		{ 
			0, 1,
			1, 1,
			1, 0,
			1, -1
		},
		&lblock[2],
		&lblock[0]
	},

	{
		LBLOCK,
		{
			0, -1,
			1, -1,
			2, -1,
			2, 0
		},
		&lblock[3],
		&lblock[1]
	},

	{
		LBLOCK,
		{
			1, 1,
			1, 0,
			1, -1,
			2, -1
		},
		&lblock[0],
		&lblock[2]
	}
};

Piece tblock[4] = 
{
	{
		TBLOCK,
		{
			0, 0,
			1, 0,
			2, 0,
			1, -1
		},
		&tblock[1],
		&tblock[3]
	},
	{
		TBLOCK,
		{
			0, 0,
			1, 0,
			1, 1,
			1, -1
		},
		&tblock[2],
		&tblock[0]
	},
	{
		TBLOCK,
		{
			1, 0,
			0,-1,
			1,-1,
			2,-1
		},
		&tblock[3],
		&tblock[1]
	},
	{
		TBLOCK,
		{
			1, 1,
			1, 0,
			1, -1,
			2, 0
		},
		&tblock[0],
		&tblock[2]
	}
};

Piece rlblock[4] =
{
	{
		RLBLOCK,
		{
			0, 0,
			1, 0,
			2, 0,
			2, -1
		},
		&rlblock[1],
		&rlblock[3]
	},
	{
		RLBLOCK,
		{
			1, 1,
			1, 0,
			0, -1,
			1, -1
		},
		&rlblock[2],
		&rlblock[0]
	},
	{
		RLBLOCK,
		{
			0, 0,
			0, -1,
			1, -1,
			2, -1
		},
		&rlblock[3],
		&rlblock[1]
	},
	{
		RLBLOCK,
		{
			1, 1,
			2, 1,
			1, 0,
			1, -1
		},
		&rlblock[0],
		&rlblock[2]
	}
};

Piece sblock[2] =
{
	{
		SBLOCK,
		{
			1, 0,
			2, 0,
			0, -1,
			1, -1
		},
		&sblock[1],
		&sblock[1]
	},
	{
		SBLOCK,
		{
			0, 1,
			0, 0,
			1, 0,
			1, -1
		},
		&sblock[0],
		&sblock[0]
	}
};

Piece rsblock[2] = 
{
	{
		RSBLOCK,
		{
			0, 0,
			1, 0,
			1, -1,
			2, -1
		},
		&rsblock[1],
		&rsblock[1]
	},
	{
		RSBLOCK,
		{
			1, 0,
			2, 0,
			2, 1,
			1, -1
		},
		&rsblock[0],
		&rsblock[0]
	}
};

Piece lineBlock[2] =
{
	{
		LINEBLOCK,
		{
			0, 0,
			1, 0,
			2, 0,
			3, 0
		},
		&lineBlock[1],
		&lineBlock[1]
	},
	{
		LINEBLOCK,
		{
			2, 1,
			2, 0,
			2, -1,
			2, -2
		},
		&lineBlock[0],
		&lineBlock[0]
	}
};

Piece *getPiece(int piece)
{
	switch ( piece +1 )
	{
	case SQUAREBLOCK:
		return &square;
		break;
	case LBLOCK:
		return &lblock[0];
		break;
	case RLBLOCK:
		return &rlblock[0];
		break;
	case TBLOCK:
		return &tblock[0];
		break;
	case LINEBLOCK:
		return &lineBlock[0];
		break;
	case RSBLOCK:
		return &rsblock[0];
		break;
	case SBLOCK:
		return &sblock[0];
		break;

	}
	return nullptr;
}