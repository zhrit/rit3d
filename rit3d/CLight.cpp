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
}


CLight::~CLight() {
	glDeleteFramebuffers(1, &m_depthMapFBO);
	glDeleteTextures(1, &m_depthMap);
}

CLight* CLight::CreateInstance() {
	return new CLight();
}

glm::vec3 CLight::getColor() const {
	return m_color;
}
void CLight::setColor(RFloat r, RFloat g, RFloat b) {
	m_color = glm::vec3(r, g, b);
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
	if (_type == LIGHTTYPE::NONE) {
		cout << "光源类型不能设置为NONE" << endl;
		return;
	}
	if (m_lightType == _type) {
		return;
	}
	if (this->gameObject == nullptr || this->gameObject->getScene() == nullptr) {
		return;
	}
	this->gameObject->getScene()->resetLightNum(m_lightType, -1);
	m_lightType = _type;
	this->gameObject->getScene()->resetLightNum(m_lightType, 1);

	m_projDirty = true;//投影矩阵标脏

	switch (m_lightType) {
	case LIGHTTYPE::DIRECTION:
	case LIGHTTYPE::SPOT:
	{
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		RFloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//上面是为了防止采样过多，阴影贴图纹理不重复，贴图外的都设置成1.0，也就是没有阴影

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		break;
	}
	case LIGHTTYPE::LPOINT:
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthMap);
		for (RUInt i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		break;
	}
	default:
		break;
	}
}

//重写setEnabled
void CLight::setEnabled(RBool value) {
	if (m_enabled == value) {
		return;
	}
	m_enabled = value;
	if (m_enabled) {
		this->gameObject->getScene()->resetLightNum(m_lightType, 1);
	}
	else {
		this->gameObject->getScene()->resetLightNum(m_lightType, -1);
	}
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

RFloat CLight::getFov() const {
	return m_fov_s;
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

void CLight::setPerspFrustum(RFloat _near, RFloat _far) {
	if (m_near_s != _near || m_far_s != _far) {
		m_near_s = _near;
		m_far_s = _far;
		m_projDirty = true;
	}
}
void CLight::setOrthoFrustum(RFloat _size, RFloat _near, RFloat _far) {
	if (m_size_s != _size || m_near_s != _near || m_far_s != _far) {
		m_size_s = _size;
		m_near_s = _near;
		m_far_s = _far;
		m_projDirty = true;
	}
}

//获取视图投影矩阵（平行光）
glm::mat4 CLight::getLightSpaceMatrix() {
	return _getProjMatrix() * _getViewMatrix();
}

//获取视图投影矩阵（点光源）
std::vector<glm::mat4> CLight::getLightSpaceMatrixs() {
	std::vector<glm::mat4> lightSpaceMatrixs;
	glm::mat4 proj = _getProjMatrix();
	std::vector<glm::mat4> views = _getViewMatrixs();
	for(int i = 0; i < 6; i++) {
		lightSpaceMatrixs.push_back(proj * views[i]);
	}
	return lightSpaceMatrixs;
}

//获取投影矩阵
glm::mat4 CLight::_getProjMatrix() {
	if (m_projDirty) {
		if (m_lightType == LIGHTTYPE::DIRECTION) {
			float hw = m_size_s / 2;
			float hh = hw / m_asp_s;
			m_projMatrix = glm::ortho(-hw, hw, -hh, hh, m_near_s, m_far_s);
		}
		else if (m_lightType == LIGHTTYPE::LPOINT) {
			m_projMatrix = glm::perspective(glm::radians(m_fov_s), m_asp_s, m_near_s, m_far_s);
		}
		else {
			m_projMatrix = glm::perspective(2 * acos(m_cutOuter), m_asp_s, m_near_s, m_far_s);
		}
		m_projDirty = false;
	}
	return m_projMatrix;
}

//获取视图矩阵
glm::mat4 CLight::_getViewMatrix() {
	return gameObject->transform->getViewMatrix();
}

//获取视图矩阵(点光源)
std::vector<glm::mat4> CLight::_getViewMatrixs() {
	return gameObject->transform->getViewMatrixs();
}
