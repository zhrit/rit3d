#include "pch.h"
#include "RGameObject.h"
#include "RScene.h"
#include "CTransform.h"
#include "Application.h"
#include "SystemManager.h"
#include "BehaviorSystem.h"


RGameObject::RGameObject(RScene* s, RString n, RString t, LAYER l) :
	m_sce(s), name(n), tag(t), m_layer(l) {
	IComponent* pComp = addComponent(COMPTYPE::TRANFORM);
	transform = (CTransform*)pComp;
}


RGameObject::~RGameObject() {
	_destroy();
}

//销毁对象
void RGameObject::_destroy() {
	//释放所用组件的空间
	removeAllComponent();
}

//获取图层
LAYER RGameObject::getLayer() const {
	return m_layer;
}
//设置图层
void RGameObject::setLayer(LAYER _layer) {
	m_layer = _layer;
}

//添加组件
IComponent* RGameObject::addComponent(COMPTYPE type) {
	if (m_compMap.find(type) != m_compMap.end()) {
		return m_compMap[type];
	}
	IComponent* pComp = IComponent::CreateComponent(type);
	pComp->gameObject = this;

	//std::pair<std::map<COMPTYPE, IComponent*>::iterator, bool> Insert_Pair;
	//Insert_Pair = 

	m_compMap.insert(std::map<COMPTYPE, IComponent*>::value_type(type, pComp));

	Application::Instance()->systemMng->onAddComponent(type, pComp);

	return pComp;
}

//移除一个特定组件
void RGameObject::removeComponent(COMPTYPE type) {
	auto iter = m_compMap.find(type);
	if (iter != m_compMap.end()) {
		Application::Instance()->systemMng->onRemoveComponent(type, iter->second);
		SafeDelete(iter->second);
		m_compMap.erase(iter);
	}
}

//移除所有特定组件
void RGameObject::removeComponents(COMPTYPE type) {
	removeComponent(type);
}

//移除所有组件
void RGameObject::removeAllComponent() {
	for (auto iter = m_compMap.begin(); iter != m_compMap.end(); iter++) {
		removeComponent(iter->first);
	}
	m_compMap.clear();
}

//获取一个特定组件
IComponent* RGameObject::getComponent(COMPTYPE type) {
	auto iter = m_compMap.find(type);
	if (iter != m_compMap.end()) {
		return iter->second;
	}
	return nullptr;
}

//获取所有特定组件
std::list<IComponent*> RGameObject::getComponents(COMPTYPE type) {
	std::list<IComponent*> l;
	return l;
}

//获取所有组件
std::list<IComponent*> RGameObject::getAllComponent() {
	std::list<IComponent*> l;
	return l;
}

//是否激活，不仅看自己是否激活，还要看祖辈是否激活
RBool RGameObject::isActive() {
	if (m_activeDirty) {
		CTransform* parent = transform->parent;

		//如果父辈没有激活，子辈必定没有激活
		if (parent == nullptr || parent->gameObject->isActive()) {
			m_active = m_activeSelf;
		}
		else {
			m_active = false;
		}

		m_activeDirty = false;
	}

	return m_active;
}

//获取自身激活状态
RBool RGameObject::getActiveSelf() const {
	return m_activeSelf;
}

//设置激活状态，仅仅设置自己的激活状态
void RGameObject::setActiveSelf(RBool value) {
	if (m_activeSelf == value) {
		return;
	}

	CTransform* parent = transform->parent;
	if (parent == nullptr || parent->gameObject->isActive()) {
		m_activeSelf = value;
	}
	else {
		m_activeSelf = value;
	}
}

//激活状态标脏
void RGameObject::_setActiveDirty() {
	m_activeDirty = true;

	for (auto it : transform->children) {
		it->gameObject->_setActiveDirty();
	}
}

//获取对象所属场景
RScene* RGameObject::getScene() const {
	return m_sce;
}

