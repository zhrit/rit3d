#pragma once

#include "RCommon.h"

class Object
{
public:
	Object() {
		className = "Object";
	}

	virtual RString toString() {
		return this->className;
	}

protected:
	RString className;
};