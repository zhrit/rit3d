#include "pch.h"
#include "CollideSystem.h"
#include "RGameObject.h"
#include "CTransform.h"
#include "RMath.h"

CollideSystem::CollideSystem(RInt od) : ISystem(od) {
	m_type = COLLIDESYSTEM;
}

CollideSystem::~CollideSystem() {

}

CollideSystem* CollideSystem::CreateInstance(RInt od) {
	return new CollideSystem(od);
}

//系统初始化时调用
void CollideSystem::onAwake() {
	//0-sphere,1-box
	m_intersectionMethod[0][0] = _intersectionTest_sphere2sphere;
	m_intersectionMethod[0][1] = _intersectionTest_sphere2box;
	m_intersectionMethod[1][0] = _intersectionTest_box2sphere;
	m_intersectionMethod[1][1] = _intersectionTest_box2box;
}

//系统被激活时调用
void CollideSystem::onEnable() {

}

//系统开始运行时调用
void CollideSystem::onStart() {

}

//实体被添加到系统时调用
void CollideSystem::onAddGameObject() {

}

//组件添加到实体时调用
void CollideSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::SPHERECOLLIDER || type == COMPTYPE::BOXCOLLIDER) {
		std::vector<CCollider*>::iterator iter;
		iter = std::find(m_colliderPool.begin(), m_colliderPool.end(), (CCollider*)pComp);
		if (iter == m_colliderPool.end()) {
			m_colliderPool.push_back((CCollider*)pComp);
		}
	}
}

//组件从实体移除时调用
void CollideSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::SPHERECOLLIDER || type == COMPTYPE::BOXCOLLIDER) {
		std::vector<CCollider*>::iterator iter;
		iter = std::find(m_colliderPool.begin(), m_colliderPool.end(), (CCollider*)pComp);
		if (iter != m_colliderPool.end()) {
			m_colliderPool.erase(iter);
		}
	}
}

//实体从系统中移除时调用
void CollideSystem::onRemoveGameObject() {

}

//系统更新时调用
void CollideSystem::onUpdate() {
	static int counts_intersect1 = 0, counts_intersect2 = 0, counts_calc1 = 0, counts_calc2 = 0;
	int count_intersect1 = 0, count_intersect2 = 0, count_calc1 = 0, count_calc2 = 0;
	//计算所有碰撞组件在世界坐标系下的几何信息
	calcGeomInfoInWorld();
	std::vector<CCollider*>::size_type i, j;
	std::vector<CCollider*>::size_type l = m_colliderPool.size();
	for (i = 0; i < l; i++) {
		for (j = i + 1; j < l; j++) {
			count_calc1++;
			if (_intersectionTest(m_colliderPool[i]->wBV, m_colliderPool[j]->wBV)) {
				count_intersect1++;
			}
		}
	}

	//构建BVH树
	_buildBVHTree();
	//每个碰撞组件和BVH树做碰撞检测
	for (i = 0; i < l; i++) {
		std::queue<BVHNode*> qNode;
		qNode.push(m_BHVTree);
		CCollider* sc = m_colliderPool[i];
		IBV* bv = sc->wBV;
		while (!qNode.empty()) {
			BVHNode* currNode = qNode.front();
			qNode.pop();
			//左子节点
			if (currNode->pLeft) {
				if (currNode->pLeft->nodeType == BVHNODETYPE::NODE) {
					//不是叶子节点
					count_calc2++;
					if (_intersectionTest(bv, currNode->pLeft->bv)) {
						qNode.push(currNode->pLeft);
					}
				}
				else {
					//是叶子节点
					if (currNode->pLeft->pc == sc) {
						currNode->pLeft->active = false;
					}
					if (currNode->pLeft->active) {
						count_calc2++;
						if (_intersectionTest(bv, currNode->pLeft->bv)) {
							count_intersect2++;
						}
					}
				}
			}
			//右子结点
			if (currNode->pRight) {
				if (currNode->pRight->nodeType == BVHNODETYPE::NODE) {
					//不是叶子节点
					count_calc2++;
					if (_intersectionTest(bv, currNode->pRight->bv)) {
						qNode.push(currNode->pRight);
					}
				}
				else {
					//是叶子节点
					if (currNode->pRight->pc == sc) {
						currNode->pRight->active = false;
					}
					if (currNode->pRight->active) {
						count_calc2++;
						if (_intersectionTest(bv, currNode->pRight->bv)) {
							count_intersect2++;
						}
					}
				}
			}
		}
	}

	if (count_intersect1 != counts_intersect1) {
		counts_intersect1 = count_intersect1;
		cout << "counts_intersect1:" << counts_intersect1 << endl;
	}
	if (count_intersect2 != counts_intersect2) {
		counts_intersect2 = count_intersect2;
		cout << "counts_intersect2:" << counts_intersect2 << endl;
	}
	if (count_calc1 != counts_calc1) {
		counts_calc1 = count_calc1;
		cout << "counts_calc1:" << counts_calc1 << endl;
	}
	if (count_calc2 != counts_calc2) {
		counts_calc2 = count_calc2;
		cout << "counts_calc2:" << counts_calc2 << endl;
	}
}

//系统更新时onUpdate之后调用
void CollideSystem::onLateUpdate() {

}

//系统被禁用时调用
void CollideSystem::onDisable() {

}

//系统被注销时调用
void CollideSystem::onDestroy() {

}

