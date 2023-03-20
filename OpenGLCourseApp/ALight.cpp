#include "ALight.h"

ALight::ALight()
{
		
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

ALight::ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
}

ALight::ALight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void ALight::useLight(GLfloat aLightIntensityLoc, GLfloat aLightColorLoc)
{
	glUniform3f(aLightColorLoc, color.x, color.y, color.z);
	glUniform1f(aLightIntensityLoc, ambientIntensity);
}

void ALight::useLight(GLfloat aLightIntensityLoc, GLfloat aLightColorLoc, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(aLightColorLoc, color.x, color.y, color.z);
	glUniform1f(aLightIntensityLoc, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);


}

ALight::~ALight()
{
}
