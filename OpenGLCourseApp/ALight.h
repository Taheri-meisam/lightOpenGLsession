#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class ALight
{
public:

	ALight();
	ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);
	ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void useLight(GLfloat aLightIntensityLoc, GLfloat aLightColorLoc);
	void useLight(GLfloat aLightIntensityLoc, GLfloat aLightColorLoc, GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	~ALight();
private:
	glm::vec3 color;
	GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
	glm::vec3 direction;


};