//计算所有碰撞组件在世界坐标系下的几何信息
void CollideSystem::calcGeomInfoInWorld() {
	for (auto c : m_colliderPool) {
		CSphereCollider* sc = (CSphereCollider*)c;
		((SphereBV*)sc->wBV)->c = glm::vec3(sc->gameObject->transform->getModelMatrix() * glm::vec4(sc->center, 1.0f));
		((SphereBV*)sc->wBV)->r = glmp::max(sc->gameObject->transform->getScale()) * sc->radius;
	}
}

//相交检测
RBool CollideSystem::_intersectionTest(IBV* _bv1, IBV* _bv2) {
	return m_intersectionMethod[_bv1->type][_bv2->type](_bv1, _bv2);
}

//相交检测，球和球
RBool CollideSystem::_intersectionTest_sphere2sphere(IBV* _bv1, IBV* _bv2) {
	SphereBV* bv1 = (SphereBV*)_bv1;
	SphereBV* bv2 = (SphereBV*)_bv2;
	glm::vec3 d = bv1->c - bv2->c;
	RFloat dis2 = glm::dot(d, d);
	return dis2 <= (bv1->r + bv2->r) * (bv1->r + bv2->r);
}

//相交检测，球和立方体
RBool CollideSystem::_intersectionTest_sphere2box(IBV* _bv1, IBV* _bv2) {
	return true;
}

//相交检测，立方体和球
RBool CollideSystem::_intersectionTest_box2sphere(IBV* _bv1, IBV* _bv2) {
	return _intersectionTest_sphere2box(_bv2, _bv1);
}

//相交检测，立方体和立方体
RBool CollideSystem::_intersectionTest_box2box(IBV* _bv1, IBV* _bv2) {
	return true;
}

//构建BVH树
void CollideSystem::_buildBVHTree() {
	//删除原来的BVHTree
	_deleteBVHTree(m_BHVTree);
	_buildBVHTreeCore(&m_BHVTree, 0, m_colliderPool.size() - 1);
}
void CollideSystem::_buildBVHTreeCore(BVHNode** tree, int start, int end) {
	int nums = end - start + 1;
	if (nums <= 0) return;//没有节点时退出
	BVHNode* pNode = new BVHNode;
	*tree = pNode;
	glm::vec3 separatDir(0.0f, 0.0f, 0.0f);
	pNode->bv = _computeBoundingVolumn(start, end, separatDir);
	pNode->nums = nums;
	pNode->active = true;

	if (1 == nums) {
		//只剩一个节点时，作为叶子节点，并记录对应的碰撞组件
		pNode->nodeType = BVHNODETYPE::LEAF;
		pNode->pc = m_colliderPool[start];
		pNode->pLeft = nullptr;
		pNode->pRight = nullptr;
	}
	else {
		//剩多个节点时继续划分
		pNode->nodeType = BVHNODETYPE::NODE;
		int k = _partition(start, end, separatDir, ((SphereBV*)pNode->bv)->c);
		_buildBVHTreeCore(&(pNode->pLeft), start, k);
		_buildBVHTreeCore(&(pNode->pRight), k + 1, end);
	}
}

//删除BVH树
void CollideSystem::_deleteBVHTree(BVHNode* pNode) {
	if (nullptr == pNode) return;
	_deleteBVHTree(pNode->pLeft);
	_deleteBVHTree(pNode->pRight);
	SafeDelete(pNode);
}

//计算m_colliderPool中start到end碰撞组件的总包围盒
IBV* CollideSystem::_computeBoundingVolumn(int start, int end, glm::vec3& separating_axis) {
	SphereBV* bv = new SphereBV;
	if (start == end) {
		//只有一个节点
		CCollider* collider = m_colliderPool[start];
		bv->c = ((SphereBV*)collider->wBV)->c;
		bv->r = ((SphereBV*)collider->wBV)->r;
	}
	else {
		//有多个节点
		//所有sphere包围盒的圆心的几何中心点为圆心
		bv->c = glm::vec3(0.0f, 0.0f, 0.0f);
		bv->r = 0.0f;
		for (int i = start; i <= end; i++) {
			CCollider* collider = m_colliderPool[i];
			bv->c += ((SphereBV*)collider->wBV)->c;
		}
		bv->c /= (end - start + 1.0f);
		for (int i = start; i <= end; i++) {
			CCollider* collider = m_colliderPool[i];
			RFloat nr = glm::length(bv->c - ((SphereBV*)collider->wBV)->c) + ((SphereBV*)collider->wBV)->r;
			if (nr > bv->r) {
				bv->r = nr;
				separating_axis = ((SphereBV*)collider->wBV)->c - bv->c;
			}
		}
	}
	return bv;
}

//划分节点
int CollideSystem::_partition(int start, int end, glm::vec3 dir, glm::vec3 point) {
	int i = start, j = end;
	if (j - i < 2) return i;
	while (i < j) {
		while (glm::dot(((SphereBV*)m_colliderPool[i]->wBV)->c - point, dir) >= 0.0f && i < end) {
			i++;
		}
		while (glm::dot(((SphereBV*)m_colliderPool[j]->wBV)->c - point, dir) < 0.0f && j > start) {
			j--;
		}
		if (i < j) {
			CCollider* temp = m_colliderPool[i];
			m_colliderPool[i] = m_colliderPool[j];
			m_colliderPool[j] = temp;
		}
	}
	return j;
}