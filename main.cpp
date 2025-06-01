#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using std::vector;

GLfloat verts[] = {
// Vertex Positions			Colors					Texture coords
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	1.f, 0.f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	1.f, 0.f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	.5f, 1.f
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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
	EBO ebo(indices, sizeof(indices));

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
	Texture texture("obama.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, false);
	texture.texUnit(shader, "tex", 0);

	Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3(0.f, 0.f, 2.f));

	// Enable the depth buffer
	glEnable(GL_DEPTH_TEST);

	// Main loop to poll events and
	while (!glfwWindowShouldClose(window)) {
		float frameStartTime = glfwGetTime(); // For calculating deltaTime
		// Clear the screen and apply new color
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		// Clear the necessary buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program
		shader.Activate();

		// Handle the camera
		camera.HandleInput(window);
		camera.Matrix(45.f, 0.1f, 100.f, shader, "camMatrix");

		// Bind the Vertex Array Object so it can be worked with
		texture.Bind();
		vao.Bind();
		// Draw the vertices using triangle mode, with the given indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
		// Swap the buffers to display creation
		glfwSwapBuffers(window);

		glfwPollEvents();

		float deltaTime = glfwGetTime() - frameStartTime; // Calculate delta time
		std::cout << "deltaTime: " << deltaTime << " seconds with FPS: " << 1/deltaTime << std::endl;
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