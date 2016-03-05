/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Mark Page
**    Harry Storbacka
*/

#include "UICore/precomp.h"
#include "UICore/Core/System/datetime.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#ifndef WIN32
#include <cstring>
#include <ctime>
#endif

namespace uicore
{
#ifndef WIN32
	const int64_t DateTime::ticks_from_1601_to_1900 = 94354848000000000LL;
#endif

	DateTime::DateTime()
		: _year(0), _month(0), _day(0), _hour(0), _minute(0), _seconds(0), _nanoseconds(0), _timezone(utc_timezone)
	{
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int seconds, int nanoseconds, TimeZone timezone)
		: _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _seconds(seconds), _nanoseconds(nanoseconds), _timezone(timezone)
	{
		throw_if_invalid_date(year, month, day, hour, minute, seconds, nanoseconds);
	}

	DateTime::~DateTime()
	{
	}

	DateTime DateTime::current_local_time()
	{
		return current_utc_time().to_local();
	}

	DateTime DateTime::current_utc_time()
	{
#ifdef WIN32
		SYSTEMTIME system_time;
		GetSystemTime(&system_time);
		DateTime datetime;
		datetime._timezone = utc_timezone;
		datetime._year = system_time.wYear;
		datetime._month = system_time.wMonth;
		datetime._day = system_time.wDay;
		datetime._hour = system_time.wHour;
		datetime._minute = system_time.wMinute;
		datetime._seconds = system_time.wSecond;
		datetime._nanoseconds = system_time.wMilliseconds * 1000000;
		return datetime;
#else
		time_t unix_ticks = 0;
		unix_ticks = time(&unix_ticks);
		if (unix_ticks == -1)
			throw Exception("Failed to get current UTC time");
		int64_t ticks = ticks_from_1601_to_1900 + ((int64_t) unix_ticks) * 10000000;
		return DateTime::utc_time_from_ticks(ticks);
#endif
	}

	DateTime DateTime::local_time_from_ticks(int64_t ticks)
	{
		return utc_time_from_ticks(ticks).to_local();
	}

	DateTime DateTime::utc_time_from_ticks(int64_t ticks)
	{
		DateTime datetime;
		datetime._timezone = utc_timezone;
#ifdef WIN32
		SYSTEMTIME system_time;
		FILETIME file_time;
		file_time.dwHighDateTime = ticks >> 32;
		file_time.dwLowDateTime = ticks & 0xffffffff;
		BOOL result = FileTimeToSystemTime(&file_time, &system_time);
		if (result == FALSE)
			throw Exception("FileTimeToSystemTime failed");
		datetime._year = system_time.wYear;
		datetime._month = system_time.wMonth;
		datetime._day = system_time.wDay;
		datetime._hour = system_time.wHour;
		datetime._minute = system_time.wMinute;
		datetime._seconds = system_time.wSecond;
		datetime._nanoseconds = system_time.wMilliseconds * 1000000;
		result = SystemTimeToFileTime(&system_time, &file_time);
		if (result == FALSE)
			throw Exception("SystemTimeToFileTime failed");
		int64_t new_ticks = (((int64_t)file_time.dwHighDateTime) << 32) + file_time.dwLowDateTime;
		datetime._nanoseconds += (ticks - new_ticks) * 100;
#else
		tm tm_utc;
		memset(&tm_utc, 0, sizeof(tm));
		time_t unix_ticks = (ticks - ticks_from_1601_to_1900) / 10000000;
		tm *result = gmtime_r(&unix_ticks, &tm_utc);
		if (result == nullptr)
			throw Exception("gmtime_r failed");
		datetime._year = ((int)result->tm_year)+1900;
		datetime._month = result->tm_mon+1;
		datetime._day = result->tm_mday;
		datetime._hour = result->tm_hour;
		datetime._minute = result->tm_min;
		datetime._seconds = result->tm_sec;
		datetime._nanoseconds = (ticks % 10000000) * 100;
#endif
		return datetime;
	}

