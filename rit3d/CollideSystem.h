#pragma once
#include "RCommon.h"
#include "ISystem.h"
#include "CCollider.h"

class CollideSystem : public ISystem {
private:
	CollideSystem(RInt od);
	virtual ~CollideSystem();

	std::vector<CCollider*> m_colliderPool;
	std::function<RBool(CCollider*, CCollider*)> m_intersectionMethod[2][2];
public:
	static CollideSystem* CreateInstance(RInt od);

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

private:
	//相交检测
	RBool _intersectionTest(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_sphere2sphere(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_sphere2box(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_box2sphere(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_box2box(CCollider* c1, CCollider* c2);
};

