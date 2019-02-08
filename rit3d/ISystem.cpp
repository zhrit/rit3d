#include "pch.h"
#include "ISystem.h"

ISystem::ISystem(RInt od) {
	////不允许直接new一个实例出来
	//if (!ISystem::CreateEnable) {
	//	cout << "Create an instance of a system is not allowed!" << endl;
	//}
	//ISystem::CreateEnable = false;

	this->order = od;
}
ISystem::~ISystem() {
	
}

//RBool ISystem::CreateEnable = false;
//存放所有系统的创建方法，用来实现反射机制
std::map<SYSTEMTYPE, std::function<ISystem*(RUInt)>> ISystem::systemMap;

//创建系统实例，所有系统实例通过此方法创建
ISystem* ISystem::CreateSystem(SYSTEMTYPE type, RInt od) {
	ISystem* st = ISystem::systemMap[type](od);
	return st;
}

//初始化系统
void ISystem::initSystem() {
	onAwake();
}

//卸载系统
void ISystem::uninitSystem() {

}

//系统更新
void ISystem::update() {
	this->onUpdate();
}

//系统更新2
void ISystem::lateUpdate() {
	this->onLateUpdate();
}

//返回系统是否被激活
RBool ISystem::isEnabled() const {
	return enabled;
}

void ISystem::setEnable(RBool value) {
	if (enabled == value) {
		return;
	}

	enabled = value;

	if (enabled) {
		onEnable();
	}
	else {
		onDisable();
	}
}

//获取系统类型
SYSTEMTYPE ISystem::getType() const {
	return m_type;
}

RBool ISystem::isStarted() const {
	return m_started;
}

void ISystem::setStarted(RBool value) {
	m_started = value;
}
