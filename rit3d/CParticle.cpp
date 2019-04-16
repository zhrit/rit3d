#include "pch.h"
#include "CParticle.h"

CParticle::CParticle() {
	m_type = COMPTYPE::PARTICLE;
}
CParticle::~CParticle() {

}

CParticle* CParticle::CreateInstance() {
	return new CParticle();
}
