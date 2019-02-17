#include "pch.h"
#include "BaseBehavior.h"

BaseBehavior::BaseBehavior() {

}
BaseBehavior::~BaseBehavior() {

}

//创建组件后的下一帧执行
void BaseBehavior::onStart() {

}

//每帧执行
void BaseBehavior::onUpdate() {

}

//每帧执行，在onUpdate之后
void BaseBehavior::onLateUpdate() {

}

//碰撞时执行
void BaseBehavior::onCollide() {

}

void BaseBehavior::update() {
	if (!m_started) {
		onStart();
		m_started = true;
	}
	onUpdate();
}
void BaseBehavior::lateUpdate() {
	onLateUpdate();
}

