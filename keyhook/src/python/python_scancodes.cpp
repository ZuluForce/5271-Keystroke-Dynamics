#ifndef PYTHONSCANCODES_H_INCLUDED
#define PYTHONSCANCODES_H_INCLUDED

#include <boost/python/enum.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>

#include "profile_enforcer.h"

using namespace boost::python;

ScanCode identity_(ScanCode x) { return x; }

BOOST_PYTHON_MODULE(DittoPyScancodes)
{
    enum_<ScanCode>("scancode")
        .value("SCANCODE_ESC"	, SCANCODE_ESC)
        .value("SCANCODE_1"		, SCANCODE_1)
        .value("SCANCODE_2"		, SCANCODE_2)
        .value("SCANCODE_3"		, SCANCODE_3)
        .value("SCANCODE_4"		, SCANCODE_4)
        .value("SCANCODE_5"		, SCANCODE_5)
        .value("SCANCODE_6"		, SCANCODE_6)
        .value("SCANCODE_7"		, SCANCODE_7)
        .value("SCANCODE_8"		, SCANCODE_8)
        .value("SCANCODE_9"		, SCANCODE_9)
        .value("SCANCODE_BCKSP"	, SCANCODE_BCKSP)
        .value("SCANCODE_Q"		, SCANCODE_Q)
        .value("SCANCODE_W"		, SCANCODE_W)
		.value("SCANCODE_E"		, SCANCODE_E)
        .value("SCANCODE_R"		, SCANCODE_R)
        .value("SCANCODE_T"		, SCANCODE_T)
        .value("SCANCODE_Y"		, SCANCODE_Y)
        .value("SCANCODE_U"		, SCANCODE_U)
        .value("SCANCODE_I"		, SCANCODE_I)
        .value("SCANCODE_O"		, SCANCODE_O)
        .value("SCANCODE_P"		, SCANCODE_P)
        .value("SCANCODE_RTRN"	, SCANCODE_RTRN)
        .value("SCANCODE_CTRL"	, SCANCODE_CTRL)
        .value("SCANCODE_A"		, SCANCODE_A)
        .value("SCANCODE_S"		, SCANCODE_S)
        .value("SCANCODE_D"		, SCANCODE_D)
        .value("SCANCODE_F"		, SCANCODE_F)
        .value("SCANCODE_G"		, SCANCODE_G)
        .value("SCANCODE_H"		, SCANCODE_H)
        .value("SCANCODE_J"		, SCANCODE_J)
        .value("SCANCODE_K"		, SCANCODE_K)
        .value("SCANCODE_L"		, SCANCODE_L)
        .value("SCANCODE_LSHIFT", SCANCODE_LSHFT)
        .value("SCANCODE_Z"		, SCANCODE_Z)
        .value("SCANCODE_X"		, SCANCODE_X)
        .value("SCANCODE_C"		, SCANCODE_C)
        .value("SCANCODE_V"		, SCANCODE_V)
        .value("SCANCODE_B"		, SCANCODE_B)
        .value("SCANCODE_N"		, SCANCODE_N)
        .value("SCANCODE_M"		, SCANCODE_M)
        .value("SCANCODE_RSHIFT", SCANCODE_RSHFT)
        .export_values();

    def("identity", identity_);
    def("get_scan_code", identity_);
}

#endif // PYTHONSCANCODES_H_INCLUDED
