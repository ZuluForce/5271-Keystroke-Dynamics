#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <interception.h>
#include "utils.h"

#define sleep(n) Sleep(1000*n)
#define sleep_ms(n) Sleep(n)

enum ScanCode {
    SCANCODE_X   = 0x2D,
    SCANCODE_Y   = 0x15,
    SCANCODE_ESC = 0x01
};

int main(int argc, char* argv[]) {
	InterceptionContext context;
	InterceptionDevice device;
	InterceptionKeyStroke stroke;

	raise_process_priority();
	
	context = interception_create_context();
	if (context == NULL) {
		fprintf(stderr, "Failed to initialize the interception context\n");
		exit(-1);
	}

	InterceptionFilter captureFilter = 0;
	
	// This will capture most keys
	captureFilter |= INTERCEPTION_KEY_DOWN;
	captureFilter |= INTERCEPTION_KEY_UP;
	
	// Add any more filters here
	// captureFilter |= INTERCEPTION_FILTER_KEY_E0
	
	// Or do the following to just grab everything
	// captureFilter |= INTERCEPTION_FILTER_KEY_ALL
	
	interception_set_filter(context, interception_is_keyboard, captureFilter);
	
	while (interception_receive(context, device = interception_wait(context), (InterceptionStroke *)&stroke, 1) > 0) {
		// To make it easier to stop
		if (stroke.code == SCANCODE_ESC) break;

		// Delay keystroke by 1 second
		sleep(1);
		interception_send(context, device, (InterceptionStroke *)&stroke, 1);
	}
	
	fprintf(stdout, "Shutting down interception context\n");
	interception_destroy_context(context);
	
	return 0;
}