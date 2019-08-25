#include "../os.h"

#include <windows.h>

bool ch::get_caret_blink_time(u32* out_ms) {
	const u32 blink_time = GetCaretBlinkTime();
	if (blink_time == INFINITE) return false;

	*out_ms = blink_time;
	return true;
}