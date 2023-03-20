#pragma once

#include  <GL/glew.h>
// download this library from here :https://github.com/nothings/stb/blob/master/stb_image.h
//this library supports different data types 
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void loadTexture();
	void useTexture();
	void clearTexture();
	~Texture();

private:
	GLuint textureID; 
	int width, height, bitDepth;
	char* fileLocation;

};
