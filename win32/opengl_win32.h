#pragma once

#include "../types.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#include <windows.h>
#include <gl/GL.h>

using WGL_Create_Context_Attribs_ARB = HGLRC(WINAPI *)(HDC hdc, HGLRC share_context, const s32* attrib_list);
using WGL_Choose_Pixel_Format_ARB = bool (WINAPI *)(HDC hdc, const s32 *int_attrib_list, const float *float_attrib_list, u32 max_formats, s32 *int_formats, u32 *num_formats);
using WGL_Swap_Interval_Ext = bool (WINAPI *)(s32 interval);

extern WGL_Swap_Interval_Ext wglSwapIntervalEXT;