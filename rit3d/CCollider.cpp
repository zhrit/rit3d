#include "pch.h"
#include "CCollider.h"


CCollider::CCollider() {
	m_type = COMPTYPE::COLLIDER;
}


CCollider::~CCollider() {

}

CCollider* CCollider::CreateInstance() {
	return new CCollider();
};
