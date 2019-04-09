#include "pch.h"
#include "CCollider.h"


CSphereCollider::CSphereCollider() {
	m_type = COMPTYPE::SPHERECOLLIDER;
}


CSphereCollider::~CSphereCollider() {

}

CSphereCollider* CSphereCollider::CreateInstance() {
	return new CSphereCollider();
};
