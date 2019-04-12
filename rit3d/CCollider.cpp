#include "pch.h"
#include "CCollider.h"


CCollider::CCollider() {

}


CCollider::~CCollider() {
	if (wBV) {
		SafeDelete(wBV);
	}
}



CSphereCollider::CSphereCollider() {
	m_type = COMPTYPE::SPHERECOLLIDER;
	wBV = new SphereBV;
}


CSphereCollider::~CSphereCollider() {
}

CSphereCollider* CSphereCollider::CreateInstance() {
	return new CSphereCollider();
};



CBoxCollider::CBoxCollider() {
	m_type = COMPTYPE::BOXCOLLIDER;
}


CBoxCollider::~CBoxCollider() {

}

CBoxCollider* CBoxCollider::CreateInstance() {
	return new CBoxCollider();
};

