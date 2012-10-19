#include "profile_enforcer.h"

std::map<unsigned int, char> const
DittoProfileEnforcer::scancodeToChar = {
	{0x15, 'y'},
	{0x2D, 'x'}
};
