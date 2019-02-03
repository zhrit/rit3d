#include "pch.h"
#include "RVector.h"

//二维向量
RVector2::RVector2() : x(0.0f), y(0.0f) {

}
RVector2::RVector2(RFloat xf, RFloat yf) : x(xf), y(yf) {

}
RVector2::RVector2(const RVector2 & rhs) {
	x = rhs.x;
	y = rhs.y;
}

RVector2::~RVector2() {

}

RVector2 RVector2::ZERO(0.0f, 0.0f);
RVector2 RVector2::UNIT_X(1.0f, 0.0f);
RVector2 RVector2::UNIT_y(0.0f, 1.0f);

inline RFloat RVector2::length() const {
	return sqrt(x * x + y * y);
}

inline void RVector2::normalize() {
	RFloat len = length();
	if (len > EPSILON_E6) {
		RFloat invL = 1.0f / len;
		x *= invL;
		y *= invL;
	}
}

inline RFloat RVector2::dot(const RVector2& rhs) const {
	return x * rhs.x + y * rhs.y;
}
inline RFloat RVector2::angle(const RVector2& rhs) const {
	RFloat lenProduct = length() * rhs.length();
	if (lenProduct < EPSILON_E6) {
		lenProduct = EPSILON_E6;
	}
	RFloat f = dot(rhs) / lenProduct;
	f = Clamp(f, -1.0f, 1.0f);

	return acos(f);
}
inline RBool RVector2::isZero() const {
	return length() < EPSILON_E6;
}
inline RFloat RVector2::squaredLength() const {
	return x * x + y * y;
}
inline RVector2 RVector2::operator*(float rhs) const {
	return RVector2(x * rhs, y * rhs);
}
inline RVector2 RVector2::operator*(const RVector2 & rhs) const {
	return RVector2(x * rhs.x, y * rhs.y);
}
inline RVector2 RVector2::operator+(const RVector2 & rhs) const {
	return RVector2(x + rhs.x, y + rhs.y);
}
inline RVector2 RVector2::operator-(const RVector2 & rhs) const {
	return RVector2(x - rhs.x, y - rhs.y);
}
inline RVector2 RVector2::operator-() {
	x = -x;
	y = -x;
	return *this;
}
inline RVector2 RVector2::operator+() {
	x = +x;
	y = +y;
	return *this;
}
inline RBool RVector2::operator==(const RVector2& rhs) const {
	return EqualFloat(x, rhs.x) && EqualFloat(y, rhs.y);
}
inline RVector2 RVector2::operator=(const RVector2& rhs) {
	x = rhs.x;
	y = rhs.y;
	return *this;
}


