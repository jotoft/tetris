#include "loadTexture.h"
#include <SOIL.h>
#include <string>


//load texture using SOIL
GLuint loadTextureFromFile(const char *path, GLenum texunit) 
{
	GLuint texture;
	int width, height;
	unsigned char * image;

	glGenTextures(1, &texture);

	glActiveTexture( texunit );
	glBindTexture( GL_TEXTURE_2D, texture );

	image = SOIL_load_image( path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
              GL_UNSIGNED_BYTE, image );

	SOIL_free_image_data( image );

	return texture;
}

