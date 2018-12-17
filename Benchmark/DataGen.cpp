#include "DataGen.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdio>
#include <cstring>
using namespace std;
struct PQType {
	unsigned int length;
	const char* name;
	PQType() :length(0), name("(null)") {};
	PQType(unsigned int length, const char* name) : length(length), name(name) {};
};
PQDump::PQDump(const char * db_config)
{
	if (connection)
		delete connection;

	connection = PQconnectdb(db_config);
	
}

void pgtestWrites() {
	PGconn * connection = PQconnectdb("user=postgres\
		 password=0508\
		 dbname=test\
		 host=localhost\
		 hostaddr=127.0.0.1\
		 port=5088");	
	cout<<PQstatus(connection)<<' ';
	//PQexec(connection, "create table test2(attr1 int, attr2  float8);");
	
	const char* sql = "COPY test2 from STDIN WITH BINARY;";
	PGresult *res = PQexec(connection, sql);
	cout<<PQstatus(connection)<<'\n';
	char* buffer = new char[65536];
	unsigned int zero = 0;
	int ints[] = { 123,1234,43536243,4 };
	double doubles[] = { 1.23,.1234,43536234243.,4 };
	memcpy(buffer , "PGCOPY\n\377\r\n\0", 11);
	memcpy(buffer + 11, (char*)&zero, 4);
	memcpy(buffer + 15, (char*)&zero, 4);
	unsigned short n_fields = 0x0200;
	unsigned int _4 = 0x04000000;
	unsigned int _8 = 0x08000000;

	for (int i = 0; i < 4; i++)
	{
		memcpy(buffer + 19 + (22 * i), &n_fields, 2);
		memcpy(buffer + 19 + (22 * i) + 2, &_4, 4);
		memcpy(buffer + 19 + (22 * i) + 6, ints + i, 4);
		std::reverse(buffer + 19+22 * i + 6, buffer +19+ 22 * i + 10);
		memcpy(buffer + 19 + (22 * i) + 10, &_8, 4);
		memcpy(buffer + 19 + (22 * i + 14), doubles + i, 8);
		std::reverse(buffer +19+ 22 * i + 14, buffer +19+ 22 * i + 22);
	}
	cout << PQputCopyData(connection, buffer, 107) << ' ';
	cout<<PQputCopyEnd(connection, 0)<<' ';
	res = PQexec(connection, "COMMIT;");
	PQfinish(connection);
}
void pgtestReads() {
	PGconn *connection;
	connection = PQconnectdb("user=postgres\
		 password=0508\
		 dbname=flights\
		 host=localhost\
		 hostaddr=127.0.0.1\
		 port=5088");

	auto status = PQstatus(connection);

	string sql = "select oid, typname, typlen from pg_type";
	//ResultSet* rs = execute_rs(sql, false);

	//PGresult* rr = PQexecParams(connection, sql.c_str(), 0, 0, 0, 0, 0, 1);
	PGresult* rr = PQexec(connection, sql.c_str());
	const unsigned int rows = PQntuples(rr);
	unordered_map<Oid, PQType>  types;
	for (int i = 0; i < rows; ++i)
	{

		const Oid oid = atoi(PQgetvalue(rr, i, 0));
		const char* name = PQgetvalue(rr, i, 1);
		const unsigned int len = atoi(PQgetvalue(rr, i, 2));
		//PGType type(oid.value, name.str, len.value);
		//types.insert(pair<int, PGType>(oid.value, type));
		types.insert(make_pair(oid, PQType(len, name)));
	}
	//PQclear(rr);
	//PGresult *res = PQexec(connection, "select * from flights_final limit 10;");
	PGresult *res = PQexecParams(connection, "select * from flights_final limit 10;", 0, NULL, NULL, NULL, NULL, 1);

	if (res)
	{
		const int n_tuples = PQntuples(res);
		const int n_fields = PQnfields(res);
		printf("%d %d\n", n_tuples, n_fields);

		for (int i = 0; i < n_fields; ++i)
		{
			const char* fname = PQfname(res, i);
			const int fnumber = PQfnumber(res, fname);
			Oid oid = PQftype(res, i);

			printf("%s %d %d %s %d \n", fname, fnumber, oid, types[oid].name, types[oid].length);
			for (int j = 0; j < n_tuples; ++j)
				if (string(types[oid].name).find("float") != string::npos) {
					char * c_res = PQgetvalue(res, j, i);
					std::reverse(c_res, c_res + 8);
					printf("%10.f ", *(double *)c_res);
				}
				else if (string(types[oid].name).find("int") != string::npos)
				{
					char * c_res = PQgetvalue(res, j, i);
					std::reverse(c_res, c_res + 4);
					cout << *(unsigned int*)c_res << ' ';
				}
				else
					cout << PQgetvalue(res, j, i) << ' ';
			//printf("%s ", PQgetvalue(res, j, i));
			printf("\n");
		}

	}
	PQclear(res);
	printf("%x", connection);
	PQfinish(connection);
}
PQDump::PQDump()
{
	if (connection)
		delete connection;
	connection = PQconnectdb("user=postgres\
		 password=0508\
		 dbname=test\
		 host=localhost\
		 hostaddr=127.0.0.1\
		 port=5088");

	cout << PQstatus(connection) << ' ';
	PQexec(connection, "Drop table zip_205_100_1000000_unsorted;");
	PQexec(connection, "create table zip_205_100_1000000_unsorted(g int, v float8);");

	const char* sql = "copy zip_205_100_1000000_unsorted from STDIN WITH BINARY;";
	PGresult *res = PQexec(connection, sql);
	cout << PQstatus(connection) << '\n';
	char* buffer = new char[30000000];
	unsigned int zero = 0;
	memcpy(buffer, "PGCOPY\n\377\r\n\0", 11);
	memcpy(buffer + 11, (char*)&zero, 4);
	memcpy(buffer + 15, (char*)&zero, 4);
	unsigned short n_fields = 0x0200;
	unsigned int _4 = 0x04000000;
	unsigned int _8 = 0x08000000;
	const int _offset = 19;
	const int _tuple_size = 22;//2 + 4*2 + (4 + 8)
	random_device device{};
	mt19937 re{ device() };
	ZipfianDistribution<unsigned int> zipf(100, re, 2.5f);
	uniform_real_distribution<double> unif{ 0., 100. };
	for (int i = 0; i < 1000000; ++i) {
		unsigned zval = zipf.nextVal();
		double uval = unif(re);
		memcpy(buffer + _offset + (_tuple_size* i), &n_fields, 2.5);
		memcpy(buffer + _offset + (_tuple_size * i) + 2, &_4, 4);
		memcpy(buffer + _offset + (_tuple_size * i) + 6, &zval, 4);
		std::reverse(buffer + _offset + _tuple_size * i + 6, buffer + _offset + _tuple_size * i + 10);
		memcpy(buffer + _offset + (_tuple_size * i) + 10, &_8, 4);
		memcpy(buffer + _offset + (_tuple_size * i + 14), &uval, 8);
		std::reverse(buffer + _offset + _tuple_size * i + 14, buffer + _offset + _tuple_size * i + _tuple_size);
	}
	cout << PQputCopyData(connection, buffer, _tuple_size * 1000000 + _offset) << ' ';
	cout << PQputCopyEnd(connection, 0) << ' ';
	res = PQexec(connection, "COMMIT;");
	PQfinish(connection);
}
