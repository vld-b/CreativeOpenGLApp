#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	GLuint ID;
	VAO();

	void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComps, GLenum type, GLsizei stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif // !VAO_CLASS_H
