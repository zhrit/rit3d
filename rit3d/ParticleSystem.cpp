#include "pch.h"
#include "ParticleSystem.h"
#include "CParticle.h"
#include "RGameObject.h"
#include "CTransform.h"
#include "RMath.h"
#include "GLProgram.h"
#include "CCamera.h"
#include "SCLightCameraCollecter.h"

ParticleSystem::ParticleSystem(RInt od) : ISystem(od) {
	m_type = PARTICLESYSTEM;
}
ParticleSystem::~ParticleSystem() {

}

ParticleSystem* ParticleSystem::CreateInstance(RInt od) {
	return new ParticleSystem(od);
}

//系统初始化时调用
void ParticleSystem::onAwake() {
	RFloat vertices[] = {
		0.0f, 0.0f, 0.0f
	};
	//创建attay buffer
	RUInt vbo;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &vbo);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

//系统被激活时调用
void ParticleSystem::onEnable() {

}

//系统开始运行时调用
void ParticleSystem::onStart() {

}

//实体被添加到系统时调用
void ParticleSystem::onAddGameObject() {

}

//组件添加到实体时调用
void ParticleSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::PARTICLE) {
		addParticle((CParticle*)pComp);
	}
}

//组件从实体移除时调用
void ParticleSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::PARTICLE) {
		removeParticle((CParticle*)pComp);
	}
}

//实体从系统中移除时调用
void ParticleSystem::onRemoveGameObject() {

}

//系统更新时调用
void ParticleSystem::onUpdate(DWORD deltaT) {
	_updateParticles(deltaT);
	_drawParticles();
}

//系统更新时onUpdate之后调用
void ParticleSystem::onLateUpdate() {

}

//系统被禁用时调用
void ParticleSystem::onDisable() {

}

//系统被注销时调用
void ParticleSystem::onDestroy() {

}

void ParticleSystem::addParticle(CParticle* _b) {
	std::vector<CParticle*>::iterator iter;
	iter = std::find(m_particlePool.begin(), m_particlePool.end(), _b);
	if (iter == m_particlePool.end()) {
		m_particlePool.push_back(_b);
	}
}

void ParticleSystem::removeParticle(CParticle* _b) {
	std::vector<CParticle*>::iterator iter;
	iter = std::find(m_particlePool.begin(), m_particlePool.end(), _b);
	if (iter != m_particlePool.end()) {
		m_particlePool.erase(iter);
	}
}

//更新粒子信息
void ParticleSystem::_updateParticles(DWORD deltaT) {
	//更新粒子信息
	for (auto cp : m_particlePool) {
		for (auto& particle : cp->particles) {
			if (particle.life > 0.0f) {
				particle.life -= deltaT / 1000.0f;
				particle.position += particle.velocity * (cp->startSpeed * deltaT / 1000.0f);
			}
		}

		//增加新的粒子
		//计算新增粒子数量
		RInt addNums = static_cast<int>((cp->cumulativeTime + deltaT) / 1000.0f * cp->rateOverTime);
		cp->cumulativeTime = cp->cumulativeTime + deltaT - addNums / cp->rateOverTime * 1000.0f;
		for (int i = 0; i < addNums; i++) {
			int j = _findUnusedParticle(cp);
			if (j == -1) {
				Particle par;
				par.life = cp->lifeTime;
				par.position = glm::vec3(0.0f, 0.0f, 0.0f);
				par.velocity = glmp::randomVec3();
				cp->particles.push_back(par);
			}
			else {
				cp->particles[j].life = cp->lifeTime;
				cp->particles[j].position = glm::vec3(0.0f, 0.0f, 0.0f);
				cp->particles[j].velocity = glmp::randomVec3();
			}
		}
	}
}
//渲染粒子
void ParticleSystem::_drawParticles() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArray(m_VAO);
	std::list<CCamera*> cameraList = SCLightCameraCollecter::Instance()->getCameraList();
	for (auto camera : cameraList) {
		for (auto cp : m_particlePool) {
			cp->shader->use();
			//texture
			cp->spirit->use(1);
			cp->shader->setInt("uSpirit", 1);
			cp->shader->setMat4("uModel", cp->gameObject->transform->getModelMatrix());
			cp->shader->setMat4("uView", camera->getViewMatrix());
			cp->shader->setMat4("uProjection", camera->getProjMatrix());
			for (auto par : cp->particles) {
				if (par.life > 0.0f) {
					cp->shader->setVec3("uOffset", par.position);
					glDrawArrays(GL_POINTS, 0, 1);
				}
			}
		}
	}
	glBindVertexArray(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

//寻找一个没用的粒子
RInt ParticleSystem::_findUnusedParticle(CParticle* cp) {
	for (RInt j = cp->lastUsedParticle; j < (int)cp->particles.size(); j++) {
		if (cp->particles[j].life <= 0.0f) {
			cp->lastUsedParticle = j;
			return j;
		}
	}
	for (RInt j = 0; j < cp->lastUsedParticle; j++) {
		if (cp->particles[j].life <= 0.0f) {
			cp->lastUsedParticle = j;
			return j;
		}
	}
	return -1;
}
