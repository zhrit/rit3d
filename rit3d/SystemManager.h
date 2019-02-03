#pragma once

#include "RCommon.h"
#include "ISystem.h"
#include "RenderSystem.h"

/*
 * 系统管理单例类
 */
class SystemManager {
public:
	static SystemManager *Instance();
private:
	SystemManager();
	virtual ~SystemManager();

	static SystemManager *m_instance;

	//已注册的所有系统的实例
	std::list<ISystem*> m_systemList;

	//注册各个系统的创建函数，系统只能通过创建函数创建，不能直接new
	void registSystemCreateFunc();

	//注册各个组件的创建函数，组件只能通过创建函数创建，不能直接new
	void registCompCreateFunc();
public:
	//为程序注册一个制定的系统
	void registSystem(SYSTEMTYPE type, RInt od);
	//系统更新入口
	void update();
	//从程序已注册的全部系统中获取一个指定的系统
	ISystem* getSystem(SYSTEMTYPE type);
	//判断时候已经注册某个系统
	RBool hasSystem(SYSTEMTYPE type);
};