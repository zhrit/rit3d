#pragma once
#include "RCommon.h"
#include "Object.h"

//组件类型
typedef enum {
	TRANFORM = 0,
	//渲染相关组件
	MESH,
	RENDER,
	CAMERA,
	LIGHT,
	POSTPROCESS,
	SKYBOX,
	//脚本相关组件
	BEHAVIOR,
	//物理相关组件
	SPHERECOLLIDER,
	BOXCOLLIDER,

	//单例组件
	LIGHTCAMERACOLLECTER,
} COMPTYPE;

class RGameObject;
class IComponent : public Object {
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

	RBool m_isSingleton{ false };//是否是单例组件

	COMPTYPE m_type;

public:
	//获取系统类型
	COMPTYPE getType() const;

	//是否允许在同一实体上添加多个该组件的实例
	RBool allowMultiple() const;

	//是否是单例组件
	RBool isSingleton() const;

	//是否可用
	RBool isEnabled() const;
	//设置可用状态
	void setEnabled(RBool value);
	//是否激活且可用
	RBool isActiveAndEnabled() const;
};

