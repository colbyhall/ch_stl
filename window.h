#pragma once

#include "string.h"
#include "os.h"
#include "math.h"

namespace ch {

    enum Window_Style {
        WS_Borderless = 0x01,
        WS_Can_Maximize = 0x02,
        WS_Can_Minimize = 0x04,
        WS_Visible = 0x08,
    };

    struct Window {
        OS_Window_Handle os_handle;
		u32 style;

        explicit operator bool() const { return os_handle; }
        operator OS_Window_Handle() const { return os_handle; }
        operator OS_Window_Handle() { return os_handle; }

		bool get_mouse_position(ch::Vector2* out_pos) const;
		ch::Vector2 get_size() const;
		ch::Vector2 get_viewport_size() const;
		bool is_visible() const;
		bool has_focus() const;

        void set_visibility(bool visibility);
        void destroy();

        using On_Exit_Requested = void(*)(const Window& window);
        On_Exit_Requested on_exit_requested;

        using On_Resize = void(*)(const Window& window);
        On_Resize on_resize;

		using On_Sizing = void(*)(const Window& window);
		On_Sizing on_sizing;

		using On_Focus_Gained = void(*)(const Window& window);
		On_Focus_Gained on_focus_gained;

		using On_Focus_Lost = void(*)(const Window& window);
		On_Focus_Lost on_focus_lost;

		using On_Mouse_Down = void(*)(const Window& window, u8 mouse_button);
		On_Mouse_Down on_mouse_down;

		using On_Mouse_Up = void(*)(const Window& window, u8 mouse_button);
		On_Mouse_Up on_mouse_up;

		using On_Key_Pressed = void(*)(const Window& window, u8 key);
		On_Key_Pressed on_key_pressed;

		using On_Key_Released = void(*)(const Window& window, u8 key);
		On_Key_Released on_key_released;

		using On_Char_Entered = void(*)(const Window& window, u32 c);
		On_Char_Entered on_char_entered;
    };

    bool create_window(const tchar* title, u32 width, u32 height, u32 style, ch::Window* out_window);
    void poll_events();
}