	int64_t DateTime::to_ticks() const
	{
		throw_if_null();
		if (_timezone == local_timezone)
		{
			return to_utc().to_ticks();
		}
		else
		{
#ifdef WIN32
			SYSTEMTIME system_time;
			FILETIME file_time;
			system_time.wYear = _year;
			system_time.wMonth = _month;
			system_time.wDay = _day;
			system_time.wHour = _hour;
			system_time.wMinute = _minute;
			system_time.wSecond = _seconds;
			system_time.wMilliseconds = _nanoseconds / 1000000;
			BOOL result = SystemTimeToFileTime(&system_time, &file_time);
			if (result == FALSE)
				throw Exception("SystemTimeToFileTime failed");
			int64_t ticks = (((int64_t)file_time.dwHighDateTime) << 32) + file_time.dwLowDateTime;
			ticks += (_nanoseconds % 1000000) / 100;
			return ticks;
#else
			int64_t ticks;

			ticks = _year - 1601;
			ticks *= 365;	// Days in a year

			// Count number of leap years
			int current_year = _year;
			int num_leaps = 0;
			for (int cnt=1601; cnt < current_year; cnt++)
			{
				if ( ( ((cnt % 4) == 0) && ((cnt % 100 !=0)) ) || ((cnt % 400)==0) )
				{
					num_leaps++;
				}
			}
			ticks += num_leaps;	// Extra days

			switch (_month)
			{
			case (12):
				ticks += 30;	// Nov
			case (11):
				ticks += 31;	// Oct
			case (10):
				ticks += 30;	// Sep
			case (9):
				ticks += 31;	// Aug
			case (8):
				ticks += 31;	// Jul
			case (7):
				ticks += 30;	// Jun
			case (6):
				ticks += 31;	// May
			case (5):
				ticks += 30;	// Apr
			case (4):
				ticks += 31;	// Mar
			case (3):
				ticks += 28;	// Feb
				if ( ( ((current_year % 4) == 0) && ((current_year % 100 !=0)) ) || ((current_year % 400)==0) )
				{
					ticks++;	// Leap year
				}
			case (2):
				ticks += 31;	// Jan
				break;
			}

			ticks += _day - 1;

			ticks *= 24;	// Hours in day
			ticks += _hour;
			ticks *= 60;	// Minutes per hour
			ticks += _minute;
			ticks *= 60;	// Seconds per minute
			ticks += _seconds;
			ticks *= 10000000;	// To ticks
			ticks += _nanoseconds / 100;

			return ticks;
#endif
		}
	}

