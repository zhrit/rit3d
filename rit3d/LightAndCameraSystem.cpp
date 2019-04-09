#include "pch.h"
#include "LightAndCameraSystem.h"
#include "SCLightCameraCollecter.h"

LightAndCameraSystem::LightAndCameraSystem(RInt od) : ISystem(od) {
	m_type = LIGHTANDCAMERA;
}

LightAndCameraSystem::~LightAndCameraSystem() {

}

LightAndCameraSystem* LightAndCameraSystem::CreateInstance(RInt od) {
	return new LightAndCameraSystem(od);
}

//系统初始化时调用
void LightAndCameraSystem::onAwake() {

}

//系统被激活时调用
void LightAndCameraSystem::onEnable() {

}

//系统开始运行时调用
void LightAndCameraSystem::onStart() {

}

//实体被添加到系统时调用
void LightAndCameraSystem::onAddGameObject() {

}

//组件添加到实体时调用
void LightAndCameraSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::LIGHT) {
		SCLightCameraCollecter::Instance()->addLight((CLight*)pComp);
	}
	if (type == COMPTYPE::CAMERA) {
		SCLightCameraCollecter::Instance()->addCamera((CCamera*)pComp);
	}
}

//组件从实体移除时调用
void LightAndCameraSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::LIGHT) {
		SCLightCameraCollecter::Instance()->removeLight((CLight*)pComp);
	}
	if (type == COMPTYPE::CAMERA) {
		SCLightCameraCollecter::Instance()->removeCamera((CCamera*)pComp);
	}
}

//实体从系统中移除时调用
void LightAndCameraSystem::onRemoveGameObject() {

}

//系统更新时调用
void LightAndCameraSystem::onUpdate() {

}

//系统更新时onUpdate之后调用
void LightAndCameraSystem::onLateUpdate() {

}

//系统被禁用时调用
void LightAndCameraSystem::onDisable() {

}

//系统被注销时调用
void LightAndCameraSystem::onDestroy() {

}
