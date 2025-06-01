#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

using glm::vec3;

class Camera {
public:
	// Basic camera properties
	vec3 position;
	vec3 orientation = vec3(0.f, 0.f, -1.f);
	vec3 up = vec3(0.f, 1.f, 0.f);

	// Stores whether the mouse has clicked on frame so that the camera doesn't jump around
	bool firstClick = true;

	short width, height;

	float speed = .1f;
	float sensitivity = 100.f;

	Camera(short width, short height, vec3 position);

	// Calculates the Camera's matrix in order to render the scene
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	// Handle player input
	void HandleInput(GLFWwindow* window);
};

#endif // !CAMERA_CLASS_H
