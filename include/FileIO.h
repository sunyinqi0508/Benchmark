#ifndef _FILEIO_H
#define _FILEIO_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATES
#include <cstdio>
#include <stdlib.h>
class SupportedData {
  //friend FileIO;
public:
  enum Types { TypesInt, TypesFloat, TypesString, TypesBreak, TypeNewLine, TypeCount };
  Types id = TypeCount;
  class Endl {}; class Break {};
  const void *val = 0;
  SupportedData() = default;
  SupportedData(const char* d): id(TypesString), val(d) {}
  SupportedData(unsigned int d) : id(TypesInt), val(new unsigned int(d)) {}
  SupportedData(float d): id(TypesFloat), val(new float(d)) {}
  SupportedData(Endl) : id(TypeNewLine), val(0) {}
  SupportedData(Break) : id(TypesBreak), val(0) {}

  SupportedData(Types id, const void *val);

  ~SupportedData() { 
	  if ((id == TypesInt || id == TypesFloat) && val) 
			{ free(const_cast<void *>(val)); val = 0; } 
  }
};
using Endl = SupportedData::Endl;
using Break = SupportedData::Break;
class FileIO {
public:

  static bool SaveCSVFile(
	const char* filename,
	int n_attributes, const char** attributes,
	int n_rows,  char** row_names, SupportedData*** data);
  FileIO() : fp(0) {}
  FileIO(std::FILE* fp) :fp(fp) {}
  void setFileHandle(std::FILE *fp) { this->fp = fp; };
  bool open(const char* filename) { return (fp = fopen(filename, "w")) != 0; }
  void close() { if (fp) fclose(fp); fp = 0; }
  FileIO& operator<<(SupportedData);
  FileIO& operator+(SupportedData);

  std::FILE* getFileHandle();
private:
  std::FILE *fp;
  static void output_string(std::FILE *fp, const char* attrib);
};
#endif