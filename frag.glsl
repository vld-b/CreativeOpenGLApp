#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.2f;
	vec3 Normal = normalize(normal);
	vec3 lightDir = normalize(lightPos - currentPos);

	float diffuse = max(dot(normal, lightDir), 0.f);

	float specularStrength = .5f;
	vec3 viewDir = normalize(camPos - currentPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float specAmount = pow(max(dot(viewDir, reflectDir), 0.f), 8);
	float specular = specularStrength * specAmount;

	FragColor = texture(tex, texCoord) * lightColor * (diffuse + ambient + specular);
}