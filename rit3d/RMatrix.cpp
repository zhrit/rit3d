#include "pch.h"
#include "RMatrix.h"

RMatrix44::RMatrix44() {
	memset(m, 0, sizeof(RFloat) * 16);
}
RMatrix44::RMatrix44(const RMatrix44& rhs) {
	m[0][0] = rhs.m[0][0], m[0][1] = rhs.m[0][1], m[0][2] = rhs.m[0][2], m[0][3] = rhs.m[0][3];
	m[1][0] = rhs.m[1][0], m[1][1] = rhs.m[1][1], m[1][2] = rhs.m[1][2], m[1][3] = rhs.m[1][3];
	m[2][0] = rhs.m[2][0], m[2][1] = rhs.m[2][1], m[2][2] = rhs.m[2][2], m[2][3] = rhs.m[2][3];
	m[3][0] = rhs.m[3][0], m[3][1] = rhs.m[3][1], m[3][2] = rhs.m[3][2], m[3][3] = rhs.m[3][3];
}
RMatrix44::RMatrix44(RFloat m00, RFloat m01, RFloat m02, RFloat m03,
	RFloat m10, RFloat m11, RFloat m12, RFloat m13,
	RFloat m20, RFloat m21, RFloat m22, RFloat m23,
	RFloat m30, RFloat m31, RFloat m32, RFloat m33) {
	m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
	m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
	m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
	m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;

}
RMatrix44::~RMatrix44() {
	
}

RMatrix44 RMatrix44::IDENTITY(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

inline RMatrix44 RMatrix44::transpose() const {
	RMatrix44 temp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; i < 4; j++) {
			temp.m[i][j] = m[j][i];
		}
	}
	return temp;
}
inline RFloat RMatrix44::getAt(size_t i, size_t j) const {
	return m[i][j];
}
inline void RMatrix44::setAt(size_t i, size_t j, RFloat val) {
	m[i][j] = val;
}
inline RMatrix44 RMatrix44::operator+(const RMatrix44& rhs) const {
	return RMatrix44(
		m[0][0] + rhs.m[0][0], m[0][1] + rhs.m[0][1], m[0][2] + rhs.m[0][2], m[0][3] + rhs.m[0][3],
		m[1][0] + rhs.m[1][0], m[1][1] + rhs.m[1][1], m[1][2] + rhs.m[1][2], m[1][3] + rhs.m[1][3],
		m[2][0] + rhs.m[2][0], m[2][1] + rhs.m[2][1], m[2][2] + rhs.m[2][2], m[2][3] + rhs.m[2][3],
		m[3][0] + rhs.m[3][0], m[3][1] + rhs.m[3][1], m[3][2] + rhs.m[3][2], m[3][3] + rhs.m[3][3]
	);
}
inline RMatrix44 RMatrix44::operator-(const RMatrix44& rhs) const {
	return RMatrix44(
		m[0][0] - rhs.m[0][0], m[0][1] - rhs.m[0][1], m[0][2] - rhs.m[0][2], m[0][3] - rhs.m[0][3],
		m[1][0] - rhs.m[1][0], m[1][1] - rhs.m[1][1], m[1][2] - rhs.m[1][2], m[1][3] - rhs.m[1][3],
		m[2][0] - rhs.m[2][0], m[2][1] - rhs.m[2][1], m[2][2] - rhs.m[2][2], m[2][3] - rhs.m[2][3],
		m[3][0] - rhs.m[3][0], m[3][1] - rhs.m[3][1], m[3][2] - rhs.m[3][2], m[3][3] - rhs.m[3][3]
	);
}
inline RMatrix44 RMatrix44::operator*(RFloat factor) const {
	return RMatrix44(
		m[0][0] * factor, m[0][1] * factor, m[0][2] * factor, m[0][3] * factor,
		m[1][0] * factor, m[1][1] * factor, m[1][2] * factor, m[1][3] * factor,
		m[2][0] * factor, m[2][1] * factor, m[2][2] * factor, m[2][3] * factor,
		m[3][0] * factor, m[3][1] * factor, m[3][2] * factor, m[3][3] * factor
	);
}
inline RMatrix44 RMatrix44::operator*(const RMatrix44& rhs) const {
	RMatrix44 temp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				temp.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}
	return temp;
}
inline RMatrix44& RMatrix44::operator=(const RMatrix44& rhs) {
	m[0][0] = rhs.m[0][0], m[0][1] = rhs.m[0][1], m[0][2] = rhs.m[0][2], m[0][3] = rhs.m[0][3];
	m[1][0] = rhs.m[1][0], m[1][1] = rhs.m[1][1], m[1][2] = rhs.m[1][2], m[1][3] = rhs.m[1][3];
	m[2][0] = rhs.m[2][0], m[2][1] = rhs.m[2][1], m[2][2] = rhs.m[2][2], m[2][3] = rhs.m[2][3];
	m[3][0] = rhs.m[3][0], m[3][1] = rhs.m[3][1], m[3][2] = rhs.m[3][2], m[3][3] = rhs.m[3][3];
	return *this;
}
inline RBool RMatrix44::operator==(const RMatrix44& rhs) const {
	return (
		EqualFloat(m[0][0], rhs.m[0][0]) && EqualFloat(m[0][1], rhs.m[0][1]) && EqualFloat(m[0][2], rhs.m[0][2]) && EqualFloat(m[0][3], rhs.m[0][3]) &&
		EqualFloat(m[1][0], rhs.m[1][0]) && EqualFloat(m[1][1], rhs.m[1][1]) && EqualFloat(m[1][2], rhs.m[1][2]) && EqualFloat(m[1][3], rhs.m[1][3]) &&
		EqualFloat(m[2][0], rhs.m[2][0]) && EqualFloat(m[2][1], rhs.m[2][1]) && EqualFloat(m[2][2], rhs.m[2][2]) && EqualFloat(m[2][3], rhs.m[2][3]) &&
		EqualFloat(m[3][0], rhs.m[3][0]) && EqualFloat(m[3][1], rhs.m[3][1]) && EqualFloat(m[3][2], rhs.m[3][2]) && EqualFloat(m[3][3], rhs.m[3][3])
		);
}