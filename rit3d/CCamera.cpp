#include "pch.h"
#include "CCamera.h"
#include "RGameObject.h"


CCamera::CCamera() {
	m_type = COMPTYPE::CAMERA;
}


CCamera::~CCamera() {

}

CCamera* CCamera::CreateInstance() {
	return new CCamera();
};

CAMERATYPE CCamera::getCameraType() const {
	return m_cameraType;
}
void CCamera::setCameraType(CAMERATYPE type) {
	if (m_cameraType != type) {
		m_cameraType = type;
		m_projDirty = true;
	}
}

//设置视锥体
RFloat CCamera::getNear() const {
	return m_near;
}
void CCamera::setNear(RFloat _n) {
	if (m_near != _n) {
		m_near = _n;
		m_projDirty = true;
	}
}

RFloat CCamera::getFar() const {
	return m_far;
}
void CCamera::setFar(RFloat _f) {
	if (m_far != _f) {
		m_far = _f;
		m_projDirty = true;
	}
}

RFloat CCamera::getAsp() const {
	return m_asp;
}
void CCamera::setAsp(RFloat _a) {
	if (m_asp != _a) {
		m_asp = _a;
		m_projDirty = true;
	}
}

RFloat CCamera::getFov() const {
	return m_fov;
}
void CCamera::setFov(RFloat _f) {
	if (m_fov != _f) {
		m_fov = _f;
		m_projDirty = true;
	}
}

RFloat CCamera::getSize() const {
	return m_size;
}
void CCamera::setSize(RFloat _s) {
	if (m_size != _s) {
		m_size = _s;
		m_projDirty = true;
	}
}

void CCamera::setOrthoFrustum(RFloat _fov, RFloat _asp, RFloat _near, RFloat _far) {
	if (m_fov != _fov || m_asp != _asp || m_near != _near || m_far != _far) {
		m_fov = _fov;
		m_asp = _asp;
		m_near = _near;
		m_far = _far;
		m_projDirty = true;
	}
}
void CCamera::setPerspFrustum(RFloat _size, RFloat _asp, RFloat _near, RFloat _far) {
	if (m_size != _size || m_asp != _asp || m_near != _near || m_far != _far) {
		m_size = _size;
		m_asp = _asp;
		m_near = _near;
		m_far = _far;
		m_projDirty = true;
	}
}

//获取投影矩阵
glm::mat4 CCamera::getProjMatrix() {
	if (m_projDirty) {
		if (m_cameraType == CAMERATYPE::ORTHO) {
			float hw = m_size / 2;
			float hh = hw / m_asp;
			m_projMatrix = glm::ortho(-hw, hw, -hh, hh, m_near, m_far);
		}
		else if (m_cameraType == CAMERATYPE::PERSPECTIVE) {
			m_projMatrix = glm::perspective(glm::radians(m_fov), m_asp, m_near, m_far);
		}
		else {

		}
	}
	return m_projMatrix;
}

//获取视图矩阵
glm::mat4 CCamera::getViewMatrix() {
	return gameObject->transform->getViewMatrix();
}
