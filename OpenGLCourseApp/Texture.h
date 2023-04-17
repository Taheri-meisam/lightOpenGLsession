#pragma once

#include  <GL/glew.h>
// download this library from here :https://github.com/nothings/stb/blob/master/stb_image.h
//this library supports different data types 
#include "stb_image.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void loadTexture();
	bool LoadTextureM();
	bool LoadTextureA();
	void useTexture();
	void clearTexture();
	~Texture();

private:
	GLuint textureID; 
	int width, height, bitDepth;
	const char* fileLocation;

};
