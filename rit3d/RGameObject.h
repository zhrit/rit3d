#pragma once

#include "RCommon.h"
#include "IComponent.h"

class CTransform;
class RScene;
class RGameObject
{
public:
	RGameObject(RScene* s, RString n = "UnName", RString t = "UnTag", LAYER l = LAYER::Default);
	~RGameObject();

	RString name;//名称
	RString tag;//类别

	CTransform* transform;//几何组件

private:
	LAYER m_layer{ LAYER::Default };//所在图层
	RScene* m_sce;//所属场景

	RBool m_activeSelf{ true };//自身的激活状态
	RBool m_active{ true };//在场景中的真实激活状态
	RBool m_activeDirty{ false };//标记激活状态是否可能改变

	std::map<COMPTYPE, IComponent*> m_compMap;

public:
	//获取图层
	LAYER getLayer() const;
	//设置图层
	void setLayer(LAYER _layer);

	//添加组件
	IComponent* addComponent(COMPTYPE type);

	//移除一个特定组件
	void removeComponent(COMPTYPE type);

	//移除所有特定组件
	void removeComponents(COMPTYPE type);

	//移除所有组件
	void removeAllComponent();

	//获取一个特定组件
	IComponent* getComponent(COMPTYPE type);

	//获取所有特定组件
	std::list<IComponent*> getComponents(COMPTYPE type);

	//获取所有组件
	std::list<IComponent*> getAllComponent();

	//是否激活，不仅看自己是否激活，还要看祖辈是否激活
	RBool isActive();

	//获取自身激活状态
	RBool getActiveSelf() const;

	//设置激活状态，仅仅设置自己的激活状态
	void setActiveSelf(RBool value);

	//激活状态标脏
	void _setActiveDirty();

	//获取对象所属场景
	RScene* getScene() const;

private:
	//销毁对象
	void _destroy();
};

