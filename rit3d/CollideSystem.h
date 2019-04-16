#pragma once
#include "RCommon.h"
#include "ISystem.h"
#include "CCollider.h"

//碰撞检测策略 Collision detection strategy
enum CDT {
	TRAVERSAL = 1,//暴力遍历法
	BVH = 2,//层次包围盒
	OCTREE = 4,//八叉树
	BST = 8,//二分空间划分树
};

//BVH树节点类型，叶子节点和非叶子节点
enum BVHNODETYPE {
	NODE,
	LEAF,
};

//Octree节点
class OctreeNode {
public:
	glm::vec3 center;
	RFloat halfWidth;
	OctreeNode* pChild[8]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	std::list<CCollider*> cList;
};

const int MAX_OCTREE_DEPTH = 5;

//BVH树节点
class BVHNode {
public:
	BVHNode() {};
	~BVHNode() {
		if (bv) {
			SafeDelete(bv);
		}
	};
	IBV* bv;//节点对应的包围盒,创建节点时，必须重新new一个bv出来，不能把碰撞组件中的IBV指针直接赋值
	BVHNODETYPE nodeType;//节点类型
	int nums;//包含的碰撞组件的数量
	CCollider* pc;//节点所对应的碰撞组件
	RBool active;//节点是否活跃（是否需要进行碰撞检测）
	BVHNode* pLeft;
	BVHNode* pRight;
	int offset{ -1 };
};

class CollideSystem : public ISystem {
private:
	CollideSystem(RInt od);
	virtual ~CollideSystem();

	std::vector<CCollider*> m_colliderPool;
	std::function<RBool(IBV*, IBV*)> m_intersectionMethod[2][2];
	int m_cdt{ CDT::TRAVERSAL };
	int m_cc1, m_ci1, m_cc2, m_ci2, m_cc3, m_ci3;
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

	//设置碰撞检测策略
	void setCollisionDetectionStrategy(int _cdt);

private:
	//相交检测
	RBool _intersectionTest(IBV* _bv1, IBV* _bv2);
	static RBool _intersectionTest_sphere2sphere(IBV* _bv1, IBV* _bv2);
	static RBool _intersectionTest_sphere2box(IBV* _bv1, IBV* _bv2);
	static RBool _intersectionTest_box2sphere(IBV* _bv1, IBV* _bv2);
	static RBool _intersectionTest_box2box(IBV* _bv1, IBV* _bv2);

	//计算所有碰撞组件在世界坐标系下的几何信息
	void calcGeomInfoInWorld();

	//----关于BVH树-----
	//BVH树
	BVHNode* m_BHVTree{ nullptr };
	std::vector<BVHNode*> m_BHVArray;

	//构建BVH树
	void _buildBVHTree();
	void _buildBVHTreeCore(BVHNode** tree, int start, int end);
	void _tree2array();
	//删除BVH树
	void _deleteBVHTree(BVHNode* pNode);
	//计算m_colliderPool中start到end碰撞组件的总包围盒,并返回一个分割轴的方向
	IBV* _computeBoundingVolumn(int start, int end, glm::vec3& separating_axis);
	//划分节点
	int _partition(int start, int end, glm::vec3 dir, glm::vec3 point);

	//----关于八叉树-----
	RInt m_octreeDepth{ 2 };//树深度
	OctreeNode* m_octree{ nullptr };//八叉树

	//构建octree
	void _buildOctree();
	OctreeNode* _buildOctreeCore(glm::vec3 center, RFloat halfWidth, int stopDepth);
	//在octree中插入碰撞组件
	void _insertCollider(OctreeNode* pTree, CCollider* pC);
	//删除octree
	void _deleteOctree(OctreeNode* pNode);
	//octree中对象碰撞检测
	void _testAllCollisionsInOctree(OctreeNode* pTree);

	//----缓存----
	//std::map<CCollider*, std::map<CCollider*, RBool>> m_cache;
};

