#include "TextureFromFile.h"
#include "Editor.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <Windows.h>
#include "processInput.h"
#include "Projection.h"
#include "mesh.h"
#include "model.h"
#include "vertexdata.h"

unsigned char current_model = 0;
const unsigned char nModelsCount = 3;
unsigned char nModels = nModelsCount;
unsigned char draw_in_wireframe = 0;
unsigned char draw_axis = 1, draw_cube = 1, draw_model = 1, draw_light = 1;
short int w = 1920, h = 1080;

mat4x4 view, projection, model;
float fov=90.0f;
float nearPlane = 0.1f, farPlane = 20.0f;

vec3 topCol(0.3f, 0.3f, 0.31f), botCol(0.1f, 0.1f, 0.11f);
vec3 camPos(0.0f, 0.0f, 0.0f), camFront(0.0f,0.0f,-1.0f),camRight(1.0f,0.0f,0.0f);

const float m11[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
const float m22[] = { 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };

int main() {
	//Initialize and set up GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(w, h, "Engine", NULL, NULL);
	if (mainWindow == NULL){ //check whether the main window was created or not
		std::cout << "Window creation failed.\n";
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(mainWindow);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set callback functions
	glfwSetCursorPosCallback(mainWindow, mouseInput);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD could not be loaded.\n";
		glfwTerminate();
		system("pause");
		return -2;
	}
	
	//set opengl viewport
	glViewport(0, 0, w, h);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	Shader proj("shaders/vs_proj.vs", "shaders/fs_tex.fs");
	Shader axis("shaders/vs_unmod.vs", "shaders/fs_unmod.fs");
	Shader modelShader("shaders/vs_model.vs", "shaders/fs_model_phong1.fs");
	Shader boxLightning("shaders/vs_normals.vs", "shaders/fs_phong_normal.fs");
	Shader vertex_only("shaders/vs_proj_only_vertex.vs", "shaders/fs_proj_only_vertex.fs");

	GLuint vaoObj,vboObj;

	glGenVertexArrays(1, &vaoObj);
	glGenBuffers(1,&vboObj);
	glBindVertexArray(vaoObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sq), &sq[0].e[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	unsigned int containerTexture = TextureFromFile("container.jpg");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setGradientColor(topCol, botCol);
	glfwSwapBuffers(mainWindow);
	glfwPollEvents();

	//load models
	Model models[nModelsCount];
	models[0] = Model("models/zero_suit_samus/zeroSuitSamus.obj");
	models[1] = Model("models/fusion_suit/fusion_suit.obj");
	//models[2] = Model("models/varia-suit/varia_suit.obj");
	models[2] = Model("models/colors/colors.obj");
	/*
	models[3] = Model("models/patrick-star-spongebob/patrick.obj");
	models[4] = Model("models/belch/scene.gltf");
	models[5] = Model("models/Stormtrooper/Stormtrooper.obj");
	models[6] = Model("models/glock-22/glock.dae");
	models[7] = Model("models/beretta92fs/beretta92fs.dae");
	*/

	// light vecs
	vec3 lightColor(1.0f, 1.0f, 1.0f);
	vec3 lightPos(1.0f, 3.0f, -10.0f);

	//box vao
	unsigned int boxVAO, boxVBO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);
	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//light vao
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//box color
	vec3 boxColor(1.0f, 0.5f, 0.31f);

	//light hitted objects shader
	Shader lightShader("shaders/vs_external_color.vs", "shaders/fs_light1.fs");
	lightShader.use();
	lightShader.setVec3f("light_color", lightColor);
	lightShader.setVec3f("color", boxColor);

	//light source shader
	Shader lightSourceShader("shaders/vs_external_color.vs", "shaders/fs_light_source.fs");
	lightSourceShader.use();
	lightSourceShader.setVec3f("light_color", lightColor);

	//set projection matrix
	projection = setProjMatrix((float)w, (float)h, nearPlane, farPlane, fov);
	proj.setMat4f("projection", projection);

	axis.use();
	axis.setMat4f("projection", projection);

	modelShader.use();
	modelShader.setFloat("model_scale", 1.0f);
	modelShader.setMat4f("projection", projection);
	modelShader.setVec3f("light_color", lightColor);
	modelShader.setVec3f("light_pos", lightPos);
	modelShader.setMat4f("model", model);

	lightShader.use();
	lightShader.setMat4f("projection", projection);

	while (!glfwWindowShouldClose(mainWindow)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(mainWindow);
		view = lookAt(camPos, camPos + camFront);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setGradientColor(topCol, botCol);

		if (draw_in_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		float radius = 5.0f;
		float camX = -sin(glfwGetTime()*0.0f)*radius;
		float camZ = cos(glfwGetTime()*0.0f)*radius;
		lightPos = vec3(camX, lightPos.y(), camZ);

		if (draw_axis) {
			axis.use();
			axis.setMat4f("view", view);
			axis.setMat4f("projection", projection);
			drawAxis(axis);
		}

		if (draw_model) {
			modelShader.use();
			modelShader.setMat4f("view", view);
			modelShader.setMat4f("projection", projection);
			modelShader.setVec3f("light_pos", lightPos);
			boxLightning.setVec3f("light_color", lightColor);
			modelShader.setVec3f("pos_vec", camPos);
			model.makeIdentity();
			model.scale_u(0.1f);
			modelShader.setMat4f("model", model);
			models[current_model].Draw(modelShader);
		}

		if (draw_cube) {
			boxLightning.use();
			boxLightning.setMat4f("view", view);
			boxLightning.setMat4f("projection", projection);
			model.makeIdentity();
			boxLightning.setMat4f("model", model);
			boxLightning.setVec3f("pos_vec", camPos);
			boxLightning.setVec3f("light_pos", lightPos);
			boxLightning.setVec3f("light_color", lightColor);
			boxLightning.setVec3f("frag_color", boxColor);
			glBindVertexArray(boxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (draw_light) {
			lightSourceShader.use();
			lightSourceShader.setMat4f("view", view);
			lightSourceShader.setMat4f("projection", projection);
			model.makeIdentity();
			model.translate(lightPos);
			lightSourceShader.setMat4f("model", model);
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}
	return 0;
}