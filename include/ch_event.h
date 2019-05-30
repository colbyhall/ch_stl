#pragma once

#include "ch_math.h"

namespace ch {
	struct Event;

	using Event_Listener = void(*)(void* owner, Event& event);

	// @TODO(CHall): Add more event types
	enum Event_Type {
		ET_Window_Resize,
		ET_Window_Exit_Requested,
		ET_Window_Key_Pressed,
		ET_Window_Key_Released,
		ET_Window_Mouse_Down,
		ET_Window_Mouse_Up,
		ET_Window_Char_Entered,
		ET_Window_Gained_Focus,
		ET_WIndow_Lost_Focus,
	};

	struct Event {
		Event_Type type;

		union {
			struct {
				u32 old_width;
				u32 old_height;
			};
			Vector2 mouse_position;
			float delta;
			u32 key_code;
			u8 c;
		};

		bool handled;
	};

}
