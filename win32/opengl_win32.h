#pragma once

#include "../types.h"
#include "../os.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WINGDIAPI DLL_IMPORT
#define APIENTRY WINAPI
#define APIENTRYP APIENTRY *
#define GLAPI extern

using HDC = HANDLE;
using HGLRC = HANDLE;
using HPBUFFERARB = HANDLE;
using HPBUFFEREXT = HANDLE;

#define DECLARE_HANDLE(handle) typedef HANDLE handle

// @HACK(CHall): To prevent windows.h include
#define _INC_WINAPIFAMILY

#define WINAPI_FAMILY_PARTITION(x) x
#define WINAPI_PARTITION_DESKTOP 1

#include <gl/GL.h>


using WGL_Create_Context_Attribs_ARB = HGLRC(WINAPI *)(HDC hdc, HGLRC share_context, const s32* attrib_list);
using WGL_Choose_Pixel_Format_ARB = bool (WINAPI *)(HDC hdc, const s32 *int_attrib_list, const float *float_attrib_list, u32 max_formats, s32 *int_formats, u32 *num_formats);
using WGL_Swap_Interval_Ext = bool (WINAPI *)(s32 interval);

extern WGL_Swap_Interval_Ext wglSwapIntervalEXT;
