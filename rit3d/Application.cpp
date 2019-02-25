#include "pch.h"
#include "Application.h"
#include "RWindow.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GLProgram.h"
#include "CPostProcess.h"
#include "CSkybox.h"

Application::Application() {
};

Application* Application::m_instance = nullptr;

Application* Application::Instance() {
	if (nullptr == m_instance) {
		m_instance = new Application();
	}
	return m_instance;
}

//引擎入口
void Application::runRit3d(RString _title, int _w, int _h) {
	if (m_isRunning) {
		return;
	}
	//输出版本信息
	cout << "current version: " << m_version << endl;
	//创建窗口
	m_window = new RWindow(_title, 0, 0, _w, _h);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		cout << "application startup failed" << endl;
		return;
	}

	//引擎初始化
	_initApp();
	//游戏入口（预编辑）
	m_gameEntry();

	m_isRunning = true;
	cout << "application startup succeed" << endl;

	//主循环
	while (!m_window->RWindowShouldClose())
	{
		//渲染逻辑
		_updateApp();

		//swap buffers
		m_window->swapBuffer();
		//poll IO events(keys pressed / released, mouse moved etc.)
		m_window->pollEvents();
	}
	//删除VAO,VBO

	m_window->destroy();
	m_isRunning = false;
	return;
}

//显示fps
void Application::showFPS() {
	m_fps.show();
}
//隐藏fps
void Application::hideFPS() {
	m_fps.hide();
}

//引擎初始化
void Application::_initApp() {
	//初始化系统管理类
	systemMng = SystemManager::Instance();
	//初始化场景管理器
	sceneMng = SceneManager::Instance();
	//初始化资源管理器
	resourceMng = ResourceManager::Instance();
	//加载默认资源
	resourceMng->createDefaultMesh();
	resourceMng->createDefaultMaterial();
	resourceMng->createDefaultShader();
	//注册必要系统
	systemMng->registSystem(STARTSYSTEM, 1);
	systemMng->registSystem(BEHAVIORSYSTEM, 2);
	systemMng->registSystem(RENDERSYSTEM, 3);

	//获取窗口环境
	GLFWwindow* wd = m_window->getWindowContext();
	glfwSetKeyCallback(wd, Application::_key_callback);
	glfwSetFramebufferSizeCallback(wd, Application::_framebuffer_size_callback);
	glfwSetMouseButtonCallback(wd, Application::_mouse_button_callback);
	glfwSetCursorPosCallback(wd, Application::_cursor_position_callback);
	glfwSetScrollCallback(wd, Application::_scroll_callback);
}
//引擎驱动
void Application::_updateApp() {
	if (m_fps.isShowed()) {
		m_fps.update();
	}
	systemMng->update();
}

//设置游戏入口函数
void Application::setGameEntry(std::function<void()> _gameEntry) {
	m_gameEntry = _gameEntry;
}

//窗口大小变化时回调函数
void Application::_framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	SystemManager::Instance()->onChangeSize(w, h);
}
//鼠标按键回调函数
void Application::_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			//左键按下
			SystemManager::Instance()->onLeftButtonDown();
		}
		else if (action == GLFW_RELEASE) {
			//左键抬起
			SystemManager::Instance()->onLeftButtonUp();
		}
		else {

		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			//右键按下
			SystemManager::Instance()->onRightButtonDown();
		}
		else if (action == GLFW_RELEASE) {
			//右键抬起
			SystemManager::Instance()->onRightButtonUp();
		}
		else {

		}
	}
	else {

	}
}
//光标位置回调
void Application::_cursor_position_callback(GLFWwindow* window, double x, double y) {
	SystemManager::Instance()->onMouseMove(x, y);
}
//鼠标滚轮回调
void Application::_scroll_callback(GLFWwindow* window, double x, double y) {
	SystemManager::Instance()->onScroll(x, y);
}
//键盘按键回调
void Application::_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (action)
	{
	case GLFW_PRESS:
		SystemManager::Instance()->onKeyDown(key);
		break;
	case GLFW_RELEASE:
		SystemManager::Instance()->onKeyUp(key);
		break;
	case GLFW_REPEAT:
		SystemManager::Instance()->onKeyKeep(key);
		break;
	default:
		break;
	}
}
