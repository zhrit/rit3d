#include "pch.h"
#include "HairSystem.h"
#include "SCLightCameraCollecter.h"
#include "RGameObject.h"
#include "CTransform.h"

HairSystem::HairSystem(RInt od) : ISystem(od) {
	m_type = HAIRSYSTEM;
}

HairSystem::~HairSystem() {

}

void HairSystem::addHair(CHair* _b) {
	std::vector<CHair*>::iterator iter;
	iter = std::find(m_hairPool.begin(), m_hairPool.end(), _b);
	if (iter == m_hairPool.end()) {
		m_hairPool.push_back(_b);
	}
}

void HairSystem::removeHair(CHair* _b) {
	std::vector<CHair*>::iterator iter;
	iter = std::find(m_hairPool.begin(), m_hairPool.end(), _b);
	if (iter != m_hairPool.end()) {
		m_hairPool.erase(iter);
	}
}

HairSystem* HairSystem::CreateInstance(RInt od) {
	return new HairSystem(od);
}

//系统初始化时调用
void HairSystem::onAwake() {

}

//系统被激活时调用
void HairSystem::onEnable() {

}

//系统开始运行时调用
void HairSystem::onStart() {

}

//实体被添加到系统时调用
void HairSystem::onAddGameObject() {

}

//组件添加到实体时调用
void HairSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::HAIR) {
		addHair((CHair*)pComp);
		_transform((CHair*)pComp);
	}
}

//组件从实体移除时调用
void HairSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::HAIR) {
		removeHair((CHair*)pComp);
	}
}

//实体从系统中移除时调用
void HairSystem::onRemoveGameObject() {

}

//系统更新时调用
void HairSystem::onUpdate(DWORD deltaT) {
	_updateHair(deltaT);
	_drawHair();
}

void HairSystem::_updateHair(DWORD deltaT) {
	RFloat dt = static_cast<RFloat>(deltaT) / 1000.0f;
	//cout << dt << endl;
	if (dt > 1.0f / 60.0f) {
		dt = 1.0f / 60.0f;
	}
	for (auto hair : m_hairPool) {
		glm::mat4 mMat = hair->gameObject->transform->getModelMatrix();
		//verlet数值积分获取新的位置
		for (auto& node : hair->m_nodes) {
			glm::vec3 p2 = node.p1 + hair->d * (node.p1 - node.p0) + hair->a * dt * dt;
			//新状态取代旧状态
			node.p0 = node.p1;
			node.p1 = p2;
		}
		//对每束发丝松弛法进行约束求解
		for (auto strand : hair->m_strands) {
			//松弛法迭代
			for (int i = 0; i < 10; i++) {
				//对每个节点的处理
				for (int index = strand.nodeStart; index < strand.nodeEnd; index++) {
					HairNode& na = hair->m_nodes[index];
					HairNode& nb = hair->m_nodes[index + 1];
					//碰撞检测和决议
					_collideHair(hair, nb.p1);
					//长度约束
					glm::vec3 a2b = nb.p1 - na.p1;
					float l = glm::length(a2b);
					float factor = (l - nb.maxLength) / (2.0f * l);
					na.p1 = na.p1 + a2b * factor;
					nb.p1 = nb.p1 - a2b * factor;
				}
				hair->m_nodes[strand.nodeStart].p1 = glm::vec3(mMat * glm::vec4(strand.rootP, 1.0f));
			}
		}
		//更新绘制数据
		int l = hair->m_drawData.size();
		for (int i = 0; i < l; i++) {
			hair->m_drawData[i] = hair->m_nodes[i].p1;
		}
	}
}
void HairSystem::_drawHair() {
	std::list<CCamera*> cameraList = SCLightCameraCollecter::Instance()->getCameraList();
	for (auto camera : cameraList) {
		for (auto hair : m_hairPool) {
			glBindVertexArray(hair->m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, hair->m_VBO);
			glBufferData(GL_ARRAY_BUFFER, 3 * hair->m_drawData.size() * sizeof(float), &(hair->m_drawData[0]), GL_STREAM_DRAW);
			hair->shader->use();
			hair->shader->setMat4("uModel", hair->gameObject->transform->getModelMatrix());
			hair->shader->setMat4("uView", camera->getViewMatrix());
			hair->shader->setMat4("uProjection", camera->getProjMatrix());
			for (int i = 0; i < static_cast<int>(hair->m_strands.size()); i++) {
				glDrawArrays(GL_LINE_STRIP, i * hair->nodeInStrand, hair->nodeInStrand);
			}
		}
		glBindVertexArray(0);
	}
}

//系统更新时onUpdate之后调用
void HairSystem::onLateUpdate() {

}

//系统被禁用时调用
void HairSystem::onDisable() {

}

void HairSystem::onDestroy() {

}

//头发的碰撞检测
void HairSystem::_collideHair(CHair* ch, glm::vec3& point) {
	glm::vec3 dir = point - ch->center;
	float l = glm::length(dir);
	if (l < ch->raduis) {
		point = ch->center + ch->raduis / l * dir;
	}
}

//节点数据从局部坐标系转换到世界坐标系
void HairSystem::_transform(CHair* ch) {
	glm::mat4 mMat = ch->gameObject->transform->getModelMatrix();
	int l = ch->m_nodes.size();
	for (int i = 0; i < l; i++) {
		ch->m_nodes[i].p0 = ch->m_nodes[i].p1 = glm::vec3(mMat * glm::vec4(ch->m_nodes[i].p1, 1.0f));
	}
	ch->center = glm::vec3(mMat * glm::vec4(ch->center, 1.0f));
}