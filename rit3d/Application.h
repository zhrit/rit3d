#pragma once

#include "RCommon.h"
#include "RWindow.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CRender.h"

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

public:
	//系统管理器
	SystemManager* systemMng{ nullptr };
	//场景管理器
	SceneManager* sceneMng{ nullptr };
	//资源管理器
	ResourceManager* resourceMng{ nullptr };
	//引擎入口
	void runRit3d();
	//设置游戏入口函数
	void setGameEntry(std::function<void()> _gameEntry);
private:
	//引擎初始化
	void _initApp();
	//引擎驱动
	void _updateApp();
};


