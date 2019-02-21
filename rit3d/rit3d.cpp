// rit3d.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
//#include <Windows.h>
#include <glad/glad.h>
#include "declare.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void example_light_shadow_1();
void example_light_shadow_2();
void example_light_shadow_3();
void example_light_shadow_4();
void example_light_shadow_5();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	Application::Instance()->setGameEntry(example_light_shadow_5);
	Application::Instance()->runRit3d();
	return 0;
}

/**
 *窗口大小调整时调用
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/**
 *按键
 */
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


