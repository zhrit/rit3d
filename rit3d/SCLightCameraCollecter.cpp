#include "pch.h"
#include "SCLightCameraCollecter.h"


SCLightCameraCollecter::SCLightCameraCollecter() {
	m_type = COMPTYPE::LIGHTCAMERACOLLECTER;
}


SCLightCameraCollecter::~SCLightCameraCollecter() {

}

SCLightCameraCollecter* SCLightCameraCollecter::m_instance = nullptr;

SCLightCameraCollecter* SCLightCameraCollecter::Instance() {
	if (nullptr == m_instance) {
		m_instance = new SCLightCameraCollecter();
	}
	return m_instance;
}

//添加相机组件
void SCLightCameraCollecter::addCamera(CCamera* camera) {
	std::list<CCamera*>::iterator iter;
	iter = std::find(m_cameraList.begin(), m_cameraList.end(), camera);
	if (iter != m_cameraList.end()) {
		return;
	}
	m_cameraList.push_back(camera);
}
//移除相机组件
void SCLightCameraCollecter::removeCamera(CCamera* camera) {
	std::list<CCamera*>::iterator iter;
	iter = std::find(m_cameraList.begin(), m_cameraList.end(), camera);
	if (iter == m_cameraList.end()) {
		return;
	}
	m_cameraList.remove(camera);
}
//获取相机列表
std::list<CCamera*> SCLightCameraCollecter::getCameraList() const {
	return m_cameraList;
}

//添加灯光组件
void SCLightCameraCollecter::addLight(CLight* light) {
	std::list<CLight*>::iterator iter;
	iter = std::find(m_lightList.begin(), m_lightList.end(), light);
	if (iter != m_lightList.end()) {
		return;
	}
	m_lightList.push_back(light);
	if (light->isEnabled()) {
		LIGHTTYPE t = light->getLightType();
		switch (t)
		{
		case LIGHTTYPE::DIRECTION:
			m_dirLightNum++;
			break;
		case LIGHTTYPE::LPOINT:
			m_poiLightNum++;
			break;
		case LIGHTTYPE::SPOT:
			m_spoLightNum++;
			break;
		default:
			break;
		}
	}
}
//移除灯光组件
void SCLightCameraCollecter::removeLight(CLight* light) {
	std::list<CLight*>::iterator iter;
	iter = std::find(m_lightList.begin(), m_lightList.end(), light);
	if (iter == m_lightList.end()) {
		return;
	}
	m_lightList.remove(light);
	LIGHTTYPE t = light->getLightType();
	switch (t)
	{
	case LIGHTTYPE::DIRECTION:
		m_dirLightNum--;
		break;
	case LIGHTTYPE::LPOINT:
		m_poiLightNum--;
		break;
	case LIGHTTYPE::SPOT:
		m_spoLightNum--;
		break;
	default:
		break;
	}
}
//获取灯光列表
std::list<CLight*> SCLightCameraCollecter::getLightList() const {
	return m_lightList;
}
//获取光源数量
RUInt SCLightCameraCollecter::getLightNum(LIGHTTYPE _type) const {
	switch (_type)
	{
	case LIGHTTYPE::DIRECTION:
		return m_dirLightNum;
	case LIGHTTYPE::LPOINT:
		return m_poiLightNum;
	case LIGHTTYPE::SPOT:
		return m_spoLightNum;
	default:
		return 0;
	}
}

//修正光源数量
void SCLightCameraCollecter::resetLightNum(LIGHTTYPE _type, RInt delta) {
	switch (_type)
	{
	case LIGHTTYPE::DIRECTION:
		m_dirLightNum += delta;
		break;
	case LIGHTTYPE::LPOINT:
		m_poiLightNum += delta;
		break;
	case LIGHTTYPE::SPOT:
		m_spoLightNum += delta;
		break;
	default:
		break;
	}
}

