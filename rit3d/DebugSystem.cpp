#include "pch.h"
#include "DebugSystem.h"


DebugSystem::DebugSystem(RInt od) : ISystem(od) {
	m_type = DEBUGSYSTEM;
}


DebugSystem::~DebugSystem() {

}

DebugSystem* DebugSystem::CreateInstance(RInt od) {
	return new DebugSystem(od);
}

//系统初始化时调用
void DebugSystem::onAwake() {

}

//系统被激活时调用
void DebugSystem::onEnable() {

}

//系统开始运行时调用
void DebugSystem::onStart() {

}

//实体被添加到系统时调用
void DebugSystem::onAddGameObject() {

}

//组件添加到实体时调用
void DebugSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {

}

//组件从实体移除时调用
void DebugSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {

}

//实体从系统中移除时调用
void DebugSystem::onRemoveGameObject() {

}

//系统更新时调用
void DebugSystem::onUpdate(DWORD deltaT) {
	m_fps.update();
}

//系统更新时onUpdate之后调用
void DebugSystem::onLateUpdate() {

}

//系统被禁用时调用
void DebugSystem::onDisable() {

}

//系统被注销时调用
void DebugSystem::onDestroy() {

}
