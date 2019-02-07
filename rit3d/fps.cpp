#include "pch.h"
#include "fps.h"

FPS::FPS() {

}
FPS::~FPS() {

}

void FPS::update() {
	m_count++;
	if (m_timer.isInterval()) {
		cout << m_count << endl;
		m_count = 0;
	}
}

void FPS::show() {
	m_timer.reset();
	m_count = 0;
	m_isShow = true;
}
void FPS::hide() {
	m_isShow = false;
}
RBool FPS::isShowed() const {
	return m_isShow;
}