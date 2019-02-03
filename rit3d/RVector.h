#pragma once

#include "RCommon.h"
#include "Object.h"
#include "RMath.h"
using RMath::Clamp;
using RMath::EqualFloat;

class RVector2 : protected Object {
public:
	RVector2();
	RVector2(RFloat xf, RFloat yf);
	RVector2(const RVector2 & rhs);
	virtual ~RVector2();

	static RVector2 ZERO;
	static RVector2 UNIT_X;
	static RVector2 UNIT_y;
	
public:
	RFloat x;
	RFloat y;

public:
	inline RFloat length() const;
	inline void normalize();
	inline RFloat dot(const RVector2& rhs) const;
	inline RFloat angle(const RVector2& rhs) const;
	inline RBool isZero() const;
	inline RFloat squaredLength() const;
	inline RVector2 operator*(float rhs) const;
	inline RVector2 operator*(const RVector2 & rhs) const;
	inline RVector2 operator+(const RVector2 & rhs) const;
	inline RVector2 operator-(const RVector2 & rhs) const;
	inline RVector2 operator-();
	inline RVector2 operator+();
	inline RBool operator==(const RVector2& rhs) const;
	inline RVector2 operator=(const RVector2& rhs);
};


class RVector3 : protected Object {
public:
	RVector3();
	RVector3(RFloat xf, RFloat yf, RFloat zf);
	RVector3(const RVector3& rhs);
	virtual ~RVector3();

	static RVector3 ZERO;
	static RVector3 UNIT_X;
	static RVector3 UNIT_y;
	static RVector3 UNIT_z;

public:
	RFloat x;
	RFloat y;
	RFloat z;

public:
	inline RFloat length() const;
	inline void normalize();
	inline RFloat dot(const RVector3& rhs) const;
	inline RVector3 cross(const RVector3& rhs) const;
	inline RFloat angle(const RVector3& rhs) const;
	inline RBool isZero() const;
	inline RFloat squaredLength() const;
	inline RVector3 operator*(float rhs) const;
	inline RVector3 operator*(const RVector3& rhs) const;
	inline RVector3 operator+(const RVector3& rhs) const;
	inline RVector3 operator-(const RVector3& rhs) const;
	inline RVector3 operator-();
	inline RVector3 operator+();
	inline RBool operator==(const RVector3& rhs) const;
	inline RVector3 operator=(const RVector3& rhs);
};


class RVector4 : protected Object {
public:
	RVector4();
	RVector4(RFloat xf, RFloat yf, RFloat zf, RFloat wf);
	RVector4(const RVector4& rhs);
	virtual ~RVector4();

	static RVector4 ZERO;
	static RVector4 UNIT_X;
	static RVector4 UNIT_y;
	static RVector4 UNIT_z;

public:
	RFloat x;
	RFloat y;
	RFloat z;
	RFloat w;

public:
	inline RFloat length() const;
	inline void normalize();
	inline RFloat dot(const RVector4& rhs) const;
	inline RVector4 cross(const RVector4& rhs) const;
	inline RFloat angle(const RVector4& rhs) const;
	inline RBool isZero() const;
	inline RFloat squaredLength() const;
	inline RVector4 operator*(float rhs) const;
	inline RVector4 operator*(const RVector4& rhs) const;
	inline RVector4 operator+(const RVector4& rhs) const;
	inline RVector4 operator-(const RVector4& rhs) const;
	inline RVector4 operator-();
	inline RVector4 operator+();
	inline RBool operator==(const RVector4& rhs) const;
	inline RVector4 operator=(const RVector4& rhs);
};