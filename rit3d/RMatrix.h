#pragma once

#include "Object.h"
#include "RCommon.h"
#include "RMath.h"
using RMath::EqualFloat;

class RMatrix44 : protected Object {
public:
	RMatrix44();
	RMatrix44(const RMatrix44& rhs);
	RMatrix44(RFloat m00, RFloat m01, RFloat m02, RFloat m03,
		RFloat m10, RFloat m11, RFloat m12, RFloat m13,
		RFloat m20, RFloat m21, RFloat m22, RFloat m23,
		RFloat m30, RFloat m31, RFloat m32, RFloat m33);
	virtual ~RMatrix44();

	static RMatrix44 IDENTITY;

public:
	RFloat m[4][4];

public:
	inline RMatrix44 transpose() const;
	inline RFloat getAt(size_t i, size_t j) const;
	inline void setAt(size_t i, size_t j, RFloat val);
	inline RMatrix44 operator+(const RMatrix44& rhs) const;
	inline RMatrix44 operator-(const RMatrix44& rhs) const;
	inline RMatrix44 operator*(RFloat factor) const;
	inline RMatrix44 operator*(const RMatrix44& rhs) const;
	inline RMatrix44& operator=(const RMatrix44& rhs);
	inline RBool operator==(const RMatrix44& rhs) const;

};
