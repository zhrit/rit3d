#pragma once

#include "RCommon.h"
#include "ISystem.h"
#include "RTimer.h"

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

	//记录各个系统的运行时间
	std::map<SYSTEMTYPE, DWORD> m_elapsedTime;

	RTimer clock_display{ 1000, 1000 };

	//注册各个系统的创建函数，系统只能通过创建函数创建，不能直接new
	void registSystemCreateFunc();

	//注册各个组件的创建函数，组件只能通过创建函数创建，不能直接new
	void registCompCreateFunc();

	//初始化必要的单例组件
	void registSingltonComp();
public:
	//为程序注册一个制定的系统
	void registSystem(SYSTEMTYPE type, RInt od);
	//系统更新入口
	void update();
	//从程序已注册的全部系统中获取一个指定的系统
	ISystem* getSystem(SYSTEMTYPE type);
	//判断时候已经注册某个系统
	RBool hasSystem(SYSTEMTYPE type);
	//窗口大小变化时回调函数
	void onChangeSize(int _w, int _h);
	//添加新的组件时调用
	void onAddComponent(COMPTYPE type, IComponent* pComp);
	//移除组件时调用
	void onRemoveComponent(COMPTYPE type, IComponent* pComp);

	//鼠标事件
	virtual void onLeftButtonDown();
	virtual void onLeftButtonUp();
	virtual void onRightButtonDown();
	virtual void onRightButtonUp();
	virtual void onMouseMove(double _x, double _y);
	virtual void onScroll(double _x, double _y);
	//键盘事件
	virtual void onKeyDown(int key);
	virtual void onKeyKeep(int key);
	virtual void onKeyUp(int key);

	//设置碰撞检测策略
	void setCollisionDetectionStrategy(int _cdt);
};