#pragma once

#include "RCommon.h"

//游戏对象所在的图层
typedef enum {
	CullingMaskNothing = 0x0000,
	CullingMaskEverything = 0xFFFF,

	BuiltinLayer0 = 0x0001,
	BuiltinLayer1 = 0x0002,
	BuiltinLayer2 = 0x0004,
	BuiltinLayer3 = 0x0008,
	BuiltinLayer4 = 0x0010,
	BuiltinLayer5 = 0x0020,
	BuiltinLayer6 = 0x0040,
	BuiltinLayer7 = 0x0080,

	UserLayer0 = 0x0100,
	UserLayer1 = 0x0200,
	UserLayer2 = 0x0400,
	UserLayer3 = 0x0800,
	UserLayer4 = 0x1000,
	UserLayer5 = 0x2000,
	UserLayer6 = 0x4000,
	UserLayer7 = 0x8000,

	Default = BuiltinLayer0,
} LAYER;

class Object
{
public:
	Object();
	virtual ~Object();

public:
	RUInt id;
};