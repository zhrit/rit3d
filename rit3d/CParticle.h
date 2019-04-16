#pragma once
#include "RCommon.h"
#include "IComponent.h"
#include "Texture.h"
#include "GLProgram.h"

enum EMISSIONSHAP {
	ES_SHPERE,
	ES_DIR,
	ES_CONE,
	ES_PLAN,
};

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	RFloat life;

	Particle() : position(0.0f), velocity(0.0f), life(0.0f) {}
};

class CParticle : public IComponent {
private:
	CParticle();
	~CParticle();

public:
	static CParticle* CreateInstance();

public:
	//----发射器相关----
	RFloat duration{ 1.0f };//粒子发射周期
	RBool loop{ true };//是否循环发射
	RFloat startSpeed{ 1.0f };//发射时的初速度
	RFloat lifeTime{ 1.0f };//粒子存活时间
	RFloat startSize{ 1.0f };//发射时粒子大小
	RFloat rateOverTime{ 5.0f };//单位时间生成粒子的数量
	EMISSIONSHAP emissionShap{ ES_SHPERE };//发射器类型

	//----渲染相关----
	Texture* spirit;//粒子的纹理
	GLProgram* shader;//渲染用的着色器

	//----粒子池----
	std::vector<Particle> particles;

	//----辅助数据----
	RFloat cumulativeTime{ 0.0f };//发射器没有生成新粒子的累计时间,单位ms
	RInt lastUsedParticle{ 0 };//辅助寻找没用的粒子
};