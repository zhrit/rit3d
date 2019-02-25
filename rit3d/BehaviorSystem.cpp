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

//窗口大小改变时执行
void BehaviorSystem::onChangeSize(int _w, int _h) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onChangeSize(_w, _h);
	}
}

//鼠标事件
void BehaviorSystem::onLeftButtonDown() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onLeftButtonDown();
	}
}
void BehaviorSystem::onLeftButtonUp() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onLeftButtonUp();
	}
}
void BehaviorSystem::onRightButtonDown() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onRightButtonDown();
	}
}
void BehaviorSystem::onRightButtonUp() {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onRightButtonUp();
	}
}
void BehaviorSystem::onMouseMove(double _x, double _y) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onMouseMove(_x, _y);
	}
}
void BehaviorSystem::onScroll(double _x, double _y) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onScroll(_x, _y);
	}
}
//键盘事件
void BehaviorSystem::onKeyDown(int key) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onKeyDown(key);
	}
}
void BehaviorSystem::onKeyKeep(int key) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onKeyKeep(key);
	}
}
void BehaviorSystem::onKeyUp(int key) {
	for (auto iter : m_behaviorPool) {
		iter->getBehavior()->onKeyUp(key);
	}
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
