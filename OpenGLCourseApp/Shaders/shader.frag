#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 Normal;

out vec4 colour;

struct DirectionalLight
{	
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;

};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour,1.0f) * directionalLight.ambientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)),0.0f);  // a.b = cos(0)
	vec4 diffuseColor = vec4(directionalLight.colour,1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	colour = texture(theTexture,texCoord) * (ambientColor+diffuseColor) ;
}