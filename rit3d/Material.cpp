#include "pch.h"
#include "Material.h"
#include "Application.h"
#include "GLProgram.h"
#include "Texture.h"
#include "ResourceManager.h"

Material::Material(RString _name) : m_name(_name) {

}
Material::~Material() {
	_destroy();
}

Material* Material::Create(RString _name) {
	return new Material(_name);
}

void Material::Destroy(Material* _mat) {
	delete _mat;
	_mat = nullptr;
}

GLProgram* Material::getShader() {
	if (m_shaderDirty) {
		//×Ö·û´®²Ù×÷
		RString str_def = "#version 330 core\n";
		for (auto iter : m_defines) {
			str_def += "#define " + iter.first + " " + iter.second + "\n";
		}
		RString shaderName = str_def + m_shaderName;
		GLProgram* sha = Application::Instance()->resourceMng->getShader(shaderName);
		if (sha == nullptr) {
			m_shader = Application::Instance()->resourceMng->createShader(shaderName, m_shaderName + ".vert", m_shaderName + ".frag", str_def);
		}
		else {
			m_shader = sha;
		}
		m_shaderDirty = false;
	}
	return m_shader;
}

RString Material::getShaderName() const {
	return m_shaderName;
}
void Material::setShader(RString _shader) {
	m_shaderName = _shader;
}

//Ôö¼Ódefine
void Material::addDefine(RString _key, RString _value) {
	if (m_defines.count(_key) > 0 && m_defines[_key] == _value) {
		return;
	}
	m_defines[_key] = _value;
	m_shaderDirty = true;
}
//É¾³ýdefine
void Material::removeDefine(RString _key) {
	std::map<RString, RString>::iterator iter = m_defines.find("_key");
	if (iter != m_defines.end()) {
		m_defines.erase(iter);
	}
}

glm::vec3 Material::getColor() const {
	return m_color;
}
void Material::setColor(RFloat r, RFloat g, RFloat b) {
	m_color = glm::vec3(r, g, b);
}

glm::vec3 Material::getAmbient() const {
	return m_ambient;
}
void Material::setAmbient(RFloat r, RFloat g, RFloat b) {
	m_ambient = glm::vec3(r, g, b);
}

glm::vec3 Material::getDiffuse() const {
	return m_diffuse;
}
void Material::setDiffuse(RFloat r, RFloat g, RFloat b) {
	m_diffuse = glm::vec3(r, g, b);
}

glm::vec3 Material::getSpecular() const {
	return m_specular;
}
void Material::setSpecular(RFloat r, RFloat g, RFloat b) {
	m_specular = glm::vec3(r, g, b);
}

RFloat Material::getShininess() const {
	return m_shininess;
}
void Material::setShininess(RFloat s) {
	m_shininess = s;
}

void Material::addTexture(Texture* _tex) {
	RUInt count = (RUInt)std::count(m_texList.begin(), m_texList.end(), _tex);
	if (count == 0) {
		m_texList.push_back(_tex);
	}
}
void Material::removeTexture(Texture* _tex) {
	m_texList.erase(std::remove(m_texList.begin(), m_texList.end(), _tex), m_texList.end());
}
void Material::clearTexture() {
	m_texList.clear();
}
RUInt Material::getTexNum() const {
	return (RUInt)m_texList.size();
}
Texture* Material::getTexture(RUInt i) {
	if (i < m_texList.size()) {
		return m_texList[i];
	}
	return nullptr;
}

void Material::setNormalMap(Texture* _map) {
	if (m_normalMap == _map) return;
	if (nullptr == m_normalMap) {
		addDefine("NORMALMAP", "1");
	}
	m_normalMap = _map;
}
Texture* Material::getNormalMap() const {
	return m_normalMap;
}
void Material::removeNormalMap() {
	if (nullptr == m_normalMap) return;
	removeDefine("NORMALMAP");
	m_normalMap = nullptr;
}

void Material::_destroy() {

}

RBool Material::isUseLight() const {
	return m_useLight;
}
void Material::useLight() {
	m_useLight = true;
}
void Material::closeLight() {
	m_useLight = false;
}

RFloat Material::getMetallic() const {
	return m_metallic;
}
void Material::setMetallic(RFloat _m) {
	m_metallic = _m;
}

RFloat Material::getRoughness() const {
	return m_roughness;
}
void Material::setRoughness(RFloat _r) {
	m_roughness = _r;
}

