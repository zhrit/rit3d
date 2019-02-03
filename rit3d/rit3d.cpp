// rit3d.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "Application.h"
#include <glad/glad.h>
#include "RVector.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void preEdit();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	cout << util::num2str(432) << endl;
	Application::Instance()->setGameEntry(preEdit);
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

void preEdit() {
	cout << "preEdit" << endl;
	Application* app = Application::Instance();
	glEnable(GL_DEPTH_TEST);
	RScene* pSce = app->sceneMng->createScene();
	RGameObject* pGO = pSce->addGameObject();
	pGO->transform->setLocalPosition(0.0f, -3.0f, -15.0f);
	pGO->transform->setLocalScale(2.0f, 2.0f, 2.0f);
	pGO->transform->setLocalFrontDir(0.2f, 0.0f, 0.9f);

	Texture* tex1 = app->resourceMng->createTexture("container.jpg");
	Texture* tex2 = app->resourceMng->createTexture("awesomeface.png");
	Material* mat = app->resourceMng->createMaterial("mat1");
	mat->addTexture(tex1);
	//mat->addTexture(tex2);
	mat->setShader("phone");
	CRender* crender = (CRender*)pGO->addComponent(RENDER);
	crender->setMesh(app->resourceMng->getMesh("cube"));
	crender->setMaterial(mat);

	RGameObject* CO = pSce->addGameObject();
	CO->transform->setLocalPosition(0.0f, 0.0f, 3.0f);
	CO->transform->setLocalFrontDir(0.0f, 0.0f, 1.0f);
	CO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CCamera* cam = (CCamera*)CO->addComponent(COMPTYPE::CAMERA);
	cam->setCameraType(CAMERATYPE::PERSPECTIVE);
	cam->setPerspFrustum(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);

	RGameObject* LO = pSce->addGameObject();
	LO->transform->setLocalPosition(0.0f, 0.0f, 0.0f);
	LO->transform->setLocalFrontDir(-1.0f, -1.0f, -1.0f);
	LO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light = (CLight*)LO->addComponent(COMPTYPE::LIGHT);
	light->setColor(0.0f, 0.0f, 1.0f);

	RGameObject* LO1 = pSce->addGameObject();
	LO1->transform->setLocalPosition(0.0f, 0.0f, 0.0f);
	LO1->transform->setLocalFrontDir(0.0f, 0.0f, -1.0f);
	LO1->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light1 = (CLight*)LO1->addComponent(COMPTYPE::LIGHT);
	light1->setColor(0.0f, 1.0f, 0.0f);
}
