#pragma once
#include "RCommon.h"
#include "IComponent.h"

class CCollider : public IComponent {
public:
	CCollider();
	virtual ~CCollider();
};

class CSphereCollider : public CCollider {
private:
	CSphereCollider();
	virtual ~CSphereCollider();

public:
	static CSphereCollider* CreateInstance();

	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//局部坐标系中心
	RFloat radius{ 0.5f };//半径
};

class CBoxCollider : public CCollider {
private:
	CBoxCollider();
	virtual ~CBoxCollider();

public:
	static CBoxCollider* CreateInstance();

	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//局部坐标系中心
};

