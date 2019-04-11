#include "pch.h"
#include "CCollider.h"


CCollider::CCollider() {

}


CCollider::~CCollider() {

}



CSphereCollider::CSphereCollider() {
	m_type = COMPTYPE::SPHERECOLLIDER;
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

