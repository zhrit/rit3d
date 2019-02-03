#include "pch.h"
#include "RColor.h"

RColor::RColor() : r(r), g(g), b(b), a(a) {

}
RColor::RColor(RFloat rf, RFloat gf, RFloat bf, RFloat af) : r(rf), g(gf), b(bf), a(af) {

}
RColor::RColor(RColor& rhs) : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a) {

}

inline RBool RColor::operator==(const RColor& rhs) const {
	return EqualFloat(r, rhs.r) && EqualFloat(g, rhs.g) && EqualFloat(b, rhs.b) && EqualFloat(a, rhs.a);
}

inline RColor RColor::operator=(const RColor& rhs) {
	r = rhs.r;
	g = rhs.g;
	b = rhs.b;
	a = rhs.a;
	return *this;
}