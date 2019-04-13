#include "pch.h"
#include "fps.h"

FPS::FPS() {

}
FPS::~FPS() {

}

void FPS::update() {
	m_count++;
	if (m_timer.isInterval()) {
		cout << "fps: " << m_count << endl;
		m_count = 0;
	}
}
