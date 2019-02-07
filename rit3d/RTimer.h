#pragma once
#include "RCommon.h"

class RTimer {
public:
	RTimer(DWORD _total, DWORD _interval);
	~RTimer();

private:
	DWORD m_timeStart{ ::GetTickCount() };//开始时间
	DWORD m_timeLast{ m_timeStart };//上一次计时时间
	DWORD m_timeTotal{ 3000 };//总的超时时间
	DWORD m_timeInterval{ 30 };//触发间隔

public:
	DWORD getTimeStart() const;
	void reset();

	DWORD getTimeTotal() const;
	void setTimeTotal(DWORD _t);

	DWORD getTimeInterval() const;
	void setTimeInterval(DWORD _t);

	RBool isTotal() const;
	RBool isInterval();
};