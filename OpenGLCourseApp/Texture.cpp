#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture()
{
	textureID = 0;    // texture ID 
	width = 0;         // width and height of texture 
	height = 0;
	bitDepth = 0;    // pixel size 
	fileLocation = " "; // where the file is located in your project folder // example ::   /texture/wood.jpg
}
// pass in the address and filename of your image( jpp, png or other formats) when you create an object of texture class
Texture::Texture(char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

Texture::~Texture()
{
	clearTexture();
}

void Texture::loadTexture()
{
	unsigned char* texdata = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID); // we bind the texture here 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //S Axis // we want to repeat the texture to fill the mesh 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T Axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR); // we apply filters for min and mag as a linear 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0); // we unbind the texture 

	if (texdata)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texdata);
    glGenerateMipmap(GL_TEXTURE_2D);
}
	else
	{
		printf("Failed to load texture \n");
	}
//	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texdata);
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0); // which texture unit to make active in GPU // there are 16-32 units //depends on GPUs this can vary 
	glBindTexture(GL_TEXTURE_2D, textureID); // we bind out texture to a 2D texture 
}

// reset everything 
void Texture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";

}
