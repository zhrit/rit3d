#pragma once
#include "RCommon.h"

class RGameObject;
class IComponent
{
public:
	IComponent();
	virtual ~IComponent();

	//创建系统实例，所有系统实例通过此方法创建
	static IComponent* CreateComponent(COMPTYPE type);

	//存放所有组件的创建方法，用来实现反射机制
	static std::map<COMPTYPE, std::function<IComponent*()>> compMap;

	RGameObject* gameObject{ nullptr };//组件的实体

protected:
	RBool m_enabled{ true };//组件是否活跃

	RBool m_allowMultiple{ false };//是否允许在同一实体上添加多个该组件的实例

	COMPTYPE m_type;

public:
	//获取系统类型
	COMPTYPE getType() const;

	//是否允许在同一实体上添加多个该组件的实例
	RBool allowMultiple() const;

	//是否可用
	RBool isEnabled() const;
	//设置可用状态
	void setEnabled(RBool value);
	//是否激活且可用
	RBool isActiveAndEnabled() const;
};

