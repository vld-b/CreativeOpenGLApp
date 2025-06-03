#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex;

uniform vec4 lightColor;
uniform vec3 lightPos;

void main()
{
	float ambient = 0.2f;
	vec3 Normal = normalize(normal);
	vec3 lightDir = normalize(lightPos - currentPos);

	float diffuse = max(dot(normal, lightDir), 0.f);

	FragColor = texture(tex, texCoord) * lightColor * (diffuse + ambient);
}