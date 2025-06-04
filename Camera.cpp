#include "Camera.h"

using glm::mat4;

Camera::Camera(short width, short height, vec3 position, GameManager* gameManager) {
	this->width = width;
	this->height = height;
	this->position = position;
	this->gameManager = gameManager;
	this->aspectRatio = (float)this->width / (float)this->height;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {

	// Initialize view and projection matrices
	mat4 view = mat4(1.f);
	mat4 proj = mat4(1.f);

	// Calculate the view matrix using the orientation and position
	view = glm::lookAt(position, position + orientation, up);
	// Calculate projection matrix using perspective projection
	proj = glm::perspective(glm::radians(FOVdeg), aspectRatio, nearPlane, farPlane);

	cameraMatrix = proj * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::HandleInput(GLFWwindow* window) {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// Calculate the rotation based on the mouse position. Center of the screen is (0|0)
	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

	// Rotate up and down
	vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	// Check if the new orientation is within a reasonable range to prevent flipping
	if (abs(glm::angle(newOrientation, up) - glm::radians(90.f)) < glm::radians(85.f)) {
		orientation = newOrientation;
	}

	// rotate around the up vector (left and right)
	orientation = glm::rotate(orientation, glm::radians(-rotY), up);

	glfwSetCursorPos(window, width / 2, height / 2);

	// Handle Player input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += speed * orientation * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= speed * glm::normalize(glm::cross(orientation, up)) * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= speed * orientation * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += speed * glm::normalize(glm::cross(orientation, up)) * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += speed * up * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position -= speed * up * (float)gameManager->deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 8.f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		speed = 4.f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}