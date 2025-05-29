#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <vector>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#ifdef _DEBUG
#define GL_DEBUG(stmt) do { \
stmt; \
CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)

#else
#define GL_DEBUG(stmt) do {stmt; }

#endif

using std::vector;

GLfloat verts[] = {
	-0.5f, -0.5f, 0.0f, 0.3f, 0.4f, 0.7f, 0.f, 0.f,
	-0.5f, 0.5f, 0.0f, 0.8f, 0.2f, 0.4f, 0.f, 1.f,
	0.5f, 0.5f, 0.0f, 0.8f, 0.5f, 0.5f, 1.f, 1.f,
	0.5f, -0.5f, 0.0f, 0.3f, 0.4f, 0.9f, 1.f, 0.f
};

vector<GLuint> indices = {
	0, 2, 1,
	0, 3, 2
};

int main() {
	// Initialize GLFW
	glfwInit();

	// Configure GLFW Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	// Create GLFW Window and add to current context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Goodbye OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Display the window by making it the current context
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers using GLAD
	gladLoadGL();

	// Set OpenGL Viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create the shader program and compile the shaders
	Shader shader("vert.glsl", "frag.glsl");

	// Create the VAO and bind it
	VAO vao;
	vao.Bind();

	// Create the VBO and EBO
	VBO vbo(verts, sizeof(verts));
	EBO ebo(indices.data(), sizeof(indices));

	// Link the VAO to the VBO, first the vertex positions and then the colors
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	// Get the uniform location for the scale variable
	GLuint uniScale = glGetUniformLocation(shader.ID, "scale");

	// Create texture and assign it to the uniform
	stbi_set_flip_vertically_on_load(true); // Flip the texture vertically
	Texture texture("583-1024x1024.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, false);
	texture.texUnit(shader, "tex", 0);

	// Main loop to poll events and
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen and apply new color
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		texture.Bind();
		// Use the shader program
		shader.Activate();
		// Set uniform scale variable
		glUniform1f(uniScale, 1.0f);
		// Bind the Vertex Array Object so it can be worked with
		vao.Bind();
		// Draw the vertices using triangle mode, with the given indices
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		// Swap the buffers to display creation
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Cleanup
	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	texture.Delete();
	shader.Delete();

	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void CheckOpenGLError(const char* stmt, const char* fname, int line) {
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "OpenGL error %08x, at %s:%i - for %s\n" << err << fname << line << stmt << std::endl;
	}
}