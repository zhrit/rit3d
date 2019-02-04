#include "pch.h"
#include "CRender.h"


CRender::CRender() {
	m_type = COMPTYPE::RENDER;
}


CRender::~CRender() {

}

CRender* CRender::CreateInstance() {
	return new CRender();
}

void CRender::setMesh(Mesh* _mesh) {
	m_mesh = _mesh;
}

void CRender::setMaterial(Material* _mat) {
	m_mat = _mat;
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
