#pragma once
#include "RCommon.h"
#include "ISystem.h"
#include "Material.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight.h"

class RenderSystem : public ISystem {
private:
	RenderSystem(RInt od);
	virtual ~RenderSystem();
public:
	static RenderSystem* CreateInstance(RInt);

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

private:
	//核心渲染函数
	void _render();

	//更新uniform变量
	void _updateUniforms(Material* pMat, CCamera* camera, CTransform* tran, std::list<CLight*> lights);

	//更新光源相关的uniform变量
	void _updateLightsUniforms(Material* pMat, std::list<CLight*> lights);
};