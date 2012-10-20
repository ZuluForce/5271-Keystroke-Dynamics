#include "profile_enforcer.h"

std::map<unsigned int, char>
DittoProfileEnforcer::scancodeToChar = {
	{0x15, 'y'},
	{0x2D, 'x'}
};

bool strokesEqual(InterceptionKeyStroke* s1,InterceptionKeyStroke* s2) {
	if (s1->code == s2->code && s1->state == s2->state) {
		return true;
	}
	return false;
}
