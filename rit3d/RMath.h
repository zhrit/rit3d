#pragma once

#include "RCommon.h"

namespace RMath {
	inline float Clamp(float x, float min, float max) {
		if (x < min) {
			x = min;
		}
		if (x > max) {
			x = max;
		}
		return x;
	}

	inline bool EqualFloat(float x, float y) {
		return Abs(x - y) < EPSILON_E6;
	}
}