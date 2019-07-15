#include "../opengl.h"

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
    {
        WNDCLASSEX window_class = {};
        window_class.cbSize = sizeof(WNDCLASSEX);
        window_class.lpfnWndProc = DefWindowProc;
        window_class.hInstance = GetModuleHandle(NULL);
        window_class.lpszClassName = TEXT("gl_loader");

        if (RegisterClassEx(&window_class)) {
            HWND window = CreateWindowEx(0, window_class.lpszClassName, TEXT("gl_loader window"), 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, window_class.hInstance, NULL);

            HDC window_context = GetDC(window);

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
            if (wglMakeCurrent(window_context, glrc)) {
                wglChoosePixelFormatARB = (WGL_Choose_Pixel_Format_ARB)wglGetProcAddress("wglChoosePixelFormatARB");
                wglCreateContextAttribsARB = (WGL_Create_Context_Attribs_ARB)wglGetProcAddress("wglCreateContextAttribsARB");
                wglSwapIntervalEXT = (WGL_Swap_Interval_Ext)wglGetProcAddress("wglSwapIntervalEXT");
                wglMakeCurrent(0, 0);
            }

            wglDeleteContext(glrc);
            ReleaseDC(window, window_context);
            DestroyWindow(window);
        } else {
            return false;
        }
    }

    ch::Window window;
    if (!create_gl_window(TEXT("load gl functions"), 12, 12, 0, &window)) {
        return false;
    }
    defer(window.destroy());

    HDC window_context = GetDC((HWND)window.os_handle);

    if (wglCreateContextAttribsARB) {
        HGLRC glrc = wglCreateContextAttribsARB(window_context, 0, win32_opengl_attribs);

        if (wglMakeCurrent(window_context, glrc)) {

#define LOAD_GL_BINDINGS(type, func) func = (type)wglGetProcAddress(#func);
            GL_BINDINGS(LOAD_GL_BINDINGS);
#undef  LOAD_GL_BINDINGS

            bool result = true;
#define CHECK_GL_BINDINGS(type, func) result = result && (func != nullptr);
            GL_BINDINGS(CHECK_GL_BINDINGS);
#undef  CHECK_GL_BINDINGS

            return result;
        }
    }

    return false;
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

    HDC window_context = GetDC((HWND)out_window->os_handle);

    if (wglChoosePixelFormatARB) {
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

        s32 spf_index = 0;
        u32 num_formats;
        wglChoosePixelFormatARB(window_context, attrib_list, 0, 1, &spf_index, &num_formats);

        PIXELFORMATDESCRIPTOR spf;
        DescribePixelFormat(window_context, spf_index, sizeof(spf), &spf);
        SetPixelFormat(window_context, spf_index, &spf);
        return true;
    }

    return false;
}

bool ch::make_current(OS_Window_Handle window_handle) {
    HDC window_context = GetDC((HWND)window_handle);

    if (wglCreateContextAttribsARB) {
        HGLRC glrc = wglCreateContextAttribsARB(window_context, 0, win32_opengl_attribs);

        return wglMakeCurrent(window_context, glrc);
    }

    return false;
}