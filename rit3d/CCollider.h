#pragma once
#include "RCommon.h"
#include "IComponent.h"

class CSphereCollider : public IComponent {
private:
	CSphereCollider();
	virtual ~CSphereCollider();

public:
	static CSphereCollider* CreateInstance();

	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//局部坐标系中心
	RFloat radius;//半径
};

