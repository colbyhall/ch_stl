#include "../opengl.h"

#include "../thirdparty/wglext.h"

WGL_Create_Context_Attribs_ARB wglCreateContextAttribsARB = nullptr;
WGL_Choose_Pixel_Format_ARB wglChoosePixelFormatARB = nullptr;
WGL_Swap_Interval_Ext wglSwapIntervalEXT = nullptr;

struct PIXELFORMATDESCRIPTOR {
	WORD  nSize;
	WORD  nVersion;
	DWORD dwFlags;
	BYTE  iPixelType;
	BYTE  cColorBits;
	BYTE  cRedBits;
	BYTE  cRedShift;
	BYTE  cGreenBits;
	BYTE  cGreenShift;
	BYTE  cBlueBits;
	BYTE  cBlueShift;
	BYTE  cAlphaBits;
	BYTE  cAlphaShift;
	BYTE  cAccumBits;
	BYTE  cAccumRedBits;
	BYTE  cAccumGreenBits;
	BYTE  cAccumBlueBits;
	BYTE  cAccumAlphaBits;
	BYTE  cDepthBits;
	BYTE  cStencilBits;
	BYTE  cAuxBuffers;
	BYTE  iLayerType;
	BYTE  bReserved;
	DWORD dwLayerMask;
	DWORD dwVisibleMask;
	DWORD dwDamageMask;
};
using LPPIXELFORMATDESCRIPTOR = PIXELFORMATDESCRIPTOR*;

#define PFD_TYPE_RGBA        0
#define PFD_TYPE_COLORINDEX  1

/* layer types */
#define PFD_MAIN_PLANE       0
#define PFD_OVERLAY_PLANE    1
#define PFD_UNDERLAY_PLANE   (-1)

/* PIXELFORMATDESCRIPTOR flags */
#define PFD_DOUBLEBUFFER            0x00000001
#define PFD_STEREO                  0x00000002
#define PFD_DRAW_TO_WINDOW          0x00000004
#define PFD_DRAW_TO_BITMAP          0x00000008
#define PFD_SUPPORT_GDI             0x00000010
#define PFD_SUPPORT_OPENGL          0x00000020
#define PFD_GENERIC_FORMAT          0x00000040
#define PFD_NEED_PALETTE            0x00000080
#define PFD_NEED_SYSTEM_PALETTE     0x00000100
#define PFD_SWAP_EXCHANGE           0x00000200
#define PFD_SWAP_COPY               0x00000400
#define PFD_SWAP_LAYER_BUFFERS      0x00000800
#define PFD_GENERIC_ACCELERATED     0x00001000
#define PFD_SUPPORT_DIRECTDRAW      0x00002000
#define PFD_DIRECT3D_ACCELERATED    0x00004000
#define PFD_SUPPORT_COMPOSITION     0x00008000

/* PIXELFORMATDESCRIPTOR flags for use in ChoosePixelFormat only */
#define PFD_DEPTH_DONTCARE          0x20000000
#define PFD_DOUBLEBUFFER_DONTCARE   0x40000000
#define PFD_STEREO_DONTCARE         0x80000000

extern "C" {
	DLL_IMPORT HDC   WINAPI GetDC(HWND);
	DLL_IMPORT int   WINAPI ReleaseDC(HWND, HDC);
	DLL_IMPORT int   WINAPI ChoosePixelFormat(HDC, const PIXELFORMATDESCRIPTOR*);
	DLL_IMPORT int   WINAPI DescribePixelFormat(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
	DLL_IMPORT HGLRC WINAPI wglCreateContext(HDC);
	DLL_IMPORT BOOL  WINAPI wglDeleteContext(HGLRC);
	DLL_IMPORT BOOL  WINAPI wglMakeCurrent(HDC, HGLRC);
	DLL_IMPORT void* WINAPI wglGetProcAddress(LPCSTR);
	DLL_IMPORT BOOL  WINAPI SwapBuffers(HDC);
	DLL_IMPORT BOOL  WINAPI SetPixelFormat(HDC, int, const PIXELFORMATDESCRIPTOR*);
}

bool ch::load_gl() {
	ch::Window fake_window;
	if (!ch::create_window("fake_window", 12, 12, 0, &fake_window)) return false;
	defer(fake_window.free());

	HDC window_context = GetDC((HWND)fake_window.os_handle);
	defer(ReleaseDC((HWND)fake_window.os_handle, window_context));

	PIXELFORMATDESCRIPTOR dpf;
	dpf.nSize = sizeof(dpf);
	dpf.nVersion = 1;
	dpf.iPixelType = PFD_TYPE_RGBA;
	dpf.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	dpf.cColorBits = 32;
	dpf.cDepthBits = 24;
	dpf.cAlphaBits = 8;
	dpf.iLayerType = PFD_MAIN_PLANE;

	const s32 suggested_pixel_format_index = ChoosePixelFormat(window_context, &dpf);

	DescribePixelFormat(window_context, suggested_pixel_format_index, sizeof(dpf), &dpf);
	SetPixelFormat(window_context, suggested_pixel_format_index, &dpf);

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
#define CHECK_GL_BINDINGS(type, func) if (!func) return false;
	GL_BINDINGS(CHECK_GL_BINDINGS);
#undef  CHECK_GL_BINDINGS
	return true;
}

bool ch::swap_buffers(OS_Window_Handle window_handle) {
	HDC window_context = GetDC((HWND)window_handle);
	defer(ReleaseDC((HWND)window_handle, window_context));
	return SwapBuffers(window_context);
}

bool ch::create_gl_window(const char* title, u32 width, u32 height, u32 style, ch::Window* out_window) {
	if (!ch::create_window(title, width, height, style, out_window)) {
		return false;
	}

	HDC window_context = GetDC((HWND)out_window->os_handle);
	defer(ReleaseDC((HWND)out_window->os_handle, window_context));
	const s32 attrib_list[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
		0,
	};

	s32 pixel_format = 0;
	u32 num_formats;
	wglChoosePixelFormatARB(window_context, attrib_list, NULL, 1, &pixel_format, &num_formats);

	PIXELFORMATDESCRIPTOR spf;
	DescribePixelFormat(window_context, pixel_format, sizeof(spf), &spf);
	SetPixelFormat(window_context, pixel_format, &spf);

	return true;
}

bool ch::make_current(OS_Window_Handle window_handle) {
	HDC window_context = GetDC((HWND)window_handle);
	defer(ReleaseDC((HWND)window_handle, window_context));

	const s32 win32_opengl_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};


	HGLRC glrc = wglCreateContextAttribsARB(window_context, 0, win32_opengl_attribs);

	return wglMakeCurrent(window_context, glrc);
}