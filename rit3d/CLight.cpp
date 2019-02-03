#include "pch.h"
#include "CLight.h"
#include "RGameObject.h"
#include "RScene.h"


CLight::CLight() {
	m_type = COMPTYPE::LIGHT;
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
	m_cutInner = inner / 57.3f;
	m_cutOuter = outer / 57.3f;
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
