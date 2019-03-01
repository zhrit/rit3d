#pragma once
#include "RCommon.h"
#include "IComponent.h"

class CCollider : public IComponent {
private:
	CCollider();
	virtual ~CCollider();

public:
	static CCollider* CreateInstance();

};

