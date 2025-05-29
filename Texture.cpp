#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool useAlpha) {
	// Set the texture type and load the image
	type = texType;
	int imgWidth, imgHeight, colChannels;
	unsigned char* bytes = stbi_load(image, &imgWidth, &imgHeight, &colChannels, 0);

	// Set the internal format to be used
	GLuint intFormat = useAlpha ? GL_RGBA : GL_RGB;

	// Generate texture, bind it and assign it to its slot
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(type, ID);

	// Set the texture scaling and repeat parameters
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate 2D image you and its mipmaps
	glTexImage2D(type, 0, intFormat, imgWidth, imgHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(type);

	// Free memory and unbind the texture
	stbi_image_free(bytes);
	glBindTexture(type, 0);
}

void Texture::texUnit(Shader &shader, const char* uniform, GLuint unit) {
	GLuint uniTex = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(uniTex, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}