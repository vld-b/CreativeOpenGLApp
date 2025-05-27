#include "ShaderClass.h"

std::string getFileContents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	std::string vertexString = getFileContents(vertexShaderFile);
	std::string fragmentString = getFileContents(fragmentShaderFile);

	const char* vertexSource = vertexString.c_str();
	const char* fragmentSource = fragmentString.c_str();

	// Create vertex shader and compile the source
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create fragment shader and compile the source
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create a Program and attach the vertex and fragment shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// Wrap up the program by linking it
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Delete the shaders because they are already in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::compileErrors(GLuint shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (!hasCompiled) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR COMPILING " << type << " SHADER!" << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (!hasCompiled) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR LINKING PROGRAM FOR TYPE: " << type << std::endl;
			std::cout << infoLog << std::endl;
		}
	}
}

void Shader::Activate() {
	// Use the shader program
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}