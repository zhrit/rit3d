#pragma once

#include "RCommon.h"

const RInt DEFAULT_WIDTH = 800;
const RInt DEFAULT_HEIGHT = 600;

class SystemManager;
class SceneManager;
class ResourceManager;
class RWindow;
/*
 * 应用单例类
 */
class Application {
public:
	static Application *Instance();
private:
	Application();
	virtual ~Application() {};

private:
	static Application *m_instance;

	const RString m_version = "0.0.1";

	RBool m_isRunning{ false };

	RWindow* m_window{ nullptr };

	//游戏入口函数，在主循环前的预编辑，游戏开发者需要实现此函数
	std::function<void()> m_gameEntry;

	//当前产生过的对象总数
	RUInt m_objectCounter{ 0 };

public:
	//系统管理器
	SystemManager* systemMng{ nullptr };
	//场景管理器
	SceneManager* sceneMng{ nullptr };
	//资源管理器
	ResourceManager* resourceMng{ nullptr };
	//引擎入口
	void runRit3d(RString _title = "Rit3D", int _w = DEFAULT_WIDTH, int _h = DEFAULT_HEIGHT);
	//设置游戏入口函数
	void setGameEntry(std::function<void()> _gameEntry);
	//id分配器初始化
	void initIDAllocator();
	//id分配器
	RUInt IDAllocator();
	//设置碰撞检测策略
	void setCollisionDetectionStrategy(int _cdt);
private:
	//引擎初始化
	void _initApp();
	//引擎驱动
	void _updateApp();
	//窗口大小变化时回调函数
	static void _framebuffer_size_callback(GLFWwindow* window, int w, int h);
	//鼠标按键回调函数
	static void _mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	//光标位置回调
	static void _cursor_position_callback(GLFWwindow* window, double x, double y);
	//鼠标滚轮回调
	static void _scroll_callback(GLFWwindow* window, double x, double y);
	//键盘按键回调
	static void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