	DateTime DateTime::to_utc() const
	{
		throw_if_null();
		if (_timezone == utc_timezone)
		{
			return *this;
		}
		else
		{
#ifdef WIN32
			SYSTEMTIME local_time;
			SYSTEMTIME system_time;
			local_time.wYear = _year;
			local_time.wMonth = _month;
			local_time.wDay = _day;
			local_time.wHour = _hour;
			local_time.wMinute = _minute;
			local_time.wSecond = _seconds;
			local_time.wMilliseconds = _nanoseconds / 1000000;
#if _MSC_VER
			BOOL result = TzSpecificLocalTimeToSystemTime(0, &local_time, &system_time);
#else
			BOOL result = SystemTimeToTzSpecificLocalTime(0, &system_time, &local_time);
#endif
			if (result == FALSE)
				throw Exception("TzSpecificLocalTimeToSystemTime failed");

			DateTime datetime;
			datetime._timezone = utc_timezone;
			datetime._year = system_time.wYear;
			datetime._month = system_time.wMonth;
			datetime._day = system_time.wDay;
			datetime._hour = system_time.wHour;
			datetime._minute = system_time.wMinute;
			datetime._seconds = system_time.wSecond;
			datetime._nanoseconds = system_time.wMilliseconds * 1000000;
			datetime._nanoseconds += (_nanoseconds % 1000000);
			return datetime;
#else
			tm tm_local;
			memset(&tm_local, 0, sizeof(tm));
			tm_local.tm_year = _year-1900;
			tm_local.tm_mon = _month-1;
			tm_local.tm_mday = _day;
			tm_local.tm_hour = _hour;
			tm_local.tm_min = _minute;
			tm_local.tm_sec = _seconds;
			tm_local.tm_isdst = -1;
			time_t unix_ticks = mktime(&tm_local);
			if (unix_ticks == -1)
				throw Exception("mktime failed");

			memset(&tm_local, 0, sizeof(tm));
			tm *result = gmtime_r(&unix_ticks, &tm_local);
			if (result == nullptr)
				throw Exception("gmtime_r failed");

			DateTime datetime;
			datetime._timezone = utc_timezone;
			datetime._year = result->tm_year+1900;
			datetime._month = result->tm_mon+1;
			datetime._day = result->tm_mday;
			datetime._hour = result->tm_hour;
			datetime._minute = result->tm_min;
			datetime._seconds = result->tm_sec;
			datetime._nanoseconds = _nanoseconds;
			return datetime;
#endif
		}
	}

	DateTime DateTime::to_local() const
	{
		throw_if_null();
		if (_timezone == local_timezone)
		{
			return *this;
		}
		else
		{
#ifdef WIN32
			SYSTEMTIME local_time;
			SYSTEMTIME system_time;
			system_time.wYear = _year;
			system_time.wMonth = _month;
			system_time.wDay = _day;
			system_time.wHour = _hour;
			system_time.wMinute = _minute;
			system_time.wSecond = _seconds;
			system_time.wMilliseconds = _nanoseconds / 1000000;
			BOOL result = SystemTimeToTzSpecificLocalTime(0, &system_time, &local_time);
			if (result == FALSE)
				throw Exception("SystemTimeToTzSpecificLocalTime failed");

			DateTime datetime;
			datetime._timezone = local_timezone;
			datetime._year = local_time.wYear;
			datetime._month = local_time.wMonth;
			datetime._day = local_time.wDay;
			datetime._hour = local_time.wHour;
			datetime._minute = local_time.wMinute;
			datetime._seconds = local_time.wSecond;
			datetime._nanoseconds = local_time.wMilliseconds * 1000000;
			datetime._nanoseconds += (_nanoseconds % 1000000);
			return datetime;
#else

			time_t unix_ticks;

			unix_ticks = _year - 1970;
			unix_ticks *= 365;	// Days in a year

			// Count number of leap years
			int current_year = _year;
			int num_leaps = 0;
			for (int cnt=1970; cnt < current_year; cnt++)
			{
				if ( ( ((cnt % 4) == 0) && ((cnt % 100 !=0)) ) || ((cnt % 400)==0) )
				{
					num_leaps++;
				}
			}
			unix_ticks += num_leaps;	// Extra days

			switch (_month)
			{
			case (12):
				unix_ticks += 30;	// Nov
			case (11):
				unix_ticks += 31;	// Oct
			case (10):
				unix_ticks += 30;	// Sep
			case (9):
				unix_ticks += 31;	// Aug
			case (8):
				unix_ticks += 31;	// Jul
			case (7):
				unix_ticks += 30;	// Jun
			case (6):
				unix_ticks += 31;	// May
			case (5):
				unix_ticks += 30;	// Apr
			case (4):
				unix_ticks += 31;	// Mar
			case (3):
				unix_ticks += 28;	// Feb
				if ( ( ((current_year % 4) == 0) && ((current_year % 100 !=0)) ) || ((current_year % 400)==0) )
				{
					unix_ticks++;	// Leap year
				}
			case (2):
				unix_ticks += 31;	// Jan
				break;
			}

			unix_ticks += _day - 1;

			unix_ticks *= 24;	// Hours in day
			unix_ticks += _hour;
			unix_ticks *= 60;	// Minutes per hour
			unix_ticks += _minute;
			unix_ticks *= 60;	// Seconds per minute
			unix_ticks += _seconds;

			tm tm_local;
			memset(&tm_local, 0, sizeof(tm));

			tm *result = localtime_r(&unix_ticks, &tm_local);
			if (result == nullptr)
				throw Exception("localtime_r failed");

			DateTime datetime;
			datetime._timezone = local_timezone;
			datetime._year = result->tm_year+1900;
			datetime._month = result->tm_mon+1;
			datetime._day = result->tm_mday;
			datetime._hour = result->tm_hour;
			datetime._minute = result->tm_min;
			datetime._seconds = result->tm_sec;
			datetime._nanoseconds = _nanoseconds;
			return datetime;
#endif
		}
	}

