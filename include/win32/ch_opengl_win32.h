#pragma once

#include <ch_types.h>

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#include "ch_os_win32.h"
#include <gl/GL.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013

#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_FULL_ACCELERATION_ARB                 0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB          0x20A9

#define WGL_RED_BITS_ARB                          0x2015
#define WGL_GREEN_BITS_ARB                        0x2017
#define WGL_BLUE_BITS_ARB                         0x2019
#define WGL_ALPHA_BITS_ARB                        0x201B
#define WGL_DEPTH_BITS_ARB                        0x2022

using WGL_Create_Context_Attribs_ARB = HGLRC (WINAPI *)(HDC hdc, HGLRC share_context, const s32* attrib_list);
using WGL_Choose_Pixel_Format_ARB = bool (WINAPI *)(HDC hdc, const s32 *int_attrib_list, const float *float_attrib_list, u32 max_formats, s32 *int_formats, u32 *num_formats);
using WGL_Swap_Interval_Ext = bool (WINAPI *)(s32 interval);

extern WGL_Create_Context_Attribs_ARB wglCreateContextAttribsARB;
extern WGL_Choose_Pixel_Format_ARB wglChoosePixelFormatARB;
extern WGL_Swap_Interval_Ext wglSwapIntervalEXT;