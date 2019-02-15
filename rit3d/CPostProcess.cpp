#include "pch.h"
#include "CPostProcess.h"
#include "Application.h"
#include "GLProgram.h"
#include "ResourceManager.h"

CPostProcess::CPostProcess() {
	m_shader = Application::Instance()->resourceMng->getShader("postProcess");
}
CPostProcess::~CPostProcess() {

}

//ÉèÖÃshaderÂ·¾¶
RString CPostProcess::getShaderPath() const {
	return m_shaderPath;
}
void CPostProcess::setShaderPath(const RString& _path) {
	m_shaderPath = _path;
	m_shader = Application::Instance()->resourceMng->getShader(_path);
	if (nullptr == m_shader) {
		m_shader = Application::Instance()->resourceMng->createShader(_path, "postProcess.vert", _path + ".frag", "", "");
	}
}

CPostProcess* CPostProcess::CreateInstance() {
	return new CPostProcess();
}

GLProgram* CPostProcess::getShader() const {
	return m_shader;
}