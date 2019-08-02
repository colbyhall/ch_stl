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

	CH_FORCEINLINE const tchar* get_month_string(u32 month) {
		switch (month) {
			case DTM_January:
				return CH_TEXT("January");
			case DTM_February:
				return CH_TEXT("February");
			case DTM_March:
				return CH_TEXT("March");
			case DTM_April:
				return CH_TEXT("April");
			case DTM_May:
				return CH_TEXT("May");
			case DTM_June:
				return CH_TEXT("June");
			case DTM_July:
				return CH_TEXT("July");
			case DTM_August:
				return CH_TEXT("August");
			case DTM_September:
				return CH_TEXT("September");
			case DTM_October:
				return CH_TEXT("October");
			case DTM_November:
				return CH_TEXT("November");
			case DTM_December:
				return CH_TEXT("December");
		}

		return nullptr;
	}

	CH_FORCEINLINE const tchar* get_days_of_week_string(u32 dow) {
		switch (dow) {
		case DTDOW_Sunday:
			return CH_TEXT("Sunday");
		case DTDOW_Monday:
			return CH_TEXT("Monday");
		case DTDOW_Tuesday:
			return CH_TEXT("Tuesday");
		case DTDOW_Wednesday:
			return CH_TEXT("Wednesday");
		case DTDOW_Thursday:
			return CH_TEXT("Thursday");
		case DTDOW_Friday:
			return CH_TEXT("Friday");
		case DTDOW_Saturday:
			return CH_TEXT("Saturda");
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
		stream << dt.month << '/' << dt.day_of_month << '/' << dt.year << ' ' << dt.hour << ':' << dt.minute << CH_TEXT(" (UTC)");
		
		return stream;
	}

	bool date_time_from_file_time(u64 file_time, ch::Date_Time* out_time);

	ch::Date_Time get_system_time();
	ch::Date_Time get_local_time();

	f64 get_time_in_seconds();

	struct Scoped_Timer {
		const f64 start;
		const tchar* name;
		Scoped_Timer(const tchar* _name) : start(ch::get_time_in_seconds()), name(_name) {}
		~Scoped_Timer() {
			const f64 end = ch::get_time_in_seconds();
			const f64 gap = end - start;

			// ch::std_out << name << CH_TEXT(": ") << gap << ch::eol;
		}
	};

#define CH_SCOPED_TIMER(name) ch::Scoped_Timer name(#name);

}