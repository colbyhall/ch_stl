#pragma once

#include "ch_string.h"
#include "ch_os.h"

namespace ch {

    enum Window_Style {
        WS_Borderless = 0x01,
        WS_Can_Maximize = 0x02,
        WS_Can_Minimize = 0x04,
        WS_Visible = 0x08,
    };

    struct Window {
        OS_Window_Handle os_handle;

        ch::String title;
        u32 width, height;
        bool is_visible;
        u32 style;

        explicit operator bool() const { return os_handle; }
        operator OS_Window_Handle() const { return os_handle; }
        operator OS_Window_Handle() { return os_handle; }

        void set_visibility(bool visibility);
        void destroy();

        using On_Exit_Requested = void(*)(const Window& window);
        On_Exit_Requested on_exit_requested;

        using On_Resize = void(*)(const Window& window, u32 old_width, u32 old_height);
        On_Resize on_resize;
    };

    bool create_window(const tchar* title, u32 width, u32 height, u32 style, ch::Window* out_window);
    void poll_events();
}