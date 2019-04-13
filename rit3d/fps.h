#pragma once
#include "RCommon.h"
#include "RTimer.h"

class FPS {
public:
	FPS();
	~FPS();
private:
	//计时器
	RTimer m_timer{ 3000, 1000 };
	//计数
	RUInt m_count{ 0 };
public:
	void update();
};