//三维向量
RVector3::RVector3() : x(0.0f), y(0.0f), z(0.0f) {

}
RVector3::RVector3(RFloat xf, RFloat yf, RFloat zf) : x(xf), y(yf), z(zf) {

}
RVector3::RVector3(const RVector3 & rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

RVector3::~RVector3() {

}

RVector3 RVector3::ZERO(0.0f, 0.0f, 0.0f);
RVector3 RVector3::UNIT_X(1.0f, 0.0f, 0.0f);
RVector3 RVector3::UNIT_y(0.0f, 1.0f, 0.0f);
RVector3 RVector3::UNIT_z(0.0f, 0.0f, 1.0f);

inline RFloat RVector3::length() const {
	return sqrt(x * x + y * y + z * z);
}

inline void RVector3::normalize() {
	RFloat len = length();
	if (len > EPSILON_E6) {
		RFloat invL = 1.0f / len;
		x *= invL;
		y *= invL;
		z *= invL;
	}
}

inline RFloat RVector3::dot(const RVector3& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}
inline RFloat RVector3::angle(const RVector3& rhs) const {
	RFloat lenProduct = length() * rhs.length();
	if (lenProduct < EPSILON_E6) {
		lenProduct = EPSILON_E6;
	}
	RFloat f = dot(rhs) / lenProduct;
	f = Clamp(f, -1.0f, 1.0f);

	return acos(f);
}
inline RVector3 RVector3::cross(const RVector3& rhs) const {
	return RVector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}
inline RBool RVector3::isZero() const {
	return length() < EPSILON_E6;
}
inline RFloat RVector3::squaredLength() const {
	return x * x + y * y + z * z;
}
inline RVector3 RVector3::operator*(float rhs) const {
	return RVector3(x * rhs, y * rhs, z * rhs);
}
inline RVector3 RVector3::operator*(const RVector3 & rhs) const {
	return RVector3(x * rhs.x, y * rhs.y, z * rhs.z);
}
inline RVector3 RVector3::operator+(const RVector3 & rhs) const {
	return RVector3(x + rhs.x, y + rhs.y, z + rhs.z);
}
inline RVector3 RVector3::operator-(const RVector3 & rhs) const {
	return RVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}
inline RVector3 RVector3::operator-() {
	x = -x;
	y = -x;
	z = -z;
	return *this;
}
inline RVector3 RVector3::operator+() {
	x = +x;
	y = +y;
	z = +z;
	return *this;
}
inline RBool RVector3::operator==(const RVector3& rhs) const {
	return EqualFloat(x, rhs.x) && EqualFloat(y, rhs.y) && EqualFloat(z, rhs.z);
}
inline RVector3 RVector3::operator=(const RVector3& rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}



//四维向量，齐次坐标
RVector4::RVector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {

}
RVector4::RVector4(RFloat xf, RFloat yf, RFloat zf, RFloat wf) : x(xf), y(yf), z(zf), w(wf) {

}
RVector4::RVector4(const RVector4 & rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
}

RVector4::~RVector4() {

}

RVector4 RVector4::ZERO(0.0f, 0.0f, 0.0f, 1.0f);
RVector4 RVector4::UNIT_X(1.0f, 0.0f, 0.0f, 1.0f);
RVector4 RVector4::UNIT_y(0.0f, 1.0f, 0.0f, 1.0f);
RVector4 RVector4::UNIT_z(0.0f, 0.0f, 1.0f, 1.0f);

inline RFloat RVector4::length() const {
	return sqrt(x * x + y * y + z * z);
}

inline void RVector4::normalize() {
	RFloat len = length();
	if (len > EPSILON_E6) {
		RFloat invL = 1.0f / len;
		x *= invL;
		y *= invL;
		z *= invL;
		w = 1.0f;
	}
}

inline RFloat RVector4::dot(const RVector4& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}
inline RFloat RVector4::angle(const RVector4& rhs) const {
	RFloat lenProduct = length() * rhs.length();
	if (lenProduct < EPSILON_E6) {
		lenProduct = EPSILON_E6;
	}
	RFloat f = dot(rhs) / lenProduct;
	f = Clamp(f, -1.0f, 1.0f);

	return acos(f);
}
inline RVector4 RVector4::cross(const RVector4& rhs) const {
	return RVector4(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x, 1.0f);
}
inline RBool RVector4::isZero() const {
	return length() < EPSILON_E6;
}
inline RFloat RVector4::squaredLength() const {
	return x * x + y * y;
}
inline RVector4 RVector4::operator*(float rhs) const {
	return RVector4(x * rhs, y * rhs, z * rhs, 1.0f);
}
inline RVector4 RVector4::operator*(const RVector4 & rhs) const {
	return RVector4(x * rhs.x, y * rhs.y, z * rhs.z, 1.0f);
}
inline RVector4 RVector4::operator+(const RVector4 & rhs) const {
	return RVector4(x + rhs.x, y + rhs.y, z + rhs.z, 1.0f);
}
inline RVector4 RVector4::operator-(const RVector4 & rhs) const {
	return RVector4(x - rhs.x, y - rhs.y, z - rhs.z, 1.0f);
}
inline RVector4 RVector4::operator-() {
	x = -x;
	y = -x;
	z = -z;
	return *this;
}
inline RVector4 RVector4::operator+() {
	x = +x;
	y = +y;
	z = +z;
	return *this;
}
inline RBool RVector4::operator==(const RVector4& rhs) const {
	return EqualFloat(x, rhs.x) && EqualFloat(y, rhs.y) && EqualFloat(z, rhs.z) && EqualFloat(w, rhs.w);
}
inline RVector4 RVector4::operator=(const RVector4& rhs) {
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}
