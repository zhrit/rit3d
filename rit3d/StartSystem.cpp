#include "pch.h"
#include "ISystem.h"

StartSystem::StartSystem(RInt od) : ISystem(od) {
	m_type = STARTSYSTEM;
}
StartSystem::~StartSystem() {

}

StartSystem* StartSystem::CreateInstance(RInt od) {
	return new StartSystem(od);
}

//系统初始化时调用
void StartSystem::onAwake() {

}

//系统被激活时调用
void StartSystem::onEnable() {

}

//系统开始运行时调用
void StartSystem::onStart() {

}

//实体被添加到系统时调用
void StartSystem::onAddGameObject() {

}

//组件添加到实体时调用
void StartSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {

}

//组件从实体移除时调用
void StartSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {

}

//实体从系统中移除时调用
void StartSystem::onRemoveGameObject() {

}

//系统更新时调用
void StartSystem::onUpdate(DWORD deltaT) {
	
}

//系统更新时onUpdate之后调用
void StartSystem::onLateUpdate() {

}

//系统被禁用时调用
void StartSystem::onDisable() {

}

//系统被注销时调用
void StartSystem::onDestroy() {

}