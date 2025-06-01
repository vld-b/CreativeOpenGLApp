#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

// single input matrix from the camera
uniform mat4 camMatrix;

void main()
{
	// get the final position of the vertex
	gl_Position = camMatrix * vec4(aPos, 1.f);
	color = aColor;
	texCoord = aTex;
}