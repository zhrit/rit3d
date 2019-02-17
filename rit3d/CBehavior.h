#pragma once
#include "RCommon.h"
#include "IComponent.h"
class BaseBehavior;
class CBehavior : public IComponent
{
private:
	CBehavior();
	virtual ~CBehavior();

	BaseBehavior* m_behavior{ nullptr };

public:
	static CBehavior* CreateInstance();

	BaseBehavior* getBehavior() const;
	void setBehavior(BaseBehavior* _b);
};

