#include "processInput.h"

float deltaTime, lastTime = 0.0f;
float roll, yaw=270.0f, pitch;
extern short int w, h;
extern unsigned char draw_in_wireframe;
extern unsigned char current_model;
extern unsigned char nModels;
extern float fov;
extern mat4x4 view, projection, model;
extern float nearPlane, farPlane;
extern unsigned char draw_axis, draw_cube, draw_model, draw_light;
extern vec3 camPos;
extern vec3 camFront;
extern vec3 camRight;
extern float farPlane, nearPlane;

void processInput(GLFWwindow *window) {
	float currentTime = glfwGetTime();
	deltaTime = abs(currentTime - lastTime);
	lastTime = currentTime;
	keyboardInput(window);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	if (fov >= 1.0f && fov <= 90.0f) fov -= yoffset;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 90.0f) fov = 90.0f;
	std::cout << "fov: " << fov << "\n";
	projection = setProjMatrix((float)w, (float)h, nearPlane, farPlane, fov);
}


void keyboardInput(GLFWwindow *window) {
	static float cameraSpeedFront = 0.0f;
	static float cameraSpeedHorizontal = 0.0f;
	static unsigned char numbers[10] = { 0,0,0,0,0,0,0,0,0,0 };
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) numbers[1] = 1;
	else if (numbers[1]) { numbers[1] = 0; draw_in_wireframe^= 1; }

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) numbers[3] = 1;
	else if (numbers[3]) { numbers[3] = 0; draw_model ^= 1; }

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) numbers[4] = 1;
	else if (numbers[4]) { numbers[4] = 0; draw_cube ^= 1; }

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) numbers[5] = 1;
	else if (numbers[5]) { numbers[5] = 0; draw_axis ^= 1; }

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) numbers[6] = 1;
	else if (numbers[6]) { numbers[6] = 0; draw_light ^= 1; }

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) numbers[2] = 1;
	else if (numbers[2]) { numbers[2] = 0; current_model++; current_model %= nModels; }

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraSpeedFront -= 2.5f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraSpeedFront = 0.0f;
		else cameraSpeedFront += 2.5f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraSpeedHorizontal += -2.5f;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraSpeedHorizontal = 0.0f;
		else cameraSpeedHorizontal += 2.5f;


	if (cameraSpeedFront > 2.5f) cameraSpeedFront = 2.5f;
	else if (cameraSpeedFront < -2.5f) cameraSpeedFront = -2.5f;
	if (cameraSpeedHorizontal > 2.5f) cameraSpeedHorizontal = 2.5f;
	else if (cameraSpeedHorizontal < -2.5f) cameraSpeedHorizontal = -2.5f;
	
	camPos += camFront * cameraSpeedFront*deltaTime + camRight * cameraSpeedHorizontal*deltaTime;

	if (cameraSpeedFront) {
		if (cameraSpeedFront > 0.0f) cameraSpeedFront -= 0.05f;
		else if(cameraSpeedFront < 0.0f) cameraSpeedFront += 0.05f;

		if (abs(cameraSpeedFront - 0.0f) < 0.05f) cameraSpeedFront = 0.0f;
	}
	if (cameraSpeedHorizontal) {
		if (cameraSpeedHorizontal > 0.0f) cameraSpeedHorizontal -= 0.05f;
		else if (cameraSpeedHorizontal < 0.0f) cameraSpeedHorizontal += 0.05f;

		if (abs(cameraSpeedHorizontal - 0.0f) < 0.05f) cameraSpeedHorizontal = 0.0f;
	}
}

void mouseInput(GLFWwindow *window, double xpos, double ypos) {
	
	//static float lastX = (float)w / 2.0f;
	//static float lastY = (float)h / 2.0f;
	static float lastX, lastY;
	static unsigned char firstMouse = 1;
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (yaw >= 360.0f) yaw -= 360.0f;

	if (pitch > 89.9f) pitch = 89.9f;
	else if (pitch < -89.9f) pitch = -89.9f;

	camFront.e[0] = cos(yaw*(pi / 180.0f));
	camFront.e[1] = sin(pitch*(pi / 180.0f));
	camFront.e[2] = cos(pitch*(pi / 180.0f))*sin(yaw*(pi / 180.0f));
	camFront.make_unit_vector();
	
	camRight = cross(camFront,vec3(0.0f, 1.0f, 0.0f));
	camRight.make_unit_vector();

}

void framebuffer_callback(GLFWwindow *window, int w, int h) {
	glViewport(0, 0, w, h);
	projection = setProjMatrix((float)w, (float)h, nearPlane, farPlane, fov);
}