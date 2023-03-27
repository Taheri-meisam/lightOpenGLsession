#version 330

in vec4 vCol;
in vec2 texCoord;
flat in vec3 Normal; // capture normal 

out vec4 colour;

struct DirectionalLight
{	
	vec3 colour;
	float ambientIntensity;
	vec3 direction;  // direction of the light 
	float diffuseIntensity;

};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.colour,1.0f) * directionalLight.ambientIntensity; //
	
	// diffuse lighting 
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)),0.0f); 
	 // a.b = cos(0)  // nomalize converts it to unit vector (lenght = 1) // if the out put is -0 then max makes sures it is not going to be a negative value
	// a.b = cos(0) = 1 -> full light 
	vec4 diffuseColor = vec4(directionalLight.colour,1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	colour = texture(theTexture,texCoord) * (ambientColor+diffuseColor) ; // ambient light 
	//colour = texture(theTexture,texCoord) * (ambientColor+diffuseColor) ;
}