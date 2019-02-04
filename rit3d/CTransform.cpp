#include "pch.h"
#include "CTransform.h"
#include "RGameObject.h"


CTransform::CTransform() {
	m_type = COMPTYPE::TRANFORM;
}


CTransform::~CTransform() {

}

CTransform* CTransform::CreateInstance() {
	return new CTransform();
}

//设置可用状态
void CTransform::setEnabled(RBool value) {
	//强制可用，屏蔽此方法
}

//设置父对象
CTransform* CTransform::getParent() const {
	return parent;
}
RBool CTransform::setParent(CTransform* p) {
	if (parent == p) {
		return true;
	}
	else {
		if (p == nullptr || p->gameObject->getScene() == gameObject->getScene()) {
			parent = p;
			_setDirty();//全局模型矩阵标脏
			return true;
		}
		else {
			return false;
		}
	}
}

//局部获取几何信息
glm::vec3 CTransform::getLocalPosition() const {
	return m_localPosition;
}
glm::vec3 CTransform::getLocalAngle() const {
	return m_localAngle;
}
glm::vec3 CTransform::getLocalScale() const {
	return m_localScale;
}
glm::vec3 CTransform::getLocalUpDir() const {
	return m_localUpDir;
}
glm::vec3 CTransform::getLocalFrontDir() const {
	return m_localFrontDir;
}

//局部设置几何信息
void CTransform::setLocalPosition(float x, float y, float z) {
	if (m_localPosition.x == x && m_localPosition.y == y && m_localPosition.z == z) {
		return;
	}
	m_localPosition.x = x, m_localPosition.y = y, m_localPosition.z = z;
	_setLocalDirty();
	m_viewDirty = true;
}
void CTransform::setLocalAngle(float x, float y, float z) {
	if (m_localAngle.x == x && m_localAngle.y == y && m_localAngle.z == z) {
		return;
	}
	m_localAngle.x = x, m_localAngle.y = y, m_localAngle.z = z;
	_setLocalDirty();
	m_viewDirty = true;
}
void CTransform::setLocalScale(float x, float y, float z) {
	if (m_localScale.x == x && m_localScale.y == y && m_localScale.z == z) {
		return;
	}
	m_localScale.x = x, m_localScale.y = y, m_localScale.z = z;
	_setLocalDirty();
}
void CTransform::setLocalUpDir(float x, float y, float z) {
	if (m_localUpDir.x == x && m_localUpDir.y == y && m_localUpDir.z == z) {
		return;
	}
	m_localUpDir.x = x, m_localUpDir.y = y, m_localUpDir.z = z;
	m_localUpDir = glm::normalize(m_localUpDir);
	_setLocalDirty();
	m_viewDirty = true;
}
void CTransform::setLocalFrontDir(float x, float y, float z) {
	if (m_localFrontDir.x == x && m_localFrontDir.y == y && m_localFrontDir.z == z) {
		return;
	}
	m_localFrontDir.x = x, m_localFrontDir.y = y, m_localFrontDir.z = z;
	m_localFrontDir = glm::normalize(m_localFrontDir);
	_setLocalDirty();
	m_viewDirty = true;
}

//全局获取几何信息
glm::vec3 CTransform::getPosition() {
	//todo
	return m_localPosition;
}
glm::vec3 CTransform::getAngle() {
	//todo
	return m_localAngle;
}
glm::vec3 CTransform::getScale() {
	//todo
	return m_localScale;
}

//局部获取模型矩阵
glm::mat4 CTransform::getLocalModelMatrix() {
	//先缩放，再旋转Y，X，Z，再平移
	if (m_localModelDirty) {
		//位移
		m_localModelMatrix = glm::translate(m_localModelMatrix, m_localPosition);
		//m_localModelMatrix = glm::rotate(m_localModelMatrix, glm::radians(m_localAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//m_localModelMatrix = glm::rotate(m_localModelMatrix, glm::radians(m_localAngle.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//m_localModelMatrix = glm::rotate(m_localModelMatrix, glm::radians(m_localAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
		//旋转
		glm::vec3 right = glm::normalize(glm::cross(m_localUpDir, m_localFrontDir));
		glm::vec3 up = glm::normalize(glm::cross(m_localFrontDir, right));
		glm::vec3 front = m_localFrontDir;

		glm::mat4 trans(
			right.x, right.y, right.z, 0.0f,
			up.x, up.y, up.z, 0.0f,
			front.x, front.y, front.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
		m_localModelMatrix = m_localModelMatrix * trans;
		//缩放
		m_localModelMatrix = glm::scale(m_localModelMatrix, m_localScale);

		m_localModelDirty = false;
	}
	return m_localModelMatrix;
}
//全局获取模型矩阵
glm::mat4 CTransform::getModelMatrix() {
	if (m_modelDirty) {
		getLocalModelMatrix();
		if (parent != nullptr) {
			m_modelMatrix = parent->getModelMatrix() * m_localModelMatrix;
		}
		else {
			m_modelMatrix = m_localModelMatrix;
		}
		m_modelDirty = false;
	}
	return m_modelMatrix;
}

//局部模型矩阵标脏
void CTransform::_setLocalDirty() {
	//自身局部标脏
	m_localModelDirty = true;
	//孩子全局标脏
	for (auto it : children) {
		it->_setDirty();
	}
}
//全局模型矩阵标脏
void CTransform::_setDirty() {
	//自身全局标脏
	m_modelDirty = true;
	//孩子全局标脏
	for (auto it : children) {
		it->_setDirty();
	}
}

//获取视图矩阵
glm::mat4 CTransform::getViewMatrrix() {
	if (m_viewDirty) {
		m_viewMatrix = glm::lookAt(m_localPosition, m_localPosition + m_localFrontDir, m_localUpDir);
		m_viewDirty = false;
	}
	return m_viewMatrix;
}
