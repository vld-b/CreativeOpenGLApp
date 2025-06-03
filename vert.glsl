#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;

out vec3 normal;
out vec3 currentPos;

// single input matrix from the camera
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	currentPos = vec3(model * vec4(aPos, 1.f));

	// get the final position of the vertex
	gl_Position = camMatrix * vec4(currentPos, 1.f);
	color = aColor;
	texCoord = aTex;
	normal = aNormal;
}