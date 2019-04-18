#include "pch.h"
#include "SystemManager.h"
#include "IComponent.h"
#include "CTransform.h"
#include "CRender.h"
#include "CCamera.h"
#include "CLight.h"
#include "CPostProcess.h"
#include "CSkybox.h"
#include "CBehavior.h"
#include "CCollider.h"
#include "CParticle.h"
#include "CHair.h"
#include "SCLightCameraCollecter.h"
#include "RenderSystem.h"
#include "BehaviorSystem.h"
#include "CollideSystem.h"
#include "ParticleSystem.h"
#include "HairSystem.h"
#include "LightAndCameraSystem.h"
#include "DebugSystem.h"

/*
 * 系统管理单例类
 */
SystemManager::SystemManager() {
	//注册系统创建函数
	registSystemCreateFunc();
	//注册组件创建函数
	registCompCreateFunc();
	//初始化必要的单例组件
	registSingltonComp();
	//清空系统列表
	m_systemList.clear();
};
SystemManager::~SystemManager() {};

SystemManager* SystemManager::m_instance = nullptr;

SystemManager* SystemManager::Instance() {
	if (nullptr == m_instance) {
		m_instance = new SystemManager();
	}
	return m_instance;
}

//注册各个系统的创建函数，系统只能通过创建函数创建，不能直接new
void SystemManager::registSystemCreateFunc() {
	ISystem::systemMap.clear();
	ISystem::systemMap[STARTSYSTEM] = StartSystem::CreateInstance;
	ISystem::systemMap[RENDERSYSTEM] = RenderSystem::CreateInstance;
	ISystem::systemMap[BEHAVIORSYSTEM] = BehaviorSystem::CreateInstance;
	ISystem::systemMap[COLLIDESYSTEM] = CollideSystem::CreateInstance;
	ISystem::systemMap[LIGHTANDCAMERA] = LightAndCameraSystem::CreateInstance;
	ISystem::systemMap[DEBUGSYSTEM] = DebugSystem::CreateInstance;
	ISystem::systemMap[PARTICLESYSTEM] = ParticleSystem::CreateInstance;
	ISystem::systemMap[HAIRSYSTEM] = HairSystem::CreateInstance;
}

//注册各个组件的创建函数，组件只能通过创建函数创建，不能直接new
void SystemManager::registCompCreateFunc() {
	IComponent::compMap.clear();
	IComponent::compMap[BEHAVIOR] = CBehavior::CreateInstance;
	IComponent::compMap[TRANFORM] = CTransform::CreateInstance;
	IComponent::compMap[RENDER] = CRender::CreateInstance;
	IComponent::compMap[CAMERA] = CCamera::CreateInstance;
	IComponent::compMap[LIGHT] = CLight::CreateInstance;
	IComponent::compMap[POSTPROCESS] = CPostProcess::CreateInstance;
	IComponent::compMap[SKYBOX] = CSkybox::CreateInstance;
	IComponent::compMap[SPHERECOLLIDER] = CSphereCollider::CreateInstance;
	IComponent::compMap[BOXCOLLIDER] = CBoxCollider::CreateInstance;
	IComponent::compMap[PARTICLE] = CParticle::CreateInstance;
	IComponent::compMap[HAIR] = CHair::CreateInstance;
}

//初始化必要的单例组件
void SystemManager::registSingltonComp() {
	SCLightCameraCollecter::Instance();//灯光相机收集组件
}

//为程序注册一个制定的系统
void SystemManager::registSystem(SYSTEMTYPE type, RInt od) {
	if (hasSystem(type)) {
		cout << "system has been registed!" << endl;
		return;
	}
	ISystem* st = ISystem::CreateSystem(type, od);
	//根据od插入到制定位置
	std::list<ISystem*>::iterator it = m_systemList.begin();
	for (; it != m_systemList.end(); it++) {
		auto pItem = *it;
		if (pItem->order > od) {
			m_systemList.insert(it, st);
			break;
		}
	}
	if (it == m_systemList.end()) {
		m_systemList.push_back(st);
	}
	//记录系统的运行时间
	m_elapsedTime.insert(std::map<SYSTEMTYPE, DWORD>::value_type(type, 0));
	st->initSystem();
}

//系统更新入口
void SystemManager::update() {
	static DWORD lastTime = ::GetTickCount();
	DWORD nowTime = ::GetTickCount();
	DWORD deltaT = nowTime - lastTime;
	lastTime = nowTime;
	//onStart()
	for (auto st : m_systemList) {
		if (st->isEnabled() && !st->isStarted()) {
			st->setStarted(true);
			st->onStart();
		}
	}
	//update
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			DWORD t1 = ::GetTickCount();
			st->update(deltaT);
			m_elapsedTime[st->getType()] += ::GetTickCount() - t1;
		}
	}
	//lateUpdate
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->lateUpdate();
		}
	}

	if (clock_display.isInterval()) {
		for (auto& iter : m_elapsedTime) {
			cout << "system_" << iter.first << ": " << iter.second << "\n";
			iter.second = 0;
		}
		cout << endl;
	}
}

//判断时候已经注册某个系统
RBool SystemManager::hasSystem(SYSTEMTYPE type) {
	for (auto it : m_systemList) {
		if (it->getType() == type) {
			return true;
		}
	}
	return false;
}

//从程序已注册的全部系统中获取一个指定的系统
ISystem* SystemManager::getSystem(SYSTEMTYPE type) {
	for (auto it : m_systemList) {
		if (it->getType() == type) {
			return it;
		}
	}
	return nullptr;
}

//窗口大小变化时回调函数
void SystemManager::onChangeSize(int _w, int _h) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onChangeSize(_w, _h);
		}
	}
}

//鼠标事件
void SystemManager::onLeftButtonDown() {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onLeftButtonDown();
		}
	}
}
void SystemManager::onLeftButtonUp() {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onLeftButtonUp();
		}
	}
}
//void SystemManager::onLeftButtonTap() {}
void SystemManager::onRightButtonDown() {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onRightButtonDown();
		}
	}
}
void SystemManager::onRightButtonUp() {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onRightButtonUp();
		}
	}
}
//void SystemManager::onRightButtonTap() {}
void SystemManager::onMouseMove(double _x, double _y) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onMouseMove(_x, _y);
		}
	}
}
void SystemManager::onScroll(double _x, double _y) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onScroll(_x, _y);
		}
	}
}

//键盘事件
void SystemManager::onKeyDown(int key) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onKeyDown(key);
		}
	}
}
void SystemManager::onKeyKeep(int key) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onKeyKeep(key);
		}
	}
}
void SystemManager::onKeyUp(int key) {
	for (auto st : m_systemList) {
		if (st->isEnabled()) {
			st->onKeyUp(key);
		}
	}
}

//添加新的组件时调用
void SystemManager::onAddComponent(COMPTYPE type, IComponent* pComp) {
	for (auto st : m_systemList) {
		st->onAddComponent(type, pComp);
	}
}
//移除组件时调用
void SystemManager::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	for (auto st : m_systemList) {
		st->onRemoveComponent(type, pComp);
	}
}

//设置碰撞检测策略
void SystemManager::setCollisionDetectionStrategy(int _cdt) {
	CollideSystem* cs = (CollideSystem*)getSystem(SYSTEMTYPE::COLLIDESYSTEM);
	if (cs) {
		cs->setCollisionDetectionStrategy(_cdt);
	}
}