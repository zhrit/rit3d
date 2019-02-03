#include "pch.h"
#include "RCommon.h"

RString util::num2str(RUInt num) {
	RString str = "";
	do {
		RUInt mod = num - num / 10 * 10;
		RChar c(mod + 48);
		RString s = "a";
		s[0] = c;
		str = s + str;
		num = num / 10;
	} while (num > 0);
	return str;
}

RString util::num2str(RInt num) {
	RString str = "";
	while (num > 0) {
		RUInt mod = num - num / 10 * 10;
		RChar c(mod + 48);
		RString s = "a";
		s[0] = c;
		str = s + str;
		num = num / 10;
	}
	return str;
}