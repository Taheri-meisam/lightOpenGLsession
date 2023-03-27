#include "ALight.h"

ALight::ALight()
{
		
	color = glm::vec3(1.0f, 1.0f, 1.0f);  // the color internsity simply shows the amount of each color to be shown 
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // pointing down

	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

ALight::ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
}

ALight::ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity /*amibient internisty*/,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity /*diffuse internisty*/)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}
// ambient light uselight
void ALight::useLight(GLuint aLightIntensityLoc, GLfloat aLightColorLoc)
{
	glUniform3f(aLightColorLoc, color.x, color.y, color.z);
	glUniform1f(aLightIntensityLoc, ambientIntensity);
}

// diffuse light 

void ALight::useLight(GLuint aLightIntensityLoc, GLfloat aLightColorLoc, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	// binding the values 
	glUniform3f(aLightColorLoc, color.x, color.y, color.z);
	glUniform1f(aLightIntensityLoc, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);


}

ALight::~ALight()
{
}
