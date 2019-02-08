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
	app->showFPS();
	RScene* pSce = app->sceneMng->createScene();

	//纹理和材质
	Texture* tex1 = app->resourceMng->createTexture("container.jpg");
	Material* mat = app->resourceMng->createMaterial("mat1");
	mat->addTexture(tex1);
	mat->setShader("phong");

	//平面
	RGameObject* plane = pSce->addGameObject();
	plane->transform->setLocalScale(1.0f, 1.0f, 1.0f);
	CRender* planerender = (CRender*)plane->addComponent(RENDER);
	planerender->setMesh(app->resourceMng->getMesh("plane"));
	planerender->setMaterial(mat);
	planerender->recieveShadow(true);
	//立方体
	RGameObject* cube = pSce->addGameObject();
	cube->transform->setLocalPosition(0.0f, 0.5f, 0.0f);
	cube->transform->setLocalScale(0.3f, 0.3f, 0.3f);
	cube->transform->setLocalFrontDir(0.2f, 0.0f, 0.9f);
	CRender* cuberender = (CRender*)cube->addComponent(RENDER);
	cuberender->setMesh(app->resourceMng->getMesh("cube"));
	cuberender->setMaterial(mat);
	cuberender->castShadow(true);
	//球
	RGameObject* sphere = pSce->addGameObject();
	sphere->transform->setLocalPosition(-1.0f, 0.5f, 0.5f);
	sphere->transform->setLocalScale(0.3f, 0.3f, 0.3f);
	sphere->transform->setLocalFrontDir(0.2f, 0.0f, 0.9f);
	CRender* sphererender = (CRender*)sphere->addComponent(RENDER);
	sphererender->setMesh(app->resourceMng->getMesh("sphere"));
	sphererender->setMaterial(mat);
	sphererender->castShadow(true);
	//sphere->setLayer(LAYER::BuiltinLayer1);

	//相机
	RGameObject* CO = pSce->addGameObject();
	CO->transform->setLocalPosition(0.0f, 4.0f, 6.0f);
	CO->transform->setLocalFrontDir(0.0f, -2.0f, -3.0f);
	CO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CCamera* cam = (CCamera*)CO->addComponent(COMPTYPE::CAMERA);
	cam->setCameraType(CAMERATYPE::PERSPECTIVE);
	cam->setPerspFrustum(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);
	//CPostProcess* post = (CPostProcess*)CO->addComponent(COMPTYPE::POSTPROCESS);
	//post->setShaderPath("edgedetection");
	CSkybox* skybox = (CSkybox*)CO->addComponent(COMPTYPE::SKYBOX);
	std::vector<RString> path;
	path.push_back("ashcanyon_rt.tga");
	path.push_back("ashcanyon_lf.tga");
	path.push_back("ashcanyon_up.tga");
	path.push_back("ashcanyon_dn.tga");
	path.push_back("ashcanyon_ft.tga");
	path.push_back("ashcanyon_bk.tga");
	//container.jpg
	skybox->setTextures(path);
	

	RGameObject* LO = pSce->addGameObject();
	LO->transform->setLocalPosition(3.0f, 1.5f, 0.0f);
	LO->transform->setLocalFrontDir(-1.0f, -0.5f, 0.0f);
	LO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light = (CLight*)LO->addComponent(COMPTYPE::LIGHT);
	light->castShadow(true);
	light->setColor(1.0f, 1.0f, 1.0f);
	light->setLightType(LIGHTTYPE::DIRECTION);
	light->setEnabled(true);

	RGameObject* LO1 = pSce->addGameObject();
	LO1->transform->setLocalPosition(0.0f, 2.0f, 0.0f);
	LO1->transform->setLocalFrontDir(1.0f, -0.5f, 0.0f);
	LO1->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light1 = (CLight*)LO1->addComponent(COMPTYPE::LIGHT);
	light1->castShadow(true);
	light1->setIntensity(0.1f, 1.0f, 1.0f);
	light1->setColor(1.0f, 1.0f, 1.0f);
	light1->setLightType(LIGHTTYPE::LPOINT);

	RGameObject* LO2 = pSce->addGameObject();
	LO2->transform->setLocalPosition(2.0f, 3.0f, -1.0f);
	LO2->transform->setLocalFrontDir(-2.0f, -2.5f, 1.0f);
	LO2->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light2 = (CLight*)LO2->addComponent(COMPTYPE::LIGHT);
	light2->castShadow(true);
	light2->setIntensity(0.1f, 1.0f, 1.0f);
	light2->setColor(1.0f, 0.0f, 0.0f);
	light2->setCutOff(10, 15);
	light2->setLightType(LIGHTTYPE::SPOT);
}
