#include "pch.h"
#include "SceneManager.h"

SceneManager* SceneManager::Instance() {
	if (m_instance == nullptr) {
		m_instance = new SceneManager();
	}
	return m_instance;
}

SceneManager* SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
	m_sceneList.clear();
}


SceneManager::~SceneManager()
{
}

RScene* SceneManager::createScene(RString name) {
	RScene* scn = RScene::CreateScene(name);
	m_sceneList.push_back(scn);
	return scn;
}

RScene* SceneManager::getSceneByName(RString name) {
	for (auto it : m_sceneList) {
		if (it->getName() == name) {
			return it;
		}
	}
	return nullptr;
}

void SceneManager::destroySceneByName(RString name) {
	for (auto it : m_sceneList) {
		if (it->getName() == name) {
			it->destroy();
		}
	}
}

std::list<RScene*> SceneManager::getAllScene() {
	return m_sceneList;
}

RUInt SceneManager::getSceneCount() {
	return m_sceneList.size();
}


