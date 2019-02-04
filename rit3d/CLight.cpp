#include "pch.h"
#include "CLight.h"
#include "RGameObject.h"
#include "RScene.h"


CLight::CLight() {
	m_type = COMPTYPE::LIGHT;
	//创建fbo
	glGenFramebuffers(1, &m_depthMapFBO);
	//创建纹理
	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


CLight::~CLight() {

}

CLight* CLight::CreateInstance() {
	return new CLight();
}

glm::vec3 CLight::getColor() const {
	return m_color;
}
void CLight::setColor(RFloat r, RFloat g, RFloat b) {
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
}
void CLight::setColor(glm::vec3 _color) {
	m_color = _color;
}

void CLight::setIntensity(RFloat amb, RFloat dif, RFloat spe) {
	m_ambientInt = amb;
	m_diffuseInt = dif;
	m_specularInt = spe;
}

RBool CLight::isCastShadow() const {
	return m_castShadow;
}
void CLight::castShadow(RBool _cast) {
	m_castShadow = _cast;
}

LIGHTTYPE CLight::getLightType() const {
	return m_lightType;
}
void CLight::setLightType(LIGHTTYPE _type) {
	if (m_lightType == _type) {
		return;
	}
	if (this->gameObject == nullptr || this->gameObject->getScene() == nullptr) {
		return;
	}
	this->gameObject->getScene()->resetLightNum(m_lightType, -1);
	m_lightType = _type;
	this->gameObject->getScene()->resetLightNum(m_lightType, 1);
}

void CLight::setAttenuation(RFloat dis, RFloat _kc, RFloat _kl, RFloat _kq) {
	m_distance = dis;
	m_kc = _kc;
	m_kl = _kl;
	m_kq = _kq;
}

void CLight::setCutOff(RFloat inner, RFloat outer) {
	m_cutInner = cos(inner / 57.3f);
	m_cutOuter = cos(outer / 57.3f);
}

RFloat CLight::getAmbInt() const {
	return m_ambientInt;
}
RFloat CLight::getDifInt() const {
	return m_diffuseInt;
}
RFloat CLight::getSpeInt() const {
	return m_specularInt;
}

RFloat CLight::getKc() const {
	return m_kc;
}
RFloat CLight::getKl() const {
	return m_kl;
}
RFloat CLight::getKq() const {
	return m_kq;
}

RFloat CLight::getCutInner() const {
	return m_cutInner;
}
RFloat CLight::getCutOuter() const {
	return m_cutOuter;
}

//阴影相关
RUInt CLight::getFramebuffer() const {
	return m_depthMapFBO;
}
RUInt CLight::getDepthMap() const {
	return m_depthMap;
}
//设置视锥体
RFloat CLight::getNear() const {
	return m_near_s;
}
void CLight::setNear(RFloat _n) {
	if (m_near_s != _n) {
		m_near_s = _n;
		m_projDirty = true;
	}
}

RFloat CLight::getFar() const {
	return m_far_s;
}
void CLight::setFar(RFloat _f) {
	if (m_far_s != _f) {
		m_far_s = _f;
		m_projDirty = true;
	}
}

RFloat CLight::getAsp() const {
	return m_asp_s;
}
void CLight::setAsp(RFloat _a) {
	if (m_asp_s != _a) {
		m_asp_s = _a;
		m_projDirty = true;
	}
}

RFloat CLight::getFov() const {
	return m_fov_s;
}
void CLight::setFov(RFloat _f) {
	if (m_fov_s != _f) {
		m_fov_s = _f;
		m_projDirty = true;
	}
}

RFloat CLight::getSize() const {
	return m_size_s;
}
void CLight::setSize(RFloat _s) {
	if (m_size_s != _s) {
		m_size_s = _s;
		m_projDirty = true;
	}
}

void CLight::setOrthoFrustum(RFloat fov, RFloat asp, RFloat near, RFloat far) {
	if (m_fov_s != fov || m_asp_s != asp || m_near_s != near || m_far_s != far) {
		m_fov_s = fov;
		m_asp_s = asp;
		m_near_s = near;
		m_far_s = far;
		m_projDirty = true;
	}
}
void CLight::setPerspFrustum(RFloat size, RFloat asp, RFloat near, RFloat far) {
	if (m_size_s != size || m_asp_s != asp || m_near_s != near || m_far_s != far) {
		m_size_s = size;
		m_asp_s = asp;
		m_near_s = near;
		m_far_s = far;
		m_projDirty = true;
	}
}

//获取视图投影矩阵
glm::mat4 CLight::getLightSpaceMatrix() {
	return _getProjMatrix() * _getViewMatrix();
}

//获取投影矩阵
glm::mat4 CLight::_getProjMatrix() {
	if (m_projDirty) {
		if (m_lightType == LIGHTTYPE::DIRECTION) {
			float hw = m_size_s / 2;
			float hh = hw / m_asp_s;
			m_projMatrix = glm::ortho(-hw, hw, -hh, hh, m_near_s, m_far_s);
		}
		else if (m_lightType == LIGHTTYPE::POINT) {
			m_projMatrix = glm::perspective(glm::radians(m_fov_s), m_asp_s, m_near_s, m_far_s);
		}
		else {

		}
	}
	return m_projMatrix;
}

//获取视图矩阵
glm::mat4 CLight::_getViewMatrix() {
	return gameObject->transform->getViewMatrrix();
}
