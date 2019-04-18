#pragma once
#include "RCommon.h"
#include "ISystem.h"
#include "CHair.h"

class HairSystem : public ISystem {
private:
	HairSystem(RInt od);
	virtual ~HairSystem();

	std::vector<CHair*> m_hairPool;

public:
	void addHair(CHair* _b);
	void removeHair(CHair* _b);
	static HairSystem* CreateInstance(RInt);

	//系统初始化时调用
	virtual void onAwake();

	//系统被激活时调用
	virtual void onEnable();

	//系统开始运行时调用
	virtual void onStart();

	//实体被添加到系统时调用
	virtual void onAddGameObject();

	//组件添加到实体时调用
	virtual void onAddComponent(COMPTYPE type, IComponent* pComp);

	//组件从实体移除时调用
	virtual void onRemoveComponent(COMPTYPE type, IComponent* pComp);

	//实体从系统中移除时调用
	virtual void onRemoveGameObject();

	//系统更新时调用
	virtual void onUpdate(DWORD deltaT);

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate();

	//系统被禁用时调用
	virtual void onDisable();

	//系统被注销时调用
	virtual void onDestroy();

private:
	//更新头发数据
	void _updateHair(DWORD deltaT);

	//绘制头发
	void _drawHair();

	//头发的碰撞检测
	void _collideHair(CHair* ch, glm::vec3& point);

	//节点数据从局部坐标系转换到世界坐标系
	void _transform(CHair* ch);
};