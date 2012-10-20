#include <stdio.h>
#include <interception.h>
#include <chrono>
#include "chrono_io"
#include "utils.h"

enum ScanCode {
	SCANCODE_Z		= 0x2C,
    SCANCODE_X   	= 0x2D,
    SCANCODE_C		= 0x2E,
    SCANCODE_V		= 0x2F,
    SCANCODE_B		= 0x30,
    SCANCODE_N		= 0x31,
    SCANCODE_M		= 0x32,
    SCANCODE_Y   	= 0x15,
    SCANCODE_ESC 	= 0x01
};

int main() {
	printf("Intercepting x and y keys and swapping them\n");

    InterceptionContext context;
    InterceptionDevice device;
    InterceptionKeyStroke stroke;

    raise_process_priority();

    context = interception_create_context();
	if (context == NULL) {
		fprintf(stderr, "Failed to initizlize interception context\n");
		return -1;
	}

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

    while(interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0) {
        if(stroke.code == SCANCODE_X) stroke.code = SCANCODE_Y;

        interception_send(context, device, (InterceptionStroke *)&stroke, 1);

        if(stroke.code == SCANCODE_ESC) break;
    }

    interception_destroy_context(context);

    return 0;
}
