#include "Graphics.h"
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <fstream>
#include "Game.h"

#include <math.h>

static const int XSIZE = 800;
static const int YSIZE = 600;


Graphics::Graphics(Game *game)
{
	theGame = game;
	time = 0.0f;
}

//initialize opengl stuff
bool Graphics::init()
{
	time = 0.0f;
    glClearColor(0.5f, 0.8f, 0.5f, 0.5f);

	//glViewport(0, 0, XSIZE, YSIZE);
	glGenVertexArrays(3, vao );
	glBindVertexArray( vao[0] );

	vertexShader = loadShader("vertex.glsl", GL_VERTEX_SHADER );
	fragmentShader = loadShader("frag.glsl", GL_FRAGMENT_SHADER );
	

	GLenum test = glGetError();

	glGenBuffers(3, vbo);

	shaderProgram = glCreateProgram();
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glBindFragDataLocation( shaderProgram, 0, "outColor" );

	glLinkProgram( shaderProgram );
	glUseProgram( shaderProgram );
	test = glGetError();
	texture = loadTextureFromFile("block2.png", GL_TEXTURE0);
	fontTexture = loadTextureFromFile("font.jpg", GL_TEXTURE1);
	glActiveTexture(GL_TEXTURE0);

	for(int i = 0; i < 3; i++)
	{
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER,vbo[i]);
		GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
		glEnableVertexAttribArray( posAttrib );
		glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof( float ), 0 );
		GLint texAttrib = glGetAttribLocation( shaderProgram, "texcoord" );
		glEnableVertexAttribArray( texAttrib );
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof( float ), (void*) (2*sizeof(float) )); 
		GLint colAttrib = glGetAttribLocation( shaderProgram, "color" );
		glEnableVertexAttribArray( colAttrib );
		glVertexAttribPointer( colAttrib, 3, GL_FLOAT, GL_FALSE, 7*sizeof( float ), (void*) ( 4*sizeof( float ) ) );
	}
	glUniform1i( glGetUniformLocation( shaderProgram, "tex" ), 0 );
	test = glGetError();
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

	glActiveTexture(GL_TEXTURE1);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


     //Return success
     return true;
}

GLuint Graphics::loadShader(const char *fileName, GLenum shadertype)
{
	GLuint shader;

	shader = glCreateShader(shadertype);

	std::stringstream buffer;
	std::ifstream file;
	file.open(fileName);
	buffer << file.rdbuf();
	file.close();
	
	std::string contents = buffer.str();
	const char *source = contents.c_str();
	
	GLint result;

	glShaderSource( shader, 1, &source, NULL );
	glCompileShader( shader );

	return shader;
}

const int MAXCHARS = 200;
const int VERTEXSIZE = 7;
const int QUADSIZE = VERTEXSIZE*6;
//Set to 1 for smoother tetromino descent, doesn't work at the moment
const float SMOOTH_MOTION = 0.0f;

void Graphics::prepare(float dt)
{

	time+=dt;
	char strscore[100];
	char strlevel[100];
	int board[20][10];
	float tetromino[QUADSIZE*4*2];
	float text[MAXCHARS*QUADSIZE];
	float *vertices = new float[20*10*QUADSIZE];

	int score = theGame->getScore();
	int level = theGame->getLevel();
	theGame->copyBoard(board);
	//int to string
	itoa(score, strscore, 10);
	itoa(level, strlevel, 10);

	//Calculate vertices for game board
	for(int row = 0, quad = 0; row < 20; row++)
		for(int col = 0; col < 10; col++, quad++)
				calcQuad(row, col, board[row][col], &vertices[quad*7*6]);

	//Calculate vertices for onscreen text
	calcText(strscore, 0.5f, 0.25f, text);
	float *textBufPos = &text[((strlen(strscore))*QUADSIZE)];
	calcText("Score", 0.4f, 0.35f, textBufPos);
	textBufPos+=(5*QUADSIZE);
	calcText("Level", 0.4f, 0.05f, textBufPos);
	textBufPos+=(5*QUADSIZE);
	calcText(strlevel, 0.5f, -0.05f, textBufPos);


	Piece *test = theGame->getCurrentPiece();
	Piece *next = theGame->getNext();

	//Calculate vertices for current piece
	for(int i = 0; i < 4; i++)
	{
		int row, col;
		theGame->getPiecePosition(row, col);
		int x = test->pattern[i][0];
		int y = test->pattern[i][1];
		row+=y;
		col+=x;
		float offsetX = 0.0f;
		float offsetY = 0.1*theGame->getPieceProgress()*SMOOTH_MOTION;
		calcQuad(row,col,offsetX, offsetY, test->id, &tetromino[(7*6*i)]);
	}

	//Calculate vertices for next piece
	for(int i = 0; i <4; i++)
	{
		int row = 17, col = 13;
		int x = next->pattern[i][0];
		int y = next->pattern[i][1];

		row+=y;
		col+=x;
		calcQuad(row, col, 0.0f, 0.0f, next->id, &tetromino[168+7*6*i]);
	}


	//Transfer vertices to GPU memory
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	bufsize[0] = sizeof(float)*(1200*7);
	glBufferData(GL_ARRAY_BUFFER, bufsize[0] , vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	bufsize[1] = sizeof(tetromino);
	glBufferData(GL_ARRAY_BUFFER, bufsize[1], tetromino, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	bufsize[2] = sizeof(float)*(strlen(strscore)+6+5+strlen(strlevel))*QUADSIZE;
	glBufferData(GL_ARRAY_BUFFER, bufsize[2], text, GL_DYNAMIC_DRAW);
	

	delete[] vertices;

}

void Graphics::render()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT);

	//Rita ut spelplanen
	glBindVertexArray(vao[0]);
	glUniform1i( glGetUniformLocation( shaderProgram, "tex" ), 0 );
	glDrawArrays(GL_TRIANGLES, 0, bufsize[0]);

	//Rita ut nuvuarande bit och nästa bit
	glBindVertexArray(vao[1]);
	glUniform1i( glGetUniformLocation( shaderProgram, "tex" ), 0 );
	glDrawArrays(GL_TRIANGLES, 0, bufsize[1]);
	
	//Rita ut text
	glBindVertexArray(vao[2]);
	//Text-texturen ligger i texturenhet 1
	glUniform1i( glGetUniformLocation( shaderProgram, "tex" ), 1 );
	glDrawArrays(GL_TRIANGLES, 0, bufsize[2]);
}

void Graphics::shutdown()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(3, vao);
	glDeleteBuffers(3, vbo);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Graphics::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

 //   gluPerspective(45.0f, float(width) / float(height), 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//Size of Tetromino block sides
const float sideX = ((float)YSIZE/(float)XSIZE)*0.1;
const float sideY = 0.1f;

//caclulate quad vertices from x and y position
void Graphics::calcQuad(float x, float y, int type, float *v)
{
	if ((type > 7 || type < 0) && type != 10)
		type = 0;
	if (type == 10)
		type = 8;

	const float color[] = {
		0.2f, 0.2f, 0.2f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.3f, 0.3f, 1.0f,
		0.9f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
	    0.9f, 0.1f, 1.1f,
		0.5f, 0.5f, 0.8f,
		sinf(time*34.0f), sinf(time*22.0f), sinf(time*46.0f)
		};



	//Offset i x och yled för att flytta alla punkter i x- och y-led
	const float tX = -0.5f;
	const float tY = -1.0f;

	float left = x +tX;
	float right = x+sideX +tX;
	float top = y+sideY + tY;
	float bottom = y + tY;

	float coords[] = {
		//triangle 1
		left, top,
		right, top,
		left, bottom,
		//triangle 2
		right, top,
		right, bottom,
		left, bottom
	};

	float texCoords[] = {
		0.0f, 0.0f, //Top left
		1.0f, 0.0f, //Top right
		0.0f, 1.0f, // Bottom-left
		1.0f, 0.0f, //Top right
		1.0f, 1.0f, //Bottom-right
		0.0f, 1.0f //Bottom-left
	};

	for(int vert = 0; vert < 6; vert++)
	{
		v[0] = coords[vert*2];
		v[1] = coords[vert*2+1];
		v[2] = texCoords[vert*2];
		v[3] = texCoords[vert*2+1];
		memcpy(&v[4], &color[type*3], sizeof(float)*3);
		v+=7;
	}
}

void Graphics::calcQuad(int row, int col, int type, float *v)
{

	float x = col*sideX;
	float y = row*sideY;
	calcQuad(x,y,type,v);
}

void Graphics::calcQuad(int row, int col, float offsetX, float offsetY, int type, float *v)
{
	float x = col*sideX+offsetX;
	float y = row*sideY+offsetY;
	calcQuad(x,y,type,v);
}

void Graphics::calcText(const char *text, float x, float y, float *result)
{
	int length = strlen(text);

	float color[] = {1.0f, 1.0f, 0.4f};

	const float strideX = 0.020*3;
	const float sizeX = 0.020*3;
	const float sizeY = 0.025*3;

	const float oneOverSixteen = 1.0f / 16.0f;

	float texCoords[6*2];

	for(int i = 0; i < length; i++)
	{
		float xPos = float(text[i] % 16) * oneOverSixteen;
		float yPos = float(text[i] / 16) * oneOverSixteen;

		//top left
		texCoords[0] = xPos;
		texCoords[1] = yPos;

		//top right
		texCoords[2] = xPos + oneOverSixteen;
		texCoords[3] = yPos;

		//bottom left
		texCoords[4] = xPos;
		texCoords[5] = (yPos +oneOverSixteen);

		//top right
		texCoords[6] = xPos + oneOverSixteen;
		texCoords[7] =  yPos;

		//bottom right
		texCoords[8] = xPos + oneOverSixteen;
		texCoords[9] = ( yPos + oneOverSixteen);

		//bottom left
		texCoords[10] = xPos;
		texCoords[11] = (yPos +oneOverSixteen);

		float pos[] = 
		{
			x, y, //top-left
			x+sizeX, y, //top-right
			x, y-sizeY, //bottom left
			x+sizeX, y, //top-right
			x+sizeX, y-sizeY, //bottom-right
			x, y-sizeY, //bottom-left
		};

		x+=strideX;

		for(int j = 0; j < 6; j++)
		{
			result[0] = pos[j*2];
			result[1] = pos[j*2+1];
			result[2] = texCoords[j*2];
			result[3] = texCoords[j*2+1];
			result[4] = color[0];
			result[5] = color[1];
			result[6] = color[2];
			result+=7;
		}

	}
	
	
}