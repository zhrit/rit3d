#include "pch.h"
#include "CCamera.h"
#include "RGameObject.h"


CCamera::CCamera() {
	m_type = COMPTYPE::CAMERA;

	//创建framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	//生成纹理
	glGenTextures(1, &m_colorTex);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//纹理附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);

	//创建渲染缓冲对象作为深度缓冲和模板缓冲
	RUInt rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//打印错误信息
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	//恢复默认framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


CCamera::~CCamera() {
	glDeleteFramebuffers(1, &m_framebuffer);
	glDeleteTextures(1, &m_colorTex);
}

CCamera* CCamera::CreateInstance() {
	return new CCamera();
};

RUInt CCamera::getCullMask() const {
	return m_cullMask;
}
void CCamera::setCullMask(RUInt _mask) {
	m_cullMask = _mask;
}

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

//获取帧缓冲对象
RUInt CCamera::getFramebuffer() {
	return m_framebuffer;
}
//获取帧缓冲上的颜色纹理对象
RUInt CCamera::getColorTex() {
	return m_colorTex;
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
