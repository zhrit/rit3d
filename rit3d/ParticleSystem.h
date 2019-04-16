#pragma once
#include "RCommon.h"
#include "ISystem.h"

class CParticle;
class CCamera;
class ParticleSystem : public ISystem {
private:
	ParticleSystem(RInt od);
	virtual ~ParticleSystem();

	std::vector<CParticle*> m_particlePool;

	RUInt m_VAO;
public:
	void addParticle(CParticle* _b);
	void removeParticle(CParticle* _b);
	static ParticleSystem* CreateInstance(RInt);

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
	//更新粒子信息
	void _updateParticles(DWORD deltaT);
	//渲染粒子
	void _drawParticles();
	//寻找一个没用的粒子
	RInt _findUnusedParticle(CParticle* cp);
};