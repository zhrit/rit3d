#pragma once
#include "IComponent.h"

class ISingletonComponent : public IComponent {
public:
	ISingletonComponent();
	virtual ~ISingletonComponent();
};

