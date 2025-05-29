#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

// Uniform (available from anywhere) scale variable
uniform float scale;

uniform mat4 model, view, proj;

void main()
{
	// Scale the vertex position by the uniform scale factor
	gl_Position = proj * view * model * vec4(aPos * scale, 1.f);
	color = aColor;
	texCoord = aTex;
}