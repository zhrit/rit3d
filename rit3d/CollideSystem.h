#pragma once
#include "RCommon.h"
#include "ISystem.h"
#include "CCollider.h"

//BVH树节点类型，叶子节点和非叶子节点
enum BVHNODETYPE {
	NODE,
	LEAF,
};
//sphere bounding volume
struct BV {
	glm::vec3 center;
	RFloat radius;
};
//BVH树节点
struct BVHNode {
	BV bv;//节点对应的包围盒
	BVHNODETYPE nodeType;//节点类型
	int nums;//包含的碰撞组件的数量
	CCollider* pc;//节点所对应的碰撞组件
	RBool active;//节点是否活跃（是否需要进行碰撞检测）
	BVHNode* pLeft;
	BVHNode* pRight;
};

class CollideSystem : public ISystem {
private:
	CollideSystem(RInt od);
	virtual ~CollideSystem();

	std::vector<CCollider*> m_colliderPool;
	std::function<RBool(CCollider*, CCollider*)> m_intersectionMethod[2][2];
public:
	static CollideSystem* CreateInstance(RInt od);

	//系统初始化时调用
	virtual void onAwake();

	//系统被激活时调用
	virtual void onEnable();

	//系统开始运行时调用
	virtual void onStart();

	//实体被添加到系统时调用
	virtual void onAddGameObject();

	//组件添加到实体时调用
	virtual void onAddComponent(COMPTYPE type, IComponent* pComp);

	//组件从实体移除时调用
	virtual void onRemoveComponent(COMPTYPE type, IComponent* pComp);

	//实体从系统中移除时调用
	virtual void onRemoveGameObject();

	//系统更新时调用
	virtual void onUpdate();

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate();

	//系统被禁用时调用
	virtual void onDisable();

	//系统被注销时调用
	virtual void onDestroy();

private:
	//相交检测
	RBool _intersectionTest(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_sphere2sphere(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_sphere2box(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_box2sphere(CCollider* c1, CCollider* c2);
	static RBool _intersectionTest_box2box(CCollider* c1, CCollider* c2);

	//计算所有碰撞组件在世界坐标系下的几何信息
	void calcGeomInfoInWorld();

	//----关于BVH树-----
	//BVH树
	BVHNode* m_BHVTree{ nullptr };
	//构建BVH树
	void _buildBVHTree();
	void _buildBVHTreeCore(BVHNode** tree, int start, int end);
	//删除BVH树
	void _deleteBVHTree(BVHNode* pNode);
	//计算m_colliderPool中start到end碰撞组件的总包围盒,并返回一个分割轴的方向
	BV _computeBoundingVolumn(int start, int end, glm::vec3& separating_axis);
	//划分节点
	int _partition(int start, int end, glm::vec3 dir, glm::vec3 point);
};

