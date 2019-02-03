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
