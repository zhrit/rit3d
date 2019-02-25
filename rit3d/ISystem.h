#pragma once

#include "RCommon.h"

/*
 * 全部系统的基类，所有系统必须继承于此类，并实现其中的纯虚函数
 * 所有系统类，必须含有自己的静态创建方法，并在systemmanager中注册
 */
class ISystem {
public:
	ISystem(RInt od);
	virtual ~ISystem();

	RInt order{ -1 };//系统执行顺序

	//创建系统实例，所有系统实例通过此方法创建
	static ISystem* CreateSystem(SYSTEMTYPE type, RInt od);

	//存放所有系统的创建方法，用来实现反射机制
	static std::map<SYSTEMTYPE, std::function<ISystem*(RUInt)>> systemMap;

	//子类需要实现static CreateInstance()方法

protected:
	RBool enabled{ true };//系统是否被激活

	RBool m_started{ false };//系统是否开启

	SYSTEMTYPE m_type;

public:
	//获取系统类型
	SYSTEMTYPE getType() const;

	//初始化系统
	void initSystem();

	//卸载系统
	void uninitSystem();

	//系统更新
	void update();

	//系统更新2
	void lateUpdate();

	//窗口大小改变时执行
	virtual void onChangeSize(int _w, int _h);
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

	//系统初始化时调用
	virtual void onAwake() = 0;

	//系统被激活时调用
	virtual void onEnable() = 0;

	//系统开始运行时调用
	virtual void onStart() = 0;

	//实体被添加到系统时调用
	virtual void onAddGameObject() = 0;

	//组件添加到实体时调用
	virtual void onAddComponent() = 0;

	//组件从实体移除时调用
	virtual void onRemoveComponent() = 0;

	//实体从系统中移除时调用
	virtual void onRemoveGameObject() = 0;

	//系统更新时调用
	virtual void onUpdate() = 0;

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate() = 0;

	//系统被禁用时调用
	virtual void onDisable() = 0;

	//系统被注销时调用
	virtual void onDestroy() = 0;

	//返回系统是否被激活
	RBool isEnabled() const;

	void setEnable(RBool value);

	RBool isStarted() const;

	void setStarted(RBool value);

private:

};


class StartSystem : public ISystem {
private:
	StartSystem(RInt od);
	virtual ~StartSystem();
public:
	static StartSystem* CreateInstance(RInt);
	
	//系统初始化时调用
	virtual void onAwake();

	//系统被激活时调用
	virtual void onEnable();

	//系统开始运行时调用
	virtual void onStart();

	//实体被添加到系统时调用
	virtual void onAddGameObject();

	//组件添加到实体时调用
	virtual void onAddComponent();

	//组件从实体移除时调用
	virtual void onRemoveComponent();

	//实体从系统中移除时调用
	virtual void onRemoveGameObject();

	//系统更新时调用
	virtual void onUpdate();

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate();

	//系统被禁用时调用
	virtual void onDisable();

	//系统被注销时调用
	virtual void onDestroy();
};