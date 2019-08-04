#include "time.h"

static ch::Scoped_Timer_Manager g_scoped_timer_manager;

ch::Scoped_Timer::Scoped_Timer(const tchar* _name) : start(ch::get_time_in_seconds()), name(_name) { }

ch::Scoped_Timer::~Scoped_Timer() {
	end = ch::get_time_in_seconds();

	ch::Scoped_Timer_Manager::get().entries.insert(*this, 0);
}

ch::Scoped_Timer_Manager& ch::Scoped_Timer_Manager::get() {
	return g_scoped_timer_manager;
}
