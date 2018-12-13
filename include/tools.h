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

namespace Tools {

	extern const char* getCurrentTimeString();
	extern const char* getTimeHash();
}
#endif