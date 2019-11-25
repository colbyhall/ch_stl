#pragma once

#include "types.h"
#include "filesystem.h"

namespace ch {

	enum DT_Month {
		DTM_January = 1,
		DTM_February,
		DTM_March,
		DTM_April,
		DTM_May,
		DTM_June,
		DTM_July,
		DTM_August,
		DTM_September,
		DTM_October,
		DTM_November,
		DTM_December,
	};

	enum DT_Days_Of_Week {
		DTDOW_Sunday = 0,
		DTDOW_Monday,
		DTDOW_Tuesday,
		DTDOW_Wednesday,
		DTDOW_Thursday,
		DTDOW_Friday,
		DTDOW_Saturday,
	};

	CH_FORCEINLINE const char* get_month_string(u32 month) {
		switch (month) {
			case DTM_January:
				return "January";
			case DTM_February:
				return "February";
			case DTM_March:
				return "March";
			case DTM_April:
				return "April";
			case DTM_May:
				return "May";
			case DTM_June:
				return "June";
			case DTM_July:
				return "July";
			case DTM_August:
				return "August";
			case DTM_September:
				return "September";
			case DTM_October:
				return "October";
			case DTM_November:
				return "November";
			case DTM_December:
				return "December";
		}

		return nullptr;
	}

	CH_FORCEINLINE const char* get_days_of_week_string(u32 dow) {
		switch (dow) {
		case DTDOW_Sunday:
			return "Sunday";
		case DTDOW_Monday:
			return "Monday";
		case DTDOW_Tuesday:
			return "Tuesday";
		case DTDOW_Wednesday:
			return "Wednesday";
		case DTDOW_Thursday:
			return "Thursday";
		case DTDOW_Friday:
			return "Friday";
		case DTDOW_Saturday:
			return "Saturday";
		}

		return nullptr;
	}

	struct Date_Time {
		u32 year;
		u32 month;
		u32 day_of_week;
		u32 day_of_month;
		u32 hour;
		u32 minute;
		u32 second;
		u32 milliseconds;

		Date_Time() = default;
	};

	CH_FORCEINLINE ch::Stream& operator<<(ch::Stream& stream, const ch::Date_Time& dt) {
		stream << dt.month << '/' << dt.day_of_month << '/' << dt.year << ' ' << dt.hour << ':' << dt.minute << " (UTC)";
		
		return stream;
	}

	bool date_time_from_file_time(u64 file_time, ch::Date_Time* out_time);

	ch::Date_Time get_system_time();
	ch::Date_Time get_local_time();
    void sleep(u32 milliseconds);

	f64 get_time_in_seconds();

	struct Scoped_Timer {
		f64 start;
		f64 end;
		const char* name;

		CH_FORCEINLINE f64 get_gap() const { return end - start; }

		Scoped_Timer() = default;
		Scoped_Timer(const char* _name);
		~Scoped_Timer();
	};

#define CH_SCOPED_TIMER(name) ch::Scoped_Timer name(#name);

	struct Scoped_Timer_Manager {

		ch::Array<ch::Scoped_Timer> entries;

		Scoped_Timer_Manager() {
			entries.allocator = ch::get_heap_allocator();
		}

		CH_FORCEINLINE void reset() { entries.count = 0; }
	};

	extern ch::Scoped_Timer_Manager scoped_timer_manager;
}