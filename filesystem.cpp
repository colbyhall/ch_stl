#include "filesystem.h"

// @HACK(CHall): We don't need to use cstl
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>

ch::Stream& ch::Stream::operator<<(f32 f) {
	tchar buffer[100];
#if CH_UNICODE
	swprintf(buffer, CH_TEXT("%f"), f);
#else
	sprintf(buffer, "%f", f);
#endif
	write_raw(buffer, ch::strlen(buffer));
	return *this;
}