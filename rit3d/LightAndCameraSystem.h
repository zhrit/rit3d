#pragma once
#include "RCommon.h"
#include "ISystem.h"

class LightAndCameraSystem : public ISystem {
private:
	LightAndCameraSystem(RInt od);
	virtual ~LightAndCameraSystem();

public:
	static LightAndCameraSystem* CreateInstance(RInt od);

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
	virtual void onUpdate();

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate();

	//系统被禁用时调用
	virtual void onDisable();

	//系统被注销时调用
	virtual void onDestroy();
};



