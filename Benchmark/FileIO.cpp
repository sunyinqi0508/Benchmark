#include "FileIO.h"
#include <fstream>
#include <stdint.h>
using namespace std;
bool FileIO::SaveCSVFile(
	const char* filename,
	int n_attributes, const char** attributes,
	int n_rows, char** row_names, SupportedData*** data
) {
	FILE *fp = 0;
	fp = fopen(filename, "w");

	if (!fp) return 1;

	// fprintf(fp, ",");
	for (int i = 0; i < n_attributes; ++i)
		output_string(fp, attributes[i]);
	fprintf(fp, "\n");

	for (int i = 0; i < n_rows; ++i) {
		//output_attribute(fp, row_names[i]);
		for (int j = 0; j < n_attributes; ++j)
			switch (data[i][j]->id) {
			case SupportedData::TypesInt:
				fprintf(fp, "%d,", *(uint32_t*)data[i][j]->val);
				break;
			case SupportedData::TypesFloat:
				fprintf(fp, "%f,", *(float*)data[i][j]->val);
				break;
			case SupportedData::TypesString:
				output_string(fp, (const char*)data[i][j]->val);
				break;
			default:
				fclose(fp);
				return 1;
			}

		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}

FileIO & FileIO::operator<<(SupportedData data)
{
	// TODO: insert return statement here
	if (fp)
		switch (data.id) {
		case SupportedData::TypesInt:
			fprintf(fp, "%d,", *(uint32_t*)data.val);
			break;
		case SupportedData::TypesFloat:
			fprintf(fp, "%f,", *(float*)data.val);
			break;
		case SupportedData::TypesString:
			output_string(fp, (const char*)data.val);
			break;
		case SupportedData::TypesBreak:
			fprintf(fp, ",");
			break;
		case SupportedData::TypeNewLine:
			fprintf(fp, "\n");
			break;
		default:
			;//fclose(fp);
		}
	else
		printf("Error: no files opened.\n");
	return *this;
}

FileIO & FileIO::operator+(SupportedData d)
{
	return *this;
}

FILE* FileIO::getFileHandle()
{
	return fp;
}

void FileIO::output_string(FILE *fp, const char * attrib)
{
	bool escape = false;
	int i = 0;
	while (!escape && attrib[i])
		escape = attrib[i++] == ',';
	if (escape)
		fprintf(fp, "\"%s\",", attrib);
	else
		fprintf(fp, "%s,", attrib);
}

SupportedData::SupportedData(SupportedData::Types id, const void * val)
{
	this->id = id;
	this->val = val;
}