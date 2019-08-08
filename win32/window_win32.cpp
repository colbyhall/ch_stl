#include "../window.h"
#include "../input.h"

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
	case WM_SIZE:
		CALL_WINDOW_EVENT(on_resize);
		break;
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
	case WM_MOUSEWHEEL:
		CALL_WINDOW_EVENT(on_mouse_wheel_scrolled, GET_WHEEL_DELTA_WPARAM(w_param));
		break;
	case WM_LBUTTONDOWN:
		SetCapture((HWND)window->os_handle);
		CALL_WINDOW_EVENT(on_mouse_button_down, CH_MOUSE_LEFT);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		CALL_WINDOW_EVENT(on_mouse_button_up, CH_MOUSE_LEFT);
		break;
	case WM_MBUTTONDOWN:
		SetCapture((HWND)window->os_handle);
		CALL_WINDOW_EVENT(on_mouse_button_down, CH_MOUSE_MIDDLE);
		break;
	case WM_MBUTTONUP:
		ReleaseCapture();
		CALL_WINDOW_EVENT(on_mouse_button_up, CH_MOUSE_MIDDLE);
		break;
	case WM_RBUTTONDOWN:
		SetCapture((HWND)window->os_handle);
		CALL_WINDOW_EVENT(on_mouse_button_down, CH_MOUSE_RIGHT);
		break;
	case WM_RBUTTONUP:
		ReleaseCapture();
		CALL_WINDOW_EVENT(on_mouse_button_up, CH_MOUSE_RIGHT);
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
	window_class.style = CS_OWNDC;

    if (!RegisterClassEx(&window_class)) {
        return false;
    }

    HWND window_handle = CreateWindow(window_class.lpszClassName, title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, window_class.hInstance, NULL);
    if (!window_handle) {
        return false;
    }

    SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)out_window);

    out_window->style = style;
    out_window->os_handle = window_handle;

	const ch::Vector2 size = out_window->get_size();
	const ch::Vector2 viewport_size = out_window->get_viewport_size();
	SetWindowPos((HWND)out_window->os_handle, 0, 0, 0, size.ux, (size.uy - viewport_size.uy) + size.uy, SWP_NOMOVE | SWP_NOZORDER);

	out_window->center_in_monitor();

    return true;
}

void ch::poll_events() {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool ch::Window::get_mouse_position(ch::Vector2* out_pos) const {
	POINT p;
	if (GetCursorPos(&p)) {
		if (ScreenToClient((HWND)os_handle, &p)) {
			out_pos->ux = (u32)p.x;
			out_pos->uy = (u32)p.y;

			const ch::Vector2 client_size = get_viewport_size();

			return p.x >= 0 && p.y >= 0 && (u32)p.x < client_size.ux && (u32)p.y < client_size.uy;
		}
	}

	return false;
}

ch::Vector2 ch::Window::get_size() const {
	RECT rect;
	GetWindowRect((HWND)os_handle, &rect);

	ch::Vector2 result;
	result.ux = rect.right - rect.left;
	result.uy = rect.bottom - rect.top;

	return result;
}

ch::Vector2 ch::Window::get_viewport_size() const {
	RECT rect;
	GetClientRect((HWND)os_handle, &rect);

	ch::Vector2 result;
	result.ux = rect.right - rect.left;
	result.uy = rect.bottom - rect.top;

	return result;
}

bool ch::Window::is_visible() const {
	return IsWindowVisible((HWND)os_handle);
}

bool ch::Window::has_focus() const {
	return GetFocus() == os_handle;
}

void ch::Window::set_visibility(bool visibility) {
    ShowWindow((HWND)os_handle, (visibility ? SW_SHOW : SW_HIDE));
}

void ch::Window::center_in_monitor() {
	const u32 monitor_width = GetSystemMetrics(SM_CXSCREEN);
	const u32 monitor_height = GetSystemMetrics(SM_CYSCREEN);

	const ch::Vector2 size = get_size();

	const u32 pos_x = monitor_width / 2 - size.ux / 2;
	const u32 pos_y = monitor_height / 2 - size.uy / 2;

	SetWindowPos((HWND)os_handle, 0, pos_x, pos_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void ch::Window::destroy() {
    if (os_handle) {
        DestroyWindow((HWND)os_handle);
        os_handle = nullptr;
    }
}
