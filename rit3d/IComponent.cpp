#include "pch.h"
#include "IComponent.h"
#include "RGameObject.h"


IComponent::IComponent() {

}


IComponent::~IComponent() {

}

//存放所有组件的创建方法，用来实现反射机制
std::map<COMPTYPE, std::function<IComponent*()>> IComponent::compMap;

//创建系统实例，所有系统实例通过此方法创建
IComponent* IComponent::CreateComponent(COMPTYPE type) {
	IComponent* comp = compMap[type]();
	return comp;
}

//获取系统类型
COMPTYPE IComponent::getType() const {
	return m_type;
}

//是否允许在同一实体上添加多个该组件的实例
RBool IComponent::allowMultiple() const {
	return m_allowMultiple;
}

//是否可用
RBool IComponent::isEnabled() const {
	return m_enabled;
}
//设置可用状态
void IComponent::setEnabled(RBool value) {
	if (m_enabled == value) {
		return;
	}

	m_enabled = value;
}
//是否激活且可用
RBool IComponent::isActiveAndEnabled() const {
	return m_enabled && gameObject->isActive();
}
