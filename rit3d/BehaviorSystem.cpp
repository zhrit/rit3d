#include "pch.h"
#include "BehaviorSystem.h"
#include "CBehavior.h"
#include "BaseBehavior.h"

BehaviorSystem::BehaviorSystem(RInt od) : ISystem(od) {
	m_type = BEHAVIORSYSTEM;
}
BehaviorSystem::~BehaviorSystem() {

}

BehaviorSystem* BehaviorSystem::CreateInstance(RInt od) {
	return new BehaviorSystem(od);
}

//系统初始化时调用
void BehaviorSystem::onAwake() {

}

//系统被激活时调用
void BehaviorSystem::onEnable() {

}

//系统开始运行时调用
void BehaviorSystem::onStart() {

}

//实体被添加到系统时调用
void BehaviorSystem::onAddGameObject() {

}

//组件添加到实体时调用
void BehaviorSystem::onAddComponent() {

}

//组件从实体移除时调用
void BehaviorSystem::onRemoveComponent() {

}

//实体从系统中移除时调用
void BehaviorSystem::onRemoveGameObject() {

}

//系统更新时调用
void BehaviorSystem::onUpdate() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->update();
	}
}

//系统更新时onUpdate之后调用
void BehaviorSystem::onLateUpdate() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->lateUpdate();
	}
}

//系统被禁用时调用
void BehaviorSystem::onDisable() {

}

//系统被注销时调用
void BehaviorSystem::onDestroy() {

}

void BehaviorSystem::addBehavior(CBehavior* _b) {
	std::vector<CBehavior*>::iterator iter;
	iter = std::find(m_behaviorPool.begin(), m_behaviorPool.end(), _b);
	if (iter == m_behaviorPool.end()) {
		m_behaviorPool.push_back(_b);
	}
}

void BehaviorSystem::removeBehavior(CBehavior* _b) {
	std::vector<CBehavior*>::iterator iter;
	iter = std::find(m_behaviorPool.begin(), m_behaviorPool.end(), _b);
	if (iter != m_behaviorPool.end()) {
		m_behaviorPool.erase(iter);
	}
}