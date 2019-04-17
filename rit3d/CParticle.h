#pragma once
#include "RCommon.h"
#include "IComponent.h"
#include "Texture.h"
#include "GLProgram.h"

enum EMISSIONTYPE {
	ET_CONSECUTIVE,
	ET_PILSING,
};

struct Particle {
	glm::vec3 position;//位置偏移
	glm::vec3 velocity;//速度方向 
	RFloat size;//大小
	RFloat rotation;//旋转
	RFloat brightness;//亮度
	RFloat life;//剩余存活时间

	Particle() : position(0.0f), velocity(0.0f), size(1.0f), rotation(0.0f), life(0.0f) {}
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
	glm::vec3 accelerat{ 0.0f, 0.0f, 0.0f };//加速度
	RFloat lifeTime{ 5.0f };//粒子存活时间
	RFloat startSize{ 0.0f };//发射时粒子大小
	RFloat endSize{ 1.0f };//粒子消亡时大小
	RFloat rateOverTime{ 5.0f };//单位时间生成粒子的数量，连续发射
	RInt rateOnce{ 10 };//一次脉冲发射的数量，脉冲发射
	EMISSIONTYPE emissionType{ ET_CONSECUTIVE };//发射器类型

	//----渲染相关----
	Texture* spirit;//粒子的纹理
	GLProgram* shader;//渲染用的着色器

	//----粒子池----
	std::vector<Particle> particles;

	//----辅助数据----
	RFloat cumulativeTime{ 0.0f };//发射器没有生成新粒子的累计时间,单位ms
	RInt lastUsedParticle{ 0 };//辅助寻找没用的粒子
	int latencyTime{ -1 };//粒子发射等待时间,单位ms

	//----粒子生成和更新函数----
	std::function<void(CParticle*, Particle&)> emissionFunc;//生成粒子的函数
	std::function<void(CParticle*, Particle&, DWORD)> updateFunc;//更新粒子的函数
};