	bool DateTime::is_null() const
	{
		return (_year == 0);
	}

	unsigned short DateTime::year() const
	{
		throw_if_null();
		return _year;
	}

	unsigned char DateTime::month() const
	{
		throw_if_null();
		return _month;
	}

	unsigned char DateTime::day() const
	{
		throw_if_null();
		return _day;
	}

	unsigned char DateTime::hour() const
	{
		throw_if_null();
		return _hour;
	}

	unsigned char DateTime::minutes() const
	{
		throw_if_null();
		return _minute;
	}

	unsigned char DateTime::seconds() const
	{
		throw_if_null();
		return _seconds;
	}

	unsigned int DateTime::nanoseconds() const
	{
		throw_if_null();
		return _nanoseconds;
	}

	DateTime::TimeZone DateTime::timezone() const
	{
		throw_if_null();
		return _timezone;
	}

	unsigned char DateTime::week() const
	{
		throw_if_null();
		int day_of_week_val = 1 + ((day_of_week() + 6) % 7);
		DateTime dt = *this;
		DateTime nearest_thursday = dt.add_days(4 - day_of_week_val);
		DateTime jan1(nearest_thursday._year, 1, 1);
		int days = jan1.difference_in_days(nearest_thursday);
		int week = 1 + days / 7; // Count of Thursdays 
		return week;
	}

	unsigned int DateTime::day_of_week() const
	{
		throw_if_null();
		if (_year < 1600)
			throw Exception("Unsupported date specified");

		int century_anchor_days[4] =
		{
			2, // 1600: Tuesday
			0, // 1700: Sunday
			5, // 1800: Friday
			3  // 1900: Wednesday
		};

		int century_value = _year / 100;
		int century_day = century_anchor_days[century_value % 4];
		int y = _year % 100;
		int a = y / 12;
		int b = y % 12;
		int c = b / 4;
		int d = a + b + c;
		int doomsday = (century_day + d % 7) % 7; // Doomsday of this year

		int month_doomsdays[12] = { 3, 7, 7, 4, 2, 6, 4, 1, 5, 3, 7, 5 }; // Days in each month that are doomsdays
		bool leap_year = (_year % 4) == 0; // Leap years are every 4th year
		if ((_year % 100) == 0 && (_year % 400) != 0) // Except for those divisible by 100 unless they are also divisible by 400
			leap_year = false;
		if (leap_year)
		{
			month_doomsdays[0] = 4;
			month_doomsdays[1] = 8;
		}

		int day_of_week = (doomsday + _day - month_doomsdays[_month - 1]) % 7;
		if (day_of_week < 0)
			day_of_week += 7;
		return day_of_week;
	}

	int DateTime::difference_in_days(const DateTime &other) const
	{
		int day1 = day_number();
		int day2 = other.day_number();
		return day2 - day1;
	}

	int DateTime::days_in_month(int month, int year)
	{
		if (month < 1 || month > 12)
			throw Exception("Invalid month specified");

		int days_per_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		int days_in_month = days_per_month[month - 1];

		if (month == 2)
		{
			bool leap_year = (year % 4) == 0; // Leap years are every 4th year
			if ((year % 100) == 0 && (year % 400) != 0) // Except for those divisible by 100 unless they are also divisible by 400
				leap_year = false;
			if (leap_year)
				days_in_month = 29;
		}

		return days_in_month;
	}

	void DateTime::set_null()
	{
		_year = 0;
		_month = 0;
		_day = 0;
		_hour = 0;
		_minute = 0;
		_seconds = 0;
		_nanoseconds = 0;
		_timezone = utc_timezone;
	}

	void DateTime::set_date(int new_year, int new_month, int new_day, int new_hour, int new_minute, int new_seconds, int new_nanoseconds, TimeZone new_timezone)
	{
		throw_if_invalid_date(new_year, new_month, new_day, new_hour, new_minute, new_seconds, new_nanoseconds);
		_year = new_year;
		_month = new_month;
		_day = new_day;
		_hour = new_hour;
		_minute = new_minute;
		_seconds = new_seconds;
		_nanoseconds = new_nanoseconds;
		_timezone = new_timezone;
	}

	void DateTime::set_year(int new_year)
	{
		if (is_null())
			set_date(new_year, 1, 1, 0, 0, 0, 0);
		else
			set_date(new_year, _month, _day, _hour, _minute, _seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_month(int new_month)
	{
		if (is_null())
			set_date(1900, new_month, 1, 0, 0, 0, 0);
		else
			set_date(_year, new_month, _day, _hour, _minute, _seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_day(int new_day)
	{
		if (is_null())
			set_date(1900, 1, new_day, 0, 0, 0, 0);
		else
			set_date(_year, _month, new_day, _hour, _minute, _seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_hour(int new_hour)
	{
		if (is_null())
			set_date(1900, 1, 1, new_hour, 0, 0, 0);
		else
			set_date(_year, _month, _day, new_hour, _minute, _seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_minutes(int new_minutes)
	{
		if (is_null())
			set_date(1900, 1, 1, 0, new_minutes, 0, 0);
		else
			set_date(_year, _month, _day, _hour, new_minutes, _seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_seconds(int new_seconds)
	{
		if (is_null())
			set_date(1900, 1, 1, 0, 0, new_seconds, 0);
		else
			set_date(_year, _month, _day, _hour, _minute, new_seconds, _nanoseconds, _timezone);
	}

	void DateTime::set_nanoseconds(int new_nanoseconds)
	{
		if (is_null())
			set_date(1900, 1, 1, 0, 0, 0, new_nanoseconds);
		else
			set_date(_year, _month, _day, _hour, _minute, _seconds, new_nanoseconds, _timezone);
	}

	void DateTime::set_timezone(TimeZone new_timezone)
	{
		if (is_null())
			set_date(1900, 1, 1, 0, 0, 0, 0, new_timezone);
		else
			set_date(_year, _month, _day, _hour, _minute, _seconds, _nanoseconds, new_timezone);
	}

	DateTime &DateTime::add_years(int years)
	{
		throw_if_null();
		_year += years;
		return *this;
	}

	DateTime &DateTime::add_days(int days)
	{
		throw_if_null();

		if (days > 0)
		{
			while (days > 0)
			{
				int days_in_month = DateTime::days_in_month(_month, _year);
				int days_left_in_current_month = days_in_month - _day;

				if (days <= days_left_in_current_month)
				{
					_day += days;
					break;
				}

				days -= (days_left_in_current_month + 1);
				_day = 1;
				_month++;
				if (_month > 12)
				{
					_year++;
					_month = 1;
				}
			}
		}
		else
		{
			days *= -1;

			while (days > 0)
			{
				int prev_month = _month - 1;
				int prev_month_year = _year;
				if (prev_month < 1)
				{
					prev_month = 12;
					prev_month_year--;
				}

				int days_left_in_current_month = _day;
				if (days < days_left_in_current_month)
				{
					_day -= days;
					break;
				}

				int days_in_prev_month = DateTime::days_in_month(prev_month, prev_month_year);
				days -= days_left_in_current_month;
				_day = days_in_prev_month;
				_month--;
				if (_month < 1)
				{
					_year--;
					_month = 12;
				}
			}

		}

		return *this;
	}

	DateTime &DateTime::add_months(int months)
	{
		throw_if_null();
		int years = months / 12;
		_year += years;
		int months_left = months - (years * 12);
		_month += months_left;
		if (_month > 12)
		{
			_month -= 12;
			_year++;
		}

		return *this;
	}

	/*
	void DateTime::add_hours(int hours)
	{
	throw_if_null();
	}

	void DateTime::add_minutes(int minutes)
	{
	throw_if_null();
	}

	void DateTime::add_seconds(int seconds)
	{
	throw_if_null();
	}

	void DateTime::add_nanoseconds(int nanoseconds)
	{
	throw_if_null();
	}
	*/
	std::string DateTime::to_long_date_string() const
	{
		throw_if_null();
		throw Exception("DateTime::to_long_date_string() not implemented");
	}

	std::string DateTime::to_short_date_string() const
	{
		throw_if_null();
		StringFormat format("%1-%2-%3");
		format.set_arg(1, year(), 4);
		format.set_arg(2, month(), 2);
		format.set_arg(3, day(), 2);
		return format.result();
	}

	DateTime DateTime::from_short_date_string(const std::string &value)
	{
		DateTime datetime;

		// Initialize all values (-1 meaning we haven't found that part of the datetime yet)
		int year = -1;
		int month = -1;
		int day = -1;
		int hour = -1;
		int minute = -1;
		int second = -1;
		int milliseconds = -1;

		std::string::size_type separator_pos = value.find_first_of(' ');

		// Check if we got a date part
		if (value.find_first_of('-') != std::string::npos)
		{
			std::string datePart;
			if (separator_pos == std::string::npos)
				datePart = value;	// We don't have two-part datetime, only a date
			else
				datePart = value.substr(0, separator_pos); // We have two-part datetime, find date part

			std::string::size_type prevPos = 0;
			while (true)
			{
				std::string::size_type pos = datePart.find_first_of("-/\\", prevPos);
				if (pos == std::string::npos)
					pos = datePart.length() - 1;

				int int_value = Text::parse_int32(datePart.substr(prevPos, pos));

				if (year == -1)
					year = int_value;
				else if (month == -1)
					month = int_value;
				else if (day == -1)
					day = int_value;

				if (pos == datePart.length() - 1)
					break;

				prevPos = pos + 1;
			}
		}

		// Check if we got a time part
		if (value.find_first_of(':') != std::string::npos)
		{
			std::string timePart;

			if (separator_pos == std::string::npos)
				timePart = value;	// We don't have two-part datetime, only a time
			else
				timePart = value.substr(separator_pos + 1); // We have two-part datetime, find time part

			std::string::size_type prevPos = 0;
			while (true)
			{
				std::string::size_type pos = timePart.find_first_of(":.", prevPos);
				if (pos == std::string::npos)
					pos = timePart.length() - 1;

				int int_value = Text::parse_int32(timePart.substr(prevPos, pos));

				if (hour == -1)
					hour = int_value;
				else if (minute == -1)
					minute = int_value;
				else if (second == -1)
					second = int_value;
				else if (milliseconds == -1)
					milliseconds = int_value;

				if (pos == timePart.length() - 1)
					break;

				prevPos = pos + 1;
			}
		}

		if (year != -1)
			datetime.set_year(year);
		if (month != -1)
			datetime.set_month(month);
		if (day != -1)
			datetime.set_day(day);
		if (hour != -1)
			datetime.set_hour(hour);
		if (minute != -1)
			datetime.set_minutes(minute);
		if (second != -1)
			datetime.set_seconds(second);
		//	if(milliseconds == -1)
		//		datetime.set_nanoseconds(milliseconds * 1000000);

		return datetime;
	}

	std::string DateTime::to_short_datetime_string() const
	{
		throw_if_null();
		// 2008-04-01
		StringFormat format("%1-%2-%3 %4:%5:%6");
		format.set_arg(1, year(), 4);
		format.set_arg(2, month(), 2);
		format.set_arg(3, day(), 2);
		format.set_arg(4, hour(), 2);
		format.set_arg(5, minutes(), 2);
		format.set_arg(6, seconds(), 2);
		return format.result();
	}

	std::string DateTime::to_long_time_string() const
	{
		throw_if_null();
		// hh:mm:ss
		StringFormat format("%1:%2:%3");
		format.set_arg(1, hour(), 2);
		format.set_arg(2, minutes(), 2);
		format.set_arg(3, seconds(), 2);
		return format.result();
	}

	std::string DateTime::to_short_time_string() const
	{
		throw_if_null();
		// hh:mm
		StringFormat format("%1:%2");
		format.set_arg(1, hour(), 2);
		format.set_arg(2, minutes(), 2);
		return format.result();
	}

	std::string DateTime::to_string() const
	{
		throw_if_null();
		// Mon Feb 3 12:32:54 2008
		std::string months[] =
		{
			"Jan",
			"Feb",
			"Mar",
			"Apr",
			"May",
			"Jun",
			"Jul",
			"Aug",
			"Sep",
			"Oct",
			"Nov",
			"Dec"
		};

		std::string days[] =
		{
			"Sun",
			"Mon",
			"Tue",
			"Wed",
			"Thu",
			"Fri",
			"Sat"
		};

		StringFormat format("%1 %2 %3 %4:%5:%6 %7");
		format.set_arg(1, days[day_of_week()]);
		format.set_arg(2, months[month() - 1]);
		format.set_arg(3, day());
		format.set_arg(4, hour(), 2);
		format.set_arg(5, minutes(), 2);
		format.set_arg(6, seconds(), 2);
		format.set_arg(7, year());
		return format.result();
	}

	bool DateTime::operator <(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a < b;
	}

	bool DateTime::operator <=(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a <= b;
	}

	bool DateTime::operator >(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a > b;
	}

	bool DateTime::operator >=(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a >= b;
	}

	bool DateTime::operator ==(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a == b;
	}

	bool DateTime::operator !=(const DateTime &other) const
	{
		int64_t a = is_null() ? 0 : to_ticks();
		int64_t b = other.is_null() ? 0 : other.to_ticks();
		return a != b;
	}

	void DateTime::throw_if_invalid_date(int year, int month, int day, int hour, int minute, int seconds, int nanoseconds) const
	{
		if (month == 2)
		{
			bool leap_year = (year % 4) == 0; // Leap years are every 4th year
			if ((year % 100) == 0 && (year % 400) != 0) // Except for those divisible by 100 unless they are also divisible by 400
				leap_year = false;
			if ((leap_year && day > 29) || (!leap_year && day > 28))
				throw Exception("Invalid date specified");
		}

		if (year < 1900 || year > 2900 || month < 1 || month > 12 || day < 1 || day > 31 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || seconds < 0 || seconds > 60 || nanoseconds >= 1000000000)
			throw Exception("Invalid date specified");
	}

	void DateTime::throw_if_null() const
	{
		if (is_null())
			throw Exception("DateTime object is null");
	}

	int DateTime::day_number() const
	{
		// see http://alcor.concordia.ca/~gpkatch/gdate-algorithm.html
		int m = _month;
		int y = _year;
		m = (m + 9) % 12;
		y = y - m / 10;
		return 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (_day - 1);
	}
}
