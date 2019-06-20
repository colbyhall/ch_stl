#include "../ch_window.h"

#if !CH_PLATFORM_WINDOWS
#error This should not be compiling on this platform
#endif

#define WIN32_MEAN_AND_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>


#define CALL_WINDOW_EVENT(event, ...) if (window->event) window->event(*window, __VA_ARGS__)
static LRESULT window_proc(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
    ch::Window* window = (ch::Window*)GetWindowLongPtr(handle, GWLP_USERDATA);

	// @TODO(CHall): I still need to do mouse events. and char entered
    switch (message) {
    case WM_DESTROY:
		CALL_WINDOW_EVENT(on_exit_requested);
        break;
	case WM_SIZE: {
        RECT rect;
        GetClientRect(handle, &rect);

        const u32 old_width = window->width;
        const u32 old_height = window->height;

        window->width = rect.right - rect.left;
        window->height = rect.bottom - rect.top;
		CALL_WINDOW_EVENT(on_resize, old_width, old_height);
	} break;
	case WM_SIZING:
		CALL_WINDOW_EVENT(on_sizing);
		break;
	case WM_SETFOCUS:
		CALL_WINDOW_EVENT(on_focus_gained);
		break;
	case WM_KILLFOCUS:
		CALL_WINDOW_EVENT(on_focus_lost);
		break;
	case WM_KEYDOWN:
		CALL_WINDOW_EVENT(on_key_pressed, (u8)w_param);
		break;
	case WM_KEYUP:
		CALL_WINDOW_EVENT(on_key_released, (u8)w_param);
		break;
	} 


    return DefWindowProc(handle, message, w_param, l_param);
}
#undef CALL_WINDOW_EVENT

bool ch::create_window(const tchar* title, u32 width, u32 height, u32 style, Window* out_window) {
    assert(out_window);

    *out_window = {};

    WNDCLASSEX window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszClassName = title;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassEx(&window_class)) {
        return false;
    }

    const u32 monitor_width = GetSystemMetrics(SM_CXSCREEN);
    const u32 monitor_height = GetSystemMetrics(SM_CYSCREEN);

    const u32 pos_x = monitor_width / 2 - width / 2;
    const u32 pos_y = monitor_height / 2 - height / 2;

    HWND window_handle = CreateWindow(window_class.lpszClassName, title, WS_OVERLAPPEDWINDOW, pos_x, pos_y, width, height, NULL, NULL, window_class.hInstance, NULL);
    if (!window_handle) {
        return false;
    }

    SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)out_window);

    out_window->title = title;
    out_window->width = width;
    out_window->height = height;
    out_window->style = style;
    out_window->os_handle = window_handle;
    out_window->is_visible = false;

    return true;
}

void ch::poll_events() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ch::Window::set_visibility(bool visibility) {
    ShowWindow((HWND)os_handle, (visibility ? SW_SHOW : SW_HIDE));
    is_visible = visibility;
}

void ch::Window::destroy() {
    if (title) {
        title.destroy();
    }

    if (os_handle) {
        DestroyWindow((HWND)os_handle);
        os_handle = nullptr;
    }
}
