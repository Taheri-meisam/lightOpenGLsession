
#include <stdio.h>

#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "ALight.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture texture1;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
ALight dirLight;
// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

static const char* lightVshader = "Shaders/LightShader.vert";

// Fragment Shader
static const char* lightFshader = "Shaders/LightShader.frag";



void calAverageNormal(unsigned int* indices, unsigned int indicesCount, GLfloat* vertices, unsigned int verticesCount,
	unsigned int verticesLenght, unsigned int normalOffset) 
{
	for (size_t i = 0; i < indicesCount; i+=6)
	{
		unsigned int in0 = indices[i]   * verticesLenght;
		unsigned int in1 = indices[i+1] * verticesLenght;
		unsigned int in2 = indices[i+2] * verticesLenght;
		unsigned int in3 = indices[i+3] * verticesLenght;
		unsigned int in4 = indices[i+4] * verticesLenght;
		unsigned int in5 = indices[i+5] * verticesLenght;
		glm::vec3 v1(vertices[in4] - vertices[in0], vertices[in4 + 1] - vertices[in0 + 1], vertices[in4 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2); // corss product gets us the prop perpendicular direction line
		normal = glm::normalize(normal); // normalize the normal 

		// jump to normals 
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		in3 += normalOffset;
		in4 += normalOffset;
		in5 += normalOffset;

		//vertices 0
		vertices[in0] += normal.x;
		vertices[in0] += normal.y;
		vertices[in0] += normal.z;

		//vertices 1
		vertices[in1] += normal.x;
		vertices[in1] += normal.y;
		vertices[in1] += normal.z;

		//vertices 2
		vertices[in2] += normal.x;
		vertices[in2] += normal.y;
		vertices[in2] += normal.z;

		//vertices 3
		vertices[in3] += normal.x;
		vertices[in3] += normal.y;
		vertices[in3] += normal.z;

		//vertices 4
		vertices[in4] += normal.x;
		vertices[in4] += normal.y;
		vertices[in4] += normal.z;
		//vertices 5
		vertices[in5] += normal.x;
		vertices[in5] += normal.y;
		vertices[in5] += normal.z;
	}
	for (size_t i = 0; i < verticesCount / verticesLenght; i++)
	{
		unsigned int nOffset = i * verticesLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset] = vec.y;
		vertices[nOffset] = vec.z;

	}

}

void CreateObjects() 
{
	// cube indices // if you have triangle or other shapes this may be differnt 
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
		-1.0f,-1.0f, 0.0f,  0.0f,0.0f,
		0.0f, -1.0f, 1.0f,  0.5f, 0.0f,
		1.0f, -1.0f, 0.0f,  1.0f ,0.0f,
		0.0f,  1.0f, 0.0f,  0.5f, 1.0f
	};


	//model from blender comes with normals


	// Define cube vertices and texture coordinates
	// U & V are texture coordinates 
	GLfloat Cubevertices[] = {
		// Front face        //U  //V     //Nx Ny   Nz
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  0.0f,0.0f,0.f,// Bottom-left
		 0.5f, -0.5f, 0.5f,  1.0f, 0.0f,  0.0f,0.0f,0.f,/// Bottom-right
		 0.5f,  0.5f, 0.5f,  1.0f, 1.0f,  0.0f,0.0f,0.f,/// Top-right
		-0.5f,  0.5f, 0.5f,  0.0f, 1.0f,  0.0f,0.0f,0.f,/// Top-left
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,0.0f,0.f,/// Bottom-left
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,0.0f,0.f,/// Bottom-right
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,0.0f,0.f,/// Top-right
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,0.0f,0.f,/// Top-left
		// Left face
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,0.0f,0.f,/// Top-right
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,0.0f,0.f,/// Top-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f,0.0f,0.f,/// Bottom-left
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,0.0f,0.f,/// Bottom-right
		// Right face
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,0.0f,0.f, // Top-left
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f,0.0f,0.f,/// Top-right
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f,0.0f,0.f,/// Bottom-right
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f,0.0f,0.f,/// Bottom-left
		 // Top face
		 -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.f,/// Bottom-left
		  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,0.0f,0.f,/// Bottom-right
		  0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,0.0f,0.f,/// Top-right
		 -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  0.0f,0.0f,0.f,/// Top-left
			// Bottom face
		 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  0.0f,0.0f,0.f, // Top-left
		  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  0.0f,0.0f,0.f,/// Top-right
		  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,0.0f,0.f,/// Bottom-right
		 -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,0.f// Bottom-left
	};
	// creating an object of the mesh and passing in cube vertices and indices, number of elements have also been passed in 
	

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};

	//light cube dimentions 

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		1, 6, 7
	};


	calAverageNormal(indices, sizeof(indices) / sizeof(indices[0]), Cubevertices, sizeof(Cubevertices) / sizeof(Cubevertices[0]), 8, 5);


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(Cubevertices, indices, sizeof(Cubevertices)/sizeof(Cubevertices[0]), sizeof(indices)/ sizeof(indices[0]));
	meshList.push_back(obj1);
	Mesh* LightObject = new Mesh();
	LightObject->CreateMesh(lightVertices,lightIndices , sizeof(lightVertices) / sizeof(lightVertices[0]), sizeof(lightIndices) / sizeof(lightIndices[0]));
	meshList.push_back(LightObject);
	//for (int i = 0; i < sizeof(Cubevertices) / sizeof(Cubevertices[0]); i++) {
	//	std::cout << Cubevertices[i] << "|";
	//	if (i%7 == 0)
	//		std::cout << std::endl;
	//}
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
//	Shader* lightShader = new Shader();
 //	lightShader->CreateFromFiles(lightVshader, lightFshader);
	//shaderList.push_back(*lightShader);
}

int main() 
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	texture1 = Texture("Texture/wall.jpg");
	texture1.loadTexture();
	dirLight = ALight(1.0f, 1.0f, 1.0f, 0.2f,2.0f, -1.0f, -2.0f, 1.0f);





	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientColor = 0, uniformAmbientIntensity = 0, uniformDirection = 0,
		uniformDiffuseIntensity = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); 
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime); // using deltatime here 
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();


		dirLight.useLight(uniformAmbientIntensity, uniformAmbientColor,uniformDiffuseIntensity,uniformDirection);

		glm::mat4 model(1.0f);	

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}