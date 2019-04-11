#include "pch.h"
#include "CollideSystem.h"
#include "RGameObject.h"
#include "CTransform.h"
#include "RMath.h"

CollideSystem::CollideSystem(RInt od) : ISystem(od) {
	m_type = COLLIDESYSTEM;
}

CollideSystem::~CollideSystem() {

}

CollideSystem* CollideSystem::CreateInstance(RInt od) {
	return new CollideSystem(od);
}

//系统初始化时调用
void CollideSystem::onAwake() {
	//0-sphere,1-box
	m_intersectionMethod[0][0] = _intersectionTest_sphere2sphere;
	m_intersectionMethod[0][1] = _intersectionTest_sphere2box;
	m_intersectionMethod[1][0] = _intersectionTest_box2sphere;
	m_intersectionMethod[1][1] = _intersectionTest_box2box;
}

//系统被激活时调用
void CollideSystem::onEnable() {

}

//系统开始运行时调用
void CollideSystem::onStart() {

}

//实体被添加到系统时调用
void CollideSystem::onAddGameObject() {

}

//组件添加到实体时调用
void CollideSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::SPHERECOLLIDER || type == COMPTYPE::BOXCOLLIDER) {
		std::vector<CCollider*>::iterator iter;
		iter = std::find(m_colliderPool.begin(), m_colliderPool.end(), (CCollider*)pComp);
		if (iter == m_colliderPool.end()) {
			m_colliderPool.push_back((CCollider*)pComp);
		}
	}
}

//组件从实体移除时调用
void CollideSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::SPHERECOLLIDER || type == COMPTYPE::BOXCOLLIDER) {
		std::vector<CCollider*>::iterator iter;
		iter = std::find(m_colliderPool.begin(), m_colliderPool.end(), (CCollider*)pComp);
		if (iter != m_colliderPool.end()) {
			m_colliderPool.erase(iter);
		}
	}
}

//实体从系统中移除时调用
void CollideSystem::onRemoveGameObject() {

}

//系统更新时调用
void CollideSystem::onUpdate() {
	std::vector<CCollider*>::size_type i, j;
	std::vector<CCollider*>::size_type l = m_colliderPool.size();
	for (i = 0; i < l; i++) {
		for (j = i + 1; j < l; j++) {
			if (_intersectionTest(m_colliderPool[i], m_colliderPool[j])) {
				//todo
			}
		}
	}
}

//系统更新时onUpdate之后调用
void CollideSystem::onLateUpdate() {

}

//系统被禁用时调用
void CollideSystem::onDisable() {

}

//系统被注销时调用
void CollideSystem::onDestroy() {

}

//相交检测
RBool CollideSystem::_intersectionTest(CCollider* c1, CCollider* c2) {
	return m_intersectionMethod[c1->getType() - SPHERECOLLIDER][c2->getType() - SPHERECOLLIDER](c1, c2);
}

//相交检测，球和球
RBool CollideSystem::_intersectionTest_sphere2sphere(CCollider* c1, CCollider* c2) {
	CSphereCollider* collider1 = (CSphereCollider*)c1;
	CSphereCollider* collider2 = (CSphereCollider*)c2;
	glm::vec3 center1 = glm::vec3(collider1->gameObject->transform->getModelMatrix() * glm::vec4(collider1->center, 1.0f));
	glm::vec3 center2 = glm::vec3(collider2->gameObject->transform->getModelMatrix() * glm::vec4(collider2->center, 1.0f));
	RFloat radius1 = glmp::max(collider1->gameObject->transform->getScale()) * collider1->radius;
	RFloat radius2 = glmp::max(collider2->gameObject->transform->getScale()) * collider2->radius;
	glm::vec3 d = center1 - center2;
	RFloat dis2 = glm::dot(d, d);
	return dis2 <= (radius1 + radius2) * (radius1 + radius2);
}

//相交检测，球和立方体
RBool CollideSystem::_intersectionTest_sphere2box(CCollider* c1, CCollider* c2) {
	return true;
}

//相交检测，立方体和球
RBool CollideSystem::_intersectionTest_box2sphere(CCollider* c1, CCollider* c2) {
	return _intersectionTest_sphere2box(c2, c1);
}

//相交检测，立方体和立方体
RBool CollideSystem::_intersectionTest_box2box(CCollider* c1, CCollider* c2) {
	return true;
}