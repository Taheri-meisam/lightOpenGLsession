where to download glm : https://github.com/g-truc/glm/releases/tag/0.9.9.8


where to download stb_image.h https://github.com/nothings/stb/blob/master/stb_image.h


if you want to use visual studio you need to add lib,dll and header files in linker section 


we need to include the following libraries :

#include <stdio.h>

#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


I assume at this point of the course you already have your shader and mesh created. 

so we start by creating a new class called Texture

in the header file of the Texture class we need to have these functions and variables  : 

#pragma once

#include  <GL/glew.h>
// download this library from here :https://github.com/nothings/stb/blob/master/stb_image.h
//this library supports different data types 
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc); // getting the address and name of the file in constructor 

	void loadTexture(); 
	void useTexture();
	void clearTexture();
	~Texture();

private:
	GLuint textureID;    // each texture needs to have an ID
	int width, height, bitDepth; //size of the texture and pixels (texel)
	char* fileLocation;

};



in the the Texture.cpp file : 


#define STB_IMAGE_IMPLEMENTATION   // stb_image macro in order to use this library you need to define it 
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
	glGenTextures(1, &textureID); // here when texture is generated an ID is assigned to it 
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
		printf("Failed to load texture \n");    // we use printf which is C programming language way of std::cout, C is more portable 
		// you can use std::cout or perror as well
	}
	stbi_image_free(texdata);
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0); // which texture unit to make active in GPU // there are 16-32 units //depends on GPUs this can vary 
	glBindTexture(GL_TEXTURE_2D, textureID); // we bind out texture to a 2D texture // here we use that texture ID to bind the texture 
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

ok we are done creating the texture class. now we need to update our Fragment shader and vertex shader

here is the shader Fragment that it is saved as file on my pc and I included this file in project folder I am working on 



and here is vertex shader : 

#version 330

layout (location = 0) in vec3 pos;  // 3D object 
layout (location = 1) in vec2 tex;  // 2D texture 

out vec4 vCol;  // color out 
out vec2 texCoord;  // texture coordinates out 

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	texCoord = tex;   //
}


here is the shader Fragment : 
//shader Fragment
#version 330

in vec2 texCoord;  

out vec4 colour;
uniform sampler2D theTexture;

void main()
{
	colour = texture(theTexture,texCoord);  // pass in theTexture and texCoord to texture property 
}



/////////


now we need to add glVertexAttribPointer where you created your mesh : 

glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5 /* start from 3rd element and every 5 times forward */, (void*)(sizeof(vertices[0]) * 3)/*where the U&V statrs in Matrix*/); // 
glEnableVertexAttribArray(1);



void glVertexAttribPointer(	GLuint index,
 		GLint size,
 		GLenum type,
 		GLboolean normalized,
 		GLsizei stride,
 		const void * pointer);

Parameters
index
Specifies the index of the generic vertex attribute to be modified.

size
Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. 
Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.

type
Specifies the data type of each component in the array.
 The symbolic constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. 
 Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only token accepted by the type parameter for that function. The initial value is GL_FLOAT.

normalized
For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or 
converted directly as fixed-point values (GL_FALSE) when they are accessed.

stride
Specifies the byte offset between consecutive generic vertex attributes. 
If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.

pointer
Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.



in the main.cpp  : 

Texture texture1Melon; // create an instance/object of the Texture class 

GLfloat deltaTime = 0.0f;   // you can use this method to slow down the frames // it is shown how to use it in while loop 
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() 
{
	// cube indices // if you have triangle or other shapes this may be differ 
	GLuint indices[] = {
			0, 1, 2, 2, 3, 0,       // Front face
			4, 5, 6, 6, 7, 4,       // Back face
			8, 9, 10, 10, 11, 8,    // Left face
			12, 13, 14, 14, 15, 12, // Right face
			16, 17, 18, 18, 19, 16, // Top face
			20, 21, 22, 22, 23, 20  // Bottom face
	};

	// example of trangle or pyramid vertices 
	GLfloat vertices[] = {
	   -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		0.0f,  -1.0f, 1.0f,  0.5f, 0.0f,
		1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
		0.0f,   1.0f, 0.0f,  0.5f, 1.0f
	};

	// Define cube vertices and texture coordinates
	// U & V are texture coordinates 
	GLfloat Cubevertices[] = {
		// Front face        //U  //V
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, 0.5f,  1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f, 0.5f,  1.0f, 1.0f, // Top-right
		-0.5f,  0.5f, 0.5f,  0.0f, 1.0f, // Top-left
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // Top-right
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // Top-left
		// Left face
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // Top-right
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // Top-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // Bottom-left
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // Bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, // Bottom-left
		 // Top face
		 -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // Bottom-left
		  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // Bottom-right
		  0.5f,  0.5f, -0.5f, 1.0f, 0.0f, // Top-right
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // Top-left
			// Bottom face
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Top-left
		  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // Top-right
		  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // Bottom-right
		 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f // Bottom-left
	};
	// creating an object of the mesh and passing in cube vertices and indices, number of elements have also been passed in 
	
	Mesh *obj1 = new Mesh();
	//CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
	obj1->CreateMesh(Cubevertices, indices, 120, 36);  //120 number of vertices and 36 is number of indices 
	//alternatively you can write this way: 	obj1->CreateMesh(Cubevertices, indices, sizeof(Cubevertices)/sizeof(Cubevertices[0]), sizeof(indices)/ sizeof(indices[0]));
	meshList.push_back(obj1); // mesh list is a vector of mesh class that you can create meshes and add them to the vector 

}

before we enter the while loop in main function : 
	//create and load the texture 
	texture1Melon = Texture("Texture/1.jpg");
	texture1Melon.loadTexture();


using deltaTime in your code : 
		GLfloat now = glfwGetTime(); 
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime); // using deltatime here 
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());