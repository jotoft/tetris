#pragma once
#include <GL/glew.h>
#include "Game.h"
#include "loadTexture.h"
#include <cstdint>

class Graphics
{
public:
	Graphics(Game *game);
	bool init();
	void prepare(float dt);
	void render();
	void shutdown();
	void onResize(int width, int height);

private: 		
	void calcQuad(int row, int col, int type, float *v) const;
	void calcQuad(int row, int col, float offsetX, float offsetY, int type, float *v) const;
	void calcQuad(float x, float y, int type, float *v) const;
	static void calcText(const char *text, float x, float y, float *result);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint m_vaos[3];
	GLuint vbo[3];
	GLuint shaderProgram;
	GLuint m_blocktexture;
	GLuint m_fonttexture;
	float m_time;
	GLsizei m_bufsize[3];

	Game *m_game;
};