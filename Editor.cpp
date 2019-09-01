#include "Editor.h"

const vec3 axis[] = {
	///vertices				//colors
	{-50.0f, 0.0f, 0.0f},	{1.0f,0.0f,0.0f},	//eje X: rojo
	{50.0f, 0.0f, 0.0f},	{1.0f,0.0f,0.0f},

	{0.0f, 0.0f, 50.0f},	{0.0f,0.0f,1.0f}, //eje Z: azul
	{0.0f, 0.0f,-50.0f},	{0.0f,0.0f,1.0f},

	{0.0f, 100.0f, 0.0f},	{0.0f,1.0f,0.0f}, //eje Y: verde
	{0.0f,-100.0f,0.0f},	{0.0f,1.0f,0.0f}
};

const float square[] = {
	-10.0f, 0.0f, 10.0f,
	 10.0f, 0.0f, 10.0f,
	-10.0f, 0.0f,-10.0f,
	 10.0f, 0.0f,-10.0f
};

const vec3 xDivisions[]={
	{0.0f, 1.0f, -0.1f},	{1.0f,0.0f,0.0f},
	{0.0f,-1.0f, -0.1f}, {1.0f,0.0f,0.0f}
};
const unsigned int iAxis[] = { 0,1,	2,3, 4,5 };
const unsigned int iX[] =	 { 0,1 };


void xMeasures(const Shader &shader){

	static unsigned int xmVao = 0;
	static unsigned int xmVbo = 0;
	static unsigned int xmEbo = 0;
	static mat4x4 model;
	if (!xmVao) {
		glGenVertexArrays(1, &xmVao);
		glGenBuffers(1, &xmVbo);
		glGenBuffers(1, &xmEbo);
		glBindVertexArray(xmVao);
		glBindBuffer(GL_VERTEX_ARRAY, xmVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(xDivisions), &xDivisions[0].e[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, xmEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iX), iX, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void*) (3*sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	shader.use();
	glBindVertexArray(xmVao);
	for (int i = 1; i < 2; i++) {
		model(3, 0) = ((float)i);
		shader.setMat4f("model", model);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		model(3, 0) = ((float)(-i));
		shader.setMat4f("model", model);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}

	
}

void drawFloorPlane(const Shader &shader,const mat4x4 view, const mat4x4 projection) {
	static unsigned int vao = 0;
	static unsigned int vbo = 0;
	if (!vao) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	shader.use();
	shader.setMat4f("view", view);
	shader.setMat4f("projection", projection);
	shader.setVec3f("color", 0.8f, 0.8f, 0.8f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
}

void drawAxis(const Shader &shader) {
	static unsigned int axisVAO = 0;
	static unsigned int axisVBO = 0;
	static unsigned int axisEBO = 0;
	static mat4x4 modelA;
	if(!axisVAO) {
		glGenVertexArrays(1, &axisVAO);
		glGenBuffers(1,&axisVBO);
		glGenBuffers(1, &axisEBO);
		glBindVertexArray(axisVAO);
		glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axis), &axis[0].e[0], GL_DYNAMIC_DRAW);
		//buffer data: target, sizeof, data of array where info is, mode
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iAxis), iAxis, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	shader.use();
	shader.setMat4f("model", modelA);
	glBindVertexArray(axisVAO);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT,0);
}


void setGradientColor(const vec3 &top, const vec3 &bot) {
	glDisable(GL_DEPTH_TEST);
	static unsigned int backgroundVAO = 0;
	static Shader background;

	if (!backgroundVAO) {
		glGenVertexArrays(1, &backgroundVAO);
		background.compile("shaders/vs_background_dg.vs", "shaders/fs_background_dg.fs");
	}
	background.use();
	background.setVec3f("topColor", top.x(), top.y(), top.z());
	background.setVec3f("botColor", bot.x(), bot.y(), bot.z());
	glBindVertexArray(backgroundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
