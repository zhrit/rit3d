#include "pch.h"
#include "RTimer.h"

RTimer::RTimer(DWORD _total, DWORD _interval) : m_timeTotal(_total), m_timeInterval(_interval) {

}

RTimer::~RTimer() {

}

DWORD RTimer::getTimeStart() const {
	return m_timeStart;
}
void RTimer::reset() {
	m_timeStart = ::GetTickCount();
	m_timeLast = m_timeStart;
}

DWORD RTimer::getTimeTotal() const {
	return m_timeTotal;
}
void RTimer::setTimeTotal(DWORD _t) {
	m_timeTotal = _t;
}

DWORD RTimer::getTimeInterval() const {
	return m_timeInterval;
}
void RTimer::setTimeInterval(DWORD _t) {
	m_timeInterval = _t;
}

RBool RTimer::isTotal() const {
	return (::GetTickCount() - m_timeStart) >= m_timeTotal ? true : false;
}
RBool RTimer::isInterval() {
	if (::GetTickCount() - m_timeLast >= m_timeInterval) {
		m_timeLast = ::GetTickCount();
		return true;
	}
	return false;
}

