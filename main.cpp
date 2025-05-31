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

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using std::vector;

GLfloat verts[] = {
// Vertex Positions			Colors					Texture coords
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.f, 0.f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	1.f, 1.f
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
	Texture texture("583-1024x1024.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE, false);
	texture.texUnit(shader, "tex", 0);

	float rotation = 0.f;
	float startTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	// Main loop to poll events and
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen and apply new color
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program
		shader.Activate();

		float timeRN = glfwGetTime();
		if (timeRN - startTime > 1 / 60) {
			rotation += .5f;
			startTime = timeRN;
		}

		glm::mat4 model = glm::mat4(1.f);
		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 proj = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f));
		view = glm::translate(view, glm::vec3(0.f, -0.5f, -2.f));
		proj = glm::perspective(glm::radians(45.f), (float)(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.f);

		GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLuint viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLuint projLoc = glGetUniformLocation(shader.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


		// Set uniform scale variable
		glUniform1f(uniScale, 1.f);
		// Bind the Vertex Array Object so it can be worked with
		texture.Bind();
		vao.Bind();
		// Draw the vertices using triangle mode, with the given indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
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