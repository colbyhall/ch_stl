#include "../opengl.h"

#include "../thirdparty/wglext.h"

using WGL_Create_Context_Attribs_ARB = HGLRC(WINAPI *)(HDC hdc, HGLRC share_context, const s32* attrib_list);
using WGL_Choose_Pixel_Format_ARB = bool (WINAPI *)(HDC hdc, const s32 *int_attrib_list, const float *float_attrib_list, u32 max_formats, s32 *int_formats, u32 *num_formats);
using WGL_Swap_Interval_Ext = bool (WINAPI *)(s32 interval);

WGL_Create_Context_Attribs_ARB wglCreateContextAttribsARB = nullptr;
WGL_Choose_Pixel_Format_ARB wglChoosePixelFormatARB = nullptr;
WGL_Swap_Interval_Ext wglSwapIntervalEXT = nullptr;

const s32 win32_opengl_attribs[] = {
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 3,
	WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0,
};

bool ch::load_gl() {
	ch::Window dummy_window;
	if (!ch::create_window(CH_TEXT("dummy_gl_window"), 1, 1, 0, &dummy_window)) return false;
	defer(dummy_window.destroy());

    HDC window_context = GetDC((HWND)dummy_window.os_handle);
	defer(ReleaseDC((HWND)dummy_window.os_handle, window_context));

    PIXELFORMATDESCRIPTOR dpf;
    dpf.nSize = sizeof(dpf);
    dpf.nVersion = 1;
    dpf.iPixelType = PFD_TYPE_RGBA;
    dpf.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    dpf.cColorBits = 32;
    dpf.cAlphaBits = 8;
    dpf.cDepthBits = 24;
    dpf.iLayerType = PFD_MAIN_PLANE;

    const s32 suggested_pixel_foramat_index = ChoosePixelFormat(window_context, &dpf);

    DescribePixelFormat(window_context, suggested_pixel_foramat_index, sizeof(dpf), &dpf);
    SetPixelFormat(window_context, suggested_pixel_foramat_index, &dpf);

    HGLRC glrc = wglCreateContext(window_context);
	defer(wglDeleteContext(glrc));
    if (wglMakeCurrent(window_context, glrc)) {
        wglChoosePixelFormatARB = (WGL_Choose_Pixel_Format_ARB)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB = (WGL_Create_Context_Attribs_ARB)wglGetProcAddress("wglCreateContextAttribsARB");
        wglSwapIntervalEXT = (WGL_Swap_Interval_Ext)wglGetProcAddress("wglSwapIntervalEXT");
#define LOAD_GL_BINDINGS(type, func) func = (type)wglGetProcAddress(#func);
		GL_BINDINGS(LOAD_GL_BINDINGS);
#undef  LOAD_GL_BINDINGS
        wglMakeCurrent(window_context, NULL);
    }

    return ch::is_gl_loaded();
}

bool ch::is_gl_loaded() {
    bool result = true;
#define CHECK_GL_BINDINGS(type, func) result = result && (func != nullptr);
    GL_BINDINGS(CHECK_GL_BINDINGS);
#undef  CHECK_GL_BINDINGS
    return result;
}

bool ch::swap_buffers(OS_Window_Handle window_handle) {
    HDC window_context = GetDC((HWND)window_handle);
    defer(ReleaseDC((HWND)window_handle, window_context));
    return SwapBuffers(window_context);
}

bool ch::create_gl_window(const tchar* title, u32 width, u32 height, u32 style, ch::Window* out_window) {
    if (!ch::create_window(title, width, height, style, out_window)) {
        return false;
    }

	HDC real_dc = GetDC((HWND)out_window->os_handle);

	const s32 pixel_format_attribs[] = {
	WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
	WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
	WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
	WGL_COLOR_BITS_ARB,         32,
	WGL_DEPTH_BITS_ARB,         24,
	WGL_STENCIL_BITS_ARB,       8,
	0
	};

	s32 pixel_format;
	u32 num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	assert(num_formats);

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	SetPixelFormat(real_dc, pixel_format, &pfd);

    return true;
}

bool ch::make_current(OS_Window_Handle window_handle) {
    HDC window_context = GetDC((HWND)window_handle);
	defer(ReleaseDC((HWND)window_handle, window_context));

	HGLRC glrc = wglCreateContextAttribsARB(window_context, 0, win32_opengl_attribs);

	return wglMakeCurrent(window_context, glrc);
}