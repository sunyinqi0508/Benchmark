#define _CRT_SECURE_NO_WARNINGS
#include "tools.h"
#include <chrono>
#include <cstdio>
#include <ctime>
namespace Tools {
	const char* getCurrentTimeString() {
		time_t now = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
		tm* time = localtime(&now);
		char *str_time = new char[24];
		int pos = 0;
		auto cvt_routine = [&str_time, &pos](const int duration, int val, const char sep = 0) {
			for (int i = pos + duration - 1; i >= pos; --i)
			{
				str_time[i] = '0' + val % 10;
				val /= 10;
			}
			pos += duration;
			str_time[pos++] = sep;
		};
		cvt_routine(4, time->tm_year + 1900, '-');
		cvt_routine(2, time->tm_mon + 1, '-');
		cvt_routine(2, time->tm_mday, ' ');
		cvt_routine(2, time->tm_hour, '-');
		cvt_routine(2, time->tm_min, '-');
		cvt_routine(2, time->tm_sec);
		return str_time;
	}
	const char* getTimeHash() {
		time_t now = std::chrono::system_clock::to_time_t(
			std::chrono::system_clock::now());
		char* str_time = new char[16];
		std::sprintf(str_time, "%lx", now);
		return str_time;
	}

	template <class T>
	inline void do_not_optimize_away(T&& datum) {
		// __asm ("" : "+r" (datum));
	}
}