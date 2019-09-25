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
        OS_Window_Handle os_handle = nullptr;
		u32 style = 0;
		ch::Vector2 min_size = (s32)800;

		Window() = default;

        explicit operator bool() const { return os_handle; }
        operator OS_Window_Handle() const { return os_handle; }
        operator OS_Window_Handle() { return os_handle; }

		bool get_mouse_position(ch::Vector2* out_pos) const;
		ch::Vector2 get_size() const;
		ch::Vector2 get_viewport_size() const;
		bool is_visible() const;
		bool has_focus() const;
		bool is_maximized() const;

        void set_visibility(bool visibility);
		void center_in_monitor();
		void maximize();
        void free();

        using On_Exit_Requested = void(*)(const Window& window);
        On_Exit_Requested on_exit_requested = nullptr;

        using On_Resize = void(*)(const Window& window);
        On_Resize on_resize = nullptr;

		using On_Sizing = void(*)(const Window& window);
		On_Sizing on_sizing = nullptr;

		using On_Focus_Gained = void(*)(const Window& window);
		On_Focus_Gained on_focus_gained = nullptr;

		using On_Focus_Lost = void(*)(const Window& window);
		On_Focus_Lost on_focus_lost = nullptr;

		using On_Mouse_Button_Down = void(*)(const Window& window, u8 mouse_button);
		On_Mouse_Button_Down on_mouse_button_down = nullptr;

		using On_Mouse_Button_Up = void(*)(const Window& window, u8 mouse_button);
		On_Mouse_Button_Up on_mouse_button_up = nullptr;

		using On_Key_Pressed = void(*)(const Window& window, u8 key);
		On_Key_Pressed on_key_pressed = nullptr;

		using On_Key_Released = void(*)(const Window& window, u8 key);
		On_Key_Released on_key_released = nullptr;

		using On_Char_Entered = void(*)(const Window& window, u32 c);
		On_Char_Entered on_char_entered = nullptr;

		using On_Mouse_Wheel_Scrolled = void(*)(const Window& window, f32 delta);
		On_Mouse_Wheel_Scrolled on_mouse_wheel_scrolled = nullptr;

		using On_Maximized = void(*)(const Window& window);
		On_Maximized on_maximized = nullptr;
    };

    bool create_window(const char* title, u32 width, u32 height, u32 style, ch::Window* out_window);
    void poll_events();
    void wait_events();
}