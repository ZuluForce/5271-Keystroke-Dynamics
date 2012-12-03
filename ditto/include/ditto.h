#ifndef DITTO_H_INCLUDED
#define DITTO_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "known_ploaders.h"
#include "enforcers/basic_enforcer.h"

struct DittoOptions {
    char* profileType; // Tells ditto which loader to use (-l flag)
    char* profileInfo; // Other data for the profile loader (-i flag)
    bool looseStrokes; // Relax the strict serial ordering for improved profile accuracy

    bool initialized;
};

#endif // DITTO_H_INCLUDED
