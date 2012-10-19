#include "profile_enforcer.h"

std::map<unsigned int, char>
DittoProfileEnforcer::scancodeToChar = {
	{0x15, 'y'},
	{0x2D, 'x'}
};
