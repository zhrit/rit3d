// rit3d.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <glad/glad.h>
#include "declare.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void example_light_shadow();
void example_pbr();
void example_bloom();
void example_normalMap();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	Application::Instance()->setGameEntry(example_normalMap);
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

//光照和阴影
void example_light_shadow() {
	cout << "light and shadow" << endl;
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
	sphere->transform->setLocalFrontDir(1.0f, 0.0f, 0.0f);
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
//pbr材质
void example_pbr() {
	cout << "pbr" << endl;
	Application* app = Application::Instance();
	app->showFPS();
	RScene* pSce = app->sceneMng->createScene();

	Texture* tex0 = app->resourceMng->createTexture("rustediron2_basecolor.png");
	Texture* tex1 = app->resourceMng->createTexture("rustediron2_metallic.png");
	Texture* tex2 = app->resourceMng->createTexture("rustediron2_roughness.png");

	Material* mat = app->resourceMng->createMaterial("mat1" );
	mat->setShader("pbr");
	mat->setColor(0.5, 0.0f, 0.0);
	mat->addTexture(tex0);
	mat->addTexture(tex1);
	mat->addTexture(tex2);

	RInt nrRows = 7;
	RInt nrCols = 7;
	RFloat spacing = 1.5;
	for (RInt col = 0; col < nrCols; col++) {
		for (RInt row = 0; row < nrRows; row++) {
			//Material* mat = app->resourceMng->createMaterial("mat"+util::num2str(row) + util::num2str(col));
			//mat->setShader("pbr");
			//mat->setColor(0.5, 0.0f, 0.0);
			//mat->setMetallic((float)row / (float)nrRows);
			//mat->setRoughness((float)col / (float)nrCols * 0.95f + 0.05f);
			//mat->addTexture(tex0);
			//mat->addTexture(tex1);
			//mat->addTexture(tex2);
			//mat->addTexture(tex3);

			//球
			RGameObject* sphere = pSce->addGameObject();
			sphere->transform->setLocalPosition((col - (nrCols / 2)) * spacing,
				(row - (nrRows / 2)) * spacing,
				0.0f);
			CRender* sphererender = (CRender*)sphere->addComponent(RENDER);
			sphererender->setMesh(app->resourceMng->getMesh("sphere"));
			sphererender->setMaterial(mat);
			sphererender->castShadow(false);
		}
	}

	//相机
	RGameObject* CO = pSce->addGameObject();
	CO->transform->setLocalPosition(-13.2f, 0.0f, 9.2f);
	CO->transform->setLocalFrontDir(13.2f, 0.0f, -9.2f);
	//CO->transform->setLocalPosition(0.0f, 0.0f, 13.0f);
	//CO->transform->setLocalFrontDir(0.0f, 0.0f, -1.0f);
	CO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CCamera* cam = (CCamera*)CO->addComponent(COMPTYPE::CAMERA);
	cam->setCameraType(CAMERATYPE::PERSPECTIVE);
	cam->setPerspFrustum(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);
	cam->backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	CSkybox* skybox = (CSkybox*)CO->addComponent(COMPTYPE::SKYBOX);
	std::vector<RString> path;
	path.push_back("ashcanyon_lf.tga");
	path.push_back("ashcanyon_rt.tga");
	path.push_back("ashcanyon_dn.tga");
	path.push_back("ashcanyon_up.tga");
	path.push_back("ashcanyon_ft.tga");
	path.push_back("ashcanyon_bk.tga");
	//container.jpg
	skybox->setTextures(path);

	RGameObject* LO1 = pSce->addGameObject();
	LO1->transform->setLocalPosition(-5.0, 5.0f, 5.0f);
	CLight* light1 = (CLight*)LO1->addComponent(COMPTYPE::LIGHT);
	light1->castShadow(false);
	light1->setColor(300.0f, 300.0f, 300.0f);
	light1->setLightType(LIGHTTYPE::LPOINT);

	RGameObject* LO2 = pSce->addGameObject();
	LO2->transform->setLocalPosition(5.0, 5.0f, 5.0f);
	CLight* light2 = (CLight*)LO2->addComponent(COMPTYPE::LIGHT);
	light2->castShadow(false);
	light2->setColor(300.0f, 300.0f, 300.0f);
	light2->setLightType(LIGHTTYPE::LPOINT);

	RGameObject* LO3 = pSce->addGameObject();
	LO3->transform->setLocalPosition(-5.0, -5.0f, 5.0f);
	CLight* light3 = (CLight*)LO3->addComponent(COMPTYPE::LIGHT);
	light3->castShadow(false);
	light3->setColor(300.0f, 300.0f, 300.0f);
	light3->setLightType(LIGHTTYPE::LPOINT);

	RGameObject* LO4 = pSce->addGameObject();
	LO4->transform->setLocalPosition(5.0, -5.0f, 5.0f);
	CLight* light4 = (CLight*)LO4->addComponent(COMPTYPE::LIGHT);
	light4->castShadow(false);
	light4->setColor(300.0f, 300.0f, 300.0f);
	light4->setLightType(LIGHTTYPE::LPOINT);

}
//泛光
void example_bloom() {
	cout << "bloom" << endl;
	Application* app = Application::Instance();
	app->showFPS();
	RScene* pSce = app->sceneMng->createScene();

	//纹理和材质
	Texture* tex1 = app->resourceMng->createTexture("container.jpg");
	Material* mat = app->resourceMng->createMaterial("mat");
	mat->addTexture(tex1);
	mat->setShader("phong");

	Material* mat1 = app->resourceMng->createMaterial("mat1");
	mat1->setShader("simple");
	mat1->setColor(10.0f, 10.0f, 10.0f);
	Material* mat2 = app->resourceMng->createMaterial("mat2");
	mat2->setShader("simple");
	mat2->setColor(10.0f, 0.0f, 0.0f);


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
	sphere->transform->setLocalFrontDir(1.0f, 0.0f, 0.0f);
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
	cam->setBloom(2.0f);
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
	LO1->transform->setLocalScale(0.2f, 0.2f, 0.2f);
	CLight* light1 = (CLight*)LO1->addComponent(COMPTYPE::LIGHT);
	light1->castShadow(true);
	light1->setIntensity(0.1f, 1.0f, 1.0f);
	light1->setColor(1.0f, 1.0f, 1.0f);
	light1->setLightType(LIGHTTYPE::LPOINT);
	CRender* lo1render = (CRender*)LO1->addComponent(RENDER);
	lo1render->setMesh(app->resourceMng->getMesh("sphere"));
	lo1render->setMaterial(mat1);

	RGameObject* LO2 = pSce->addGameObject();
	LO2->transform->setLocalPosition(1.6f, 2.4f, -0.8f);
	LO2->transform->setLocalFrontDir(-2.0f, -2.5f, 1.0f);
	LO2->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	LO2->transform->setLocalScale(0.2f, 0.2f, 0.2f);
	CLight* light2 = (CLight*)LO2->addComponent(COMPTYPE::LIGHT);
	light2->castShadow(true);
	light2->setIntensity(0.1f, 1.0f, 1.0f);
	light2->setColor(1.0f, 0.0f, 0.0f);
	light2->setCutOff(10, 15);
	light2->setLightType(LIGHTTYPE::SPOT);
	CRender* lo2render = (CRender*)LO2->addComponent(RENDER);
	lo2render->setMesh(app->resourceMng->getMesh("sphere"));
	lo2render->setMaterial(mat2);
}
//法线贴图
void example_normalMap() {
	cout << "normal map" << endl;
	Application* app = Application::Instance();
	app->showFPS();
	RScene* pSce = app->sceneMng->createScene();

	//纹理和材质
	Texture* tex1 = app->resourceMng->createTexture("brickwall.jpg");
	Texture* tex2 = app->resourceMng->createTexture("brickwall_normal.jpg");
	Material* mat = app->resourceMng->createMaterial("mat1");
	mat->addTexture(tex1);
	mat->setShader("pbr");
	mat->setNormalMap(tex2);
	mat->setMetallic(0.0f);
	mat->setRoughness(0.8f);

	Material* mat1 = app->resourceMng->createMaterial("mat2");
	mat1->addTexture(tex1);
	mat1->setShader("pbr");
	mat1->setMetallic(0.0f);
	mat1->setRoughness(0.8f);

	//平面
	RGameObject* plane1 = pSce->addGameObject();
	plane1->transform->setLocalPosition(-3.0f, 0.0f, 0.0);
	plane1->transform->setLocalScale(0.5f, 1.0f, 0.5f);
	plane1->transform->setLocalUpDir(0.5f, 0.0f, 0.866f);
	plane1->transform->setLocalFrontDir(0.0f, -1.0f, 0.0f);
	CRender* planerender1 = (CRender*)plane1->addComponent(RENDER);
	planerender1->setMesh(app->resourceMng->getMesh("plane"));
	planerender1->setMaterial(mat);

	RGameObject* plane2 = pSce->addGameObject();
	plane2->transform->setLocalPosition(3.0f, 0.0f, 0.0);
	plane2->transform->setLocalScale(0.5f, 1.0f, 0.5f);
	plane2->transform->setLocalUpDir(-0.5f, 0.0f, 0.866f);
	plane2->transform->setLocalFrontDir(0.0f, -1.0f, 0.0f);
	CRender* planerender2 = (CRender*)plane2->addComponent(RENDER);
	planerender2->setMesh(app->resourceMng->getMesh("plane"));
	planerender2->setMaterial(mat1);

	//相机
	RGameObject* CO = pSce->addGameObject();
	CO->transform->setLocalPosition(0.0f, 0.0f, 10.0f);
	CO->transform->setLocalFrontDir(0.0f, 0.0f, -3.0f);
	CO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CCamera* cam = (CCamera*)CO->addComponent(COMPTYPE::CAMERA);
	cam->setCameraType(CAMERATYPE::PERSPECTIVE);
	cam->setPerspFrustum(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);

	RGameObject* LO = pSce->addGameObject();
	LO->transform->setLocalPosition(3.0f, 1.5f, 0.0f);
	LO->transform->setLocalFrontDir(-1.0f, -0.5f, -1.0f);
	LO->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light = (CLight*)LO->addComponent(COMPTYPE::LIGHT);
	light->castShadow(false);
	light->setColor(1.0f, 1.0f, 1.0f);
	light->setLightType(LIGHTTYPE::DIRECTION);

	RGameObject* LO1 = pSce->addGameObject();
	LO1->transform->setLocalPosition(0.0f, 2.0f, 2.0f);
	LO1->transform->setLocalFrontDir(1.0f, -0.5f, 0.0f);
	LO1->transform->setLocalUpDir(0.0f, 1.0f, 0.0f);
	CLight* light1 = (CLight*)LO1->addComponent(COMPTYPE::LIGHT);
	light1->castShadow(false);
	light1->setIntensity(0.1f, 1.0f, 1.0f);
	light1->setColor(300.0f, 300.0f, 300.0f);
	light1->setLightType(LIGHTTYPE::LPOINT);
}
