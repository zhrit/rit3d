#pragma once
#include "RCommon.h"
#include "ISystem.h"

class CBehavior;
class BehaviorSystem : public ISystem {
private:
	BehaviorSystem(RInt od);
	virtual ~BehaviorSystem();

	std::vector<CBehavior*> m_behaviorPool;
public:
	void addBehavior(CBehavior* _b);
	void removeBehavior(CBehavior* _b);
	static BehaviorSystem* CreateInstance(RInt);

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
};