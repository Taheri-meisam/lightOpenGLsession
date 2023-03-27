#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class ALight
{
public:

	ALight();
	ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
	ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, /*diffuse values*/ GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void useLight(GLuint aLightIntensityLoc, GLfloat aLightColorLoc);
	void useLight(GLuint aLightIntensityLoc, GLfloat aLightColorLoc, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~ALight();
private:
	glm::vec3 color; // alternatively you can use    gsl::Vector3D color;
	GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
	glm::vec3 direction; // diffuse light needs direction 


};

