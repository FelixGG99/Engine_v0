#include "vec.h"
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<algorithm>
#include<iostream>
#include "Projection.h"
#ifndef pi
#define pi 3.141592f
#endif // !pi

void processInput(GLFWwindow *window);
void keyboardInput(GLFWwindow *window);
void mouseInput(GLFWwindow *window, double xpos, double ypos);
void framebuffer_callback(GLFWwindow *window, int w, int h);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);