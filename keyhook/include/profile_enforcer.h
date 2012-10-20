#ifndef PROFILE_ENFORCER_H_INCLUDED
#define PROFILE_ENFORCER_H_INCLUDED

#include <map>
#include <vector>
#include <initializer_list>
#include "interception.h"
#include "profile_loader.h"

enum ScanCode {
	SCANCODE_ESC	= 0x01,
	SCANCODE_1		= 0x02,
	SCANCODE_2		= 0x03,
	SCANCODE_3		= 0x04,
	SCANCODE_4		= 0x05,
	SCANCODE_5		= 0x06,
	SCANCODE_6		= 0x07,
	SCANCODE_7		= 0x08,
	SCANCODE_8		= 0x09,
	SCANCODE_9		= 0x0A,
	SCANCODE_0		= 0x0B,
	SCANCODE_BCKSP	= 0x0E,
	SCANCODE_Q		= 0x10,
	SCANCODE_W		= 0x11,
	SCANCODE_E		= 0x12,
	SCANCODE_R		= 0x13,
	SCANCODE_T		= 0x14,
	SCANCODE_Y		= 0x15,
	SCANCODE_U		= 0x16,
	SCANCODE_I		= 0x17,
	SCANCODE_O		= 0x18,
	SCANCODE_P		= 0x19,
	SCANCODE_RTRN	= 0x1C,
	SCANCODE_CTRL	= 0x1D,
	SCANCODE_A		= 0x1E,
	SCANCODE_S		= 0x1F,
	SCANCODE_D		= 0x20,
	SCANCODE_F		= 0x21,
	SCANCODE_G		= 0x22,
	SCANCODE_H		= 0x23,
	SCANCODE_J		= 0x24,
	SCANCODE_K		= 0x25,
	SCANCODE_L		= 0x26,
	SCANCODE_LSHFT	= 0x2A,
	SCANCODE_Z		= 0x2C,
    SCANCODE_X   	= 0x2D,
    SCANCODE_C		= 0x2E,
    SCANCODE_V		= 0x2F,
    SCANCODE_B		= 0x30,
    SCANCODE_N		= 0x31,
    SCANCODE_M		= 0x32,
    SCANCODE_RSHFT	= 0x36
};

bool strokesEqual(InterceptionKeyStroke*,InterceptionKeyStroke*);

class DittoProfileEnforcer {
private:

public:
	static std::map<unsigned int, char> scancodeToChar;

	virtual void enforce(KDProfile&) = 0;

};

#endif // PROFILE_ENFORCER_H_INCLUDED
