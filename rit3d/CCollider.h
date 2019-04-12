#pragma once
#include "RCommon.h"
#include "IComponent.h"

//包围盒类型
typedef enum {
	SPHEREBV = 0,
	BOXBV,
} BVTYPE;

//interface of bounding volume
class IBV {
public:
	BVTYPE type;
	IBV() {}
};

//sphere bounding volume
class SphereBV : public IBV {
public:
	glm::vec3 c{ 0.0f, 0.0f, 0.0f };
	RFloat r{ 0.0f };

	SphereBV() {
		type = BVTYPE::SPHEREBV;
	}
};

//碰撞组建基类
class CCollider : public IComponent {
public:
	CCollider();
	virtual ~CCollider();

	//世界坐标系下的碰撞盒，引擎内部计算用得数据结构。对外不可见，对外可见得只有上面得属性。
	IBV* wBV{ nullptr };
};

//球碰撞组件
class CSphereCollider : public CCollider {
private:
	CSphereCollider();
	virtual ~CSphereCollider();

public:
	static CSphereCollider* CreateInstance();

	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//局部坐标系中心
	RFloat radius{ 0.5f };//局部坐标系半径
};

//立方体碰撞组件
class CBoxCollider : public CCollider {
private:
	CBoxCollider();
	virtual ~CBoxCollider();

public:
	static CBoxCollider* CreateInstance();

	glm::vec3 center{ 0.0f, 0.0f, 0.0f };//局部坐标系中心
};

