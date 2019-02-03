#pragma once

#include "Object.h"
#include "RMath.h"
using RMath::EqualFloat;

class RColor : protected Object {
public:
	RColor();
	RColor(RFloat rf, RFloat gf, RFloat bf, RFloat af = 1.0f);
	RColor(RColor& rhs);

public:
	RFloat r;
	RFloat g;
	RFloat b;
	RFloat a;

public:
	inline RBool operator==(const RColor& rhs) const;
	inline RColor operator=(const RColor& rhs);
};