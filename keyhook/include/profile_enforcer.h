#ifndef PROFILE_ENFORCER_H_INCLUDED
#define PROFILE_ENFORCER_H_INCLUDED

#include <map>
#include <vector>
#include <initializer_list>
#include "interception.h"
#include "profile_loader.h"

enum ScanCode {
	SCANCODE_ESC	= 0x01, // 1
	SCANCODE_1		= 0x02, // 2
	SCANCODE_2		= 0x03, // 3
	SCANCODE_3		= 0x04, // 4
	SCANCODE_4		= 0x05, // 5
	SCANCODE_5		= 0x06, // 6
	SCANCODE_6		= 0x07, // 7
	SCANCODE_7		= 0x08, // 8
	SCANCODE_8		= 0x09, // 9
	SCANCODE_9		= 0x0A, // 10
	SCANCODE_0		= 0x0B, // 11
	SCANCODE_BCKSP	= 0x0E, // 14
	SCANCODE_Q		= 0x10, // 16
	SCANCODE_W		= 0x11, // 17
	SCANCODE_E		= 0x12, // 18
	SCANCODE_R		= 0x13, // 19
	SCANCODE_T		= 0x14, // 20
	SCANCODE_Y		= 0x15, // 21
	SCANCODE_U		= 0x16, // 22
	SCANCODE_I		= 0x17, // 23
	SCANCODE_O		= 0x18, // 24
	SCANCODE_P		= 0x19, // 25
	SCANCODE_RTRN	= 0x1C, // 28
	SCANCODE_CTRL	= 0x1D, // 29
	SCANCODE_A		= 0x1E, // 30
	SCANCODE_S		= 0x1F, // 31
	SCANCODE_D		= 0x20, // 32
	SCANCODE_F		= 0x21, // 33
	SCANCODE_G		= 0x22, // 34
	SCANCODE_H		= 0x23, // 35
	SCANCODE_J		= 0x24, // 36
	SCANCODE_K		= 0x25, // 37
	SCANCODE_L		= 0x26, // 38
	SCANCODE_LSHFT	= 0x2A, // 42
	SCANCODE_Z		= 0x2C, // 44
    SCANCODE_X   	= 0x2D, // 45
    SCANCODE_C		= 0x2E, // 46
    SCANCODE_V		= 0x2F, // 47
    SCANCODE_B		= 0x30, // 48
    SCANCODE_N		= 0x31, // 49
    SCANCODE_M		= 0x32, // 50
    SCANCODE_RSHFT	= 0x36, // 54
    SCANCODE_SPACE	= 0x39, // 57
};

// Be sure to update if we map new scancodes
static const ScanCode maxScanCode = SCANCODE_RSHFT;

bool strokesEqual(InterceptionKeyStroke*,InterceptionKeyStroke*);

class DittoProfileEnforcer {
private:

public:
	static std::map<unsigned int, char> scancodeToChar;

	virtual void enforce(KDProfile&) = 0;

};

#endif // PROFILE_ENFORCER_H_INCLUDED
