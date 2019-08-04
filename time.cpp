#include "time.h"

ch::Scoped_Timer_Manager ch::scoped_timer_manager;

ch::Scoped_Timer::Scoped_Timer(const tchar* _name) : start(ch::get_time_in_seconds()), name(_name) { }

ch::Scoped_Timer::~Scoped_Timer() {
	end = ch::get_time_in_seconds();

	ch::scoped_timer_manager.entries.insert(*this, 0);
}