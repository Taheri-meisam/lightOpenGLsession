#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm; // normal of vertex  

out vec4 vCol;
out vec2 texCoord;
flat out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	texCoord = tex;
	Normal = mat3(transpose(inverse(model))) * norm;  // Normal needs to move with the model. also should move with rotation and scaling
	// mat3 -> convert to vec3 // inverse then transpose -> fixes the problem with scaling and rotation  
}