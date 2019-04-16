#include "pch.h"
#include "CollideSystem.h"
#include "RGameObject.h"
#include "CTransform.h"
#include "RMath.h"

CollideSystem::CollideSystem(RInt od) : ISystem(od) {
	m_type = COLLIDESYSTEM;
}

CollideSystem::~CollideSystem() {
	_deleteBVHTree(m_BHVTree);
	_deleteOctree(m_octree);
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
	//计算所有碰撞组件在世界坐标系下的几何信息
	calcGeomInfoInWorld();
	if(m_cdt & CDT::TRAVERSAL) {
		m_cc1 = m_ci1 = 0;
		std::vector<CCollider*>::size_type i, j;
		std::vector<CCollider*>::size_type l = m_colliderPool.size();
		for (i = 0; i < l; i++) {
			for (j = i + 1; j < l; j++) {
				m_cc1++;
				if (_intersectionTest(m_colliderPool[i]->wBV, m_colliderPool[j]->wBV)) {
					m_ci1++;
				}
			}
		}
	}
	if(m_cdt & CDT::BVH) {
		m_cc2 = m_ci2 = 0;
		std::vector<CCollider*>::size_type i;
		std::vector<CCollider*>::size_type l = m_colliderPool.size();
		//构建BVH树
		_buildBVHTree();
		//每个碰撞组件和BVH树做碰撞检测
		for (i = 0; i < l; i++) {
			CCollider* sc = m_colliderPool[i];
			IBV* bv = sc->wBV;
			std::vector<BVHNode*>::size_type k = 1;
			while (k < m_BHVArray.size()) {
				BVHNode* curr = m_BHVArray[k];
				if (curr->nodeType == BVHNODETYPE::LEAF) {
					//是叶子节点
					if (curr->pc == sc) {
						curr->active = false;
					}
					if (curr->active) {
						m_cc2++;
						if (_intersectionTest(bv, curr->bv)) {
							m_ci2++;
						}
					}
				}
				else {
					//不是叶子节点
					m_cc2++;
					if (!_intersectionTest(bv, curr->bv)) {
						while (curr->nodeType == BVHNODETYPE::NODE) {
							k = curr->offset;
							curr = m_BHVArray[k];
						}
					}
				}
				k++;
			}

		}
	}

	if (m_cdt & CDT::OCTREE) {
		m_cc3 = m_ci3 = 0;
		//构建Octree
		_buildOctree();
		//所有对象的碰撞检测
		_testAllCollisionsInOctree(m_octree);
	}

	if (m_cdt & CDT::BST) {

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

//设置碰撞检测策略
void CollideSystem::setCollisionDetectionStrategy(int _cdt) {
	if (m_cdt == _cdt) return;
	m_cdt = _cdt;
}

//计算所有碰撞组件在世界坐标系下的几何信息
void CollideSystem::calcGeomInfoInWorld() {
	for (auto c : m_colliderPool) {
		CSphereCollider* sc = (CSphereCollider*)c;
		glm::vec3 cw = glm::vec3(sc->gameObject->transform->getModelMatrix() * glm::vec4(sc->center, 1.0f));
		RFloat rw = glmp::max(sc->gameObject->transform->getScale()) * sc->radius;
		if (sc->wBV && ((SphereBV*)sc->wBV)->c == cw && ((SphereBV*)sc->wBV)->r == rw) {
			sc->dirty = false;
		}
		else {
			((SphereBV*)sc->wBV)->c = cw;
			((SphereBV*)sc->wBV)->r = rw;
			sc->dirty = true;
		}
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
	_tree2array();
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
void CollideSystem::_tree2array() {
	m_BHVArray.clear();
	std::stack<BVHNode*> s;
	BVHNode* p = m_BHVTree;
	while (p != nullptr || !s.empty()) {
		if (p != nullptr) {
			m_BHVArray.push_back(p);
			s.push(p);
			p = p->pLeft;
		}
		else {
			p = s.top();
			s.pop();
			if (p->nodeType == BVHNODETYPE::NODE) {
				p->offset = m_BHVArray.size();
			}
			p = p->pRight;
		}
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

//构建octree
void CollideSystem::_buildOctree() {
	//清除原有的树
	//_deleteOctree(m_octree);
	//如果octree为空，则建立一颗octree
	int l = m_colliderPool.size();
	if (l == 0) return;
	if (nullptr == m_octree) {
		//计算octree根节点的中心和半径
		glm::vec3 c = glm::vec3(0.0f, 0.0f, 0.0f);//根节点中心
		RFloat hw = 0.0f;//根节点立方体宽度的一半
		for (int i = 0; i < l; i++) {
			c += ((SphereBV*)m_colliderPool[i]->wBV)->c;
		}
		c /= l;
		for (int i = 0; i < l; i++) {
			CCollider* collider = m_colliderPool[i];
			glm::vec3 deltaC = glm::abs(c - ((SphereBV*)collider->wBV)->c) + ((SphereBV*)collider->wBV)->r;
			RFloat nhw = glmp::max(deltaC);
			if (nhw > hw) hw = nhw;
		}
		//初始化一颗指定深度的octree
		m_octree = _buildOctreeCore(c, hw, m_octreeDepth);
	}

	//在octree中插入碰撞组件
	for (int i = 0; i < l; i++) {
		if (m_colliderPool[i]->dirty) {
			_insertCollider(m_octree, m_colliderPool[i]);
		}
	}
}

OctreeNode* CollideSystem::_buildOctreeCore(glm::vec3 center, RFloat halfWidth, int stopDepth) {
	if (stopDepth <= 0) return nullptr;

	OctreeNode* pNode = new OctreeNode;
	pNode->center = center;
	pNode->halfWidth = halfWidth;

	glm::vec3 offset;
	RFloat step = halfWidth * 0.5f;
	for (int i = 0; i < 8; i++) {
		offset.x = ((i & 1) ? step : -step);
		offset.y = ((i & 2) ? step : -step);
		offset.z = ((i & 4) ? step : -step);
		pNode->pChild[i] = _buildOctreeCore(center + offset, step, stopDepth - 1);
	}
	/*
	挂限号   i(2)   i(10)
	I       111    7
	II      011    3
	III     010    2
	IV      110    6
	V       101    5
	VI      001    1
	VII     000    0
	VIII    100    4
	*/
	return pNode;
}

//在octree中插入碰撞组件
void CollideSystem::_insertCollider(OctreeNode* pTree, CCollider* pC) {
	RInt index = 0;//挂限标识
	RBool straddle = false;//是否同时与多个子挂限相交
	for (int i = 0; i < 3; i++) {
		float delta = ((SphereBV*)pC->wBV)->c[i] - pTree->center[i];
		if (abs(delta) < ((SphereBV*)pC->wBV)->r) {
			straddle = true;
			break;
		}
		if (delta > 0.0f) {
			index |= (1 << i);
		}
	}
	if (!straddle && pTree->pChild[index]) {
		//不同时与多个子挂限相交，同时存在子挂限，插入到子挂限中
		_insertCollider(pTree->pChild[index], pC);
	}
	else {
		//否则(同时与多个子挂限相交，或不存在子挂限)，插入到当前节点中
		if (pC->paNode != pTree) {
			if (pC->paNode) {
				//从原来的移除
				pC->paNode->cList.remove(pC);
			}
			pTree->cList.push_back(pC);
			pC->paNode = pTree;
		}
	}
}

//删除octree
void CollideSystem::_deleteOctree(OctreeNode* pNode) {
	if (nullptr == pNode) return;
	for (int i = 0; i < 8; i++) {
		_deleteOctree(pNode->pChild[i]);
	}
	SafeDelete(pNode);
}

//octree中对象碰撞检测
void CollideSystem::_testAllCollisionsInOctree(OctreeNode* pTree) {
	//不仅要和本节点中的对象碰撞检测，还要和所有祖先结点中的所有对象碰撞检测
	static OctreeNode* ancestorStack[MAX_OCTREE_DEPTH];
	static int depth = 0;
	ancestorStack[depth++] = pTree;
	for (int n = 0; n < depth; n++) {
		for (auto pC1 : pTree->cList) {
			for (auto pC2 : ancestorStack[n]->cList) {
				if (pC1 == pC2) break;
				m_cc3++;
				if (_intersectionTest(pC1->wBV, pC2->wBV)) {
					m_ci3++;
				}
			}
		}
	}

	//遍历子树
	for (int i = 0; i < 8; i++) {
		if (pTree->pChild[i])
			_testAllCollisionsInOctree(pTree->pChild[i]);
	}

	depth--;
}