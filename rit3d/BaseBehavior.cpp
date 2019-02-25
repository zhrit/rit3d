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

//窗口大小改变时执行
void BaseBehavior::onChangeSize(int _w, int _h) {
	
}//鼠标事件
void BaseBehavior::onLeftButtonDown() {}
void BaseBehavior::onLeftButtonUp() {}
void BaseBehavior::onRightButtonDown() {}
void BaseBehavior::onRightButtonUp() {}
void BaseBehavior::onMouseMove(double _x, double _y) {}
void BaseBehavior::onScroll(double _x, double _y) {}
//键盘事件
void BaseBehavior::onKeyDown(int key) {}
void BaseBehavior::onKeyKeep(int key) {}
void BaseBehavior::onKeyUp(int key) {}


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

