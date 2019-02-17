#include "pch.h"
#include "CBehavior.h"
#include "BaseBehavior.h"

CBehavior::CBehavior() {
	m_type = COMPTYPE::BEHAVIOR;
}


CBehavior::~CBehavior() {

}

CBehavior* CBehavior::CreateInstance() {
	return new CBehavior();
};

BaseBehavior* CBehavior::getBehavior() const {
	return m_behavior;
}
void CBehavior::setBehavior(BaseBehavior* _b) {
	_b->gameObject = gameObject;
	m_behavior = _b;
}
