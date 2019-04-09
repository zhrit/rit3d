#include "pch.h"
#include "CRender.h"
#include "Mesh.h"
#include "Material.h"


CRender::CRender() {
	m_type = COMPTYPE::RENDER;
}


CRender::~CRender() {

}

CRender* CRender::CreateInstance() {
	return new CRender();
}

void CRender::addMesh(Mesh* _mesh) {
	m_meshs.push_back(_mesh);
}

void CRender::addMaterial(Material* _mat) {
	m_mats.push_back(_mat);
}

RBool CRender::isCastShadow() const {
	return m_castShadow;
}
void CRender::castShadow(RBool _cast) {
	m_castShadow = _cast;
}

RBool CRender::isRecieveShadow() const {
	return m_recieveShadow;
}
void CRender::recieveShadow(RBool _recieve) {
	m_recieveShadow = _recieve;
}
