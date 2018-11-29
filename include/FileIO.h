#ifndef _FILEIO_H
#define _FILEIO_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATES
#include <cstdio>
class SupportedData {
  //friend FileIO;
public:
  enum Types { TypesInt, TypesFloat, TypesString };
  Types id:3;
  const void *val;
  SupportedData() = default;
  SupportedData(Types id, const void *val);
};
class FileIO {
public:

  static bool SaveCSVFile(
	const char* filename,
	int n_attributes, const char** attributes,
	int n_rows,  char** row_names, SupportedData*** data);
  
private:
  static void output_string(std::FILE *fp, const char* attrib);
};
#endif