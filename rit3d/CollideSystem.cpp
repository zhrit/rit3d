#include "pch.h"
#include "CollideSystem.h"
#include "CCollider.h"

CollideSystem::CollideSystem(RInt od) : ISystem(od) {
	m_type = COLLIDESYSTEM;
}

CollideSystem::~CollideSystem() {

}

CollideSystem* CollideSystem::CreateInstance(RInt od) {
	return new CollideSystem(od);
}

//系统初始化时调用
void CollideSystem::onAwake() {

}

//系统被激活时调用
void CollideSystem::onEnable() {

}

//系统开始运行时调用
void CollideSystem::onStart() {

}

//实体被添加到系统时调用
void CollideSystem::onAddGameObject() {

}

//组件添加到实体时调用
void CollideSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {

}

//组件从实体移除时调用
void CollideSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {

}

//实体从系统中移除时调用
void CollideSystem::onRemoveGameObject() {

}

//系统更新时调用
void CollideSystem::onUpdate() {

}

//系统更新时onUpdate之后调用
void CollideSystem::onLateUpdate() {

}

//系统被禁用时调用
void CollideSystem::onDisable() {

}

//系统被注销时调用
void CollideSystem::onDestroy() {

}