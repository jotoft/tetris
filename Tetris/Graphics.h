#pragma once
#include <GL/glew.h>
#include "Game.h"
#include "loadTexture.h"


class Graphics
{
public:
	Graphics(Game *game);
	bool init();
	void prepare(float dt);
	void render();
	void shutdown();
	void onResize(int width, int height);
	static GLuint loadShader(const char *filename, GLuint shadertype);

private: 		
	void calcQuad(int row, int col, int type, float *v);
	void calcQuad(int row, int col, float offsetX, float offsetY, int type, float *v);
	void calcQuad(float x, float y, int type, float *v);
	void calcText(const char *text, float x, float y, float *result);

	GLuint loadVertexShader(const char *filename);
	GLuint loadFragmentShader(const char *filename);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint vao[3];
	GLuint vbo[3];
	GLuint shaderProgram;
	GLuint texture;
	GLuint fontTexture;
	float time;
	int bufsize[3];

	Game *theGame;
};