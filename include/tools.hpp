#ifndef _TOOL_H
#define _TOOL_H
//#include <numeric>
//#include <string>
//namespace std {
//
//
//  std::string to_string(unsigned int number) {
//	const int size_table[] = { 9, 99, 999, 9999, 99999, 999999, 9999999,
//			99999999, 999999999, std::numeric_limits<unsigned int>::max() };
//	int i = 0;
//	while(true)
//	  if (number <= size_table[i++])
//		break;
//	std::string str(i, 0);
//	while (i > 0)
//	{
//	  str[i--] = number % 10;
//	  number /= 10;
//	}
//	return str;
//  }
//
//}
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
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
	sprintf(str_time, "%x", now);
	return str_time;
  }
}
#endif