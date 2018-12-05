#ifndef _BENCHMARK_H
#define _BENCHMARK_H
#include "Parameters.h"
#include "gc.h"
#include <chrono>
template<class _Compressor_t>
class Benchmark {

private:
	
	int (*get_size)(_Compressor_t&);
	void *(*init)(GC * gc);
	void (*emplace_back)(_Compressor_t&, unsigned int& i);
	const char* method_name;
	unsigned int* rawdata;
	GC *gc;
	
	Benchmark(const char* method_name, unsigned int* rawdata,
		void(*emplace_back)(_Compressor_t&, unsigned int& i),
		int(*get_size)(_Compressor_t&),
		void *(*init)(GC* gc), GC* gc
	);
	void operator()();
public:	
	static void bench(const char* method_name, unsigned int* rawdata,
		 void(*emplace_back)(_Compressor_t&, unsigned int& i),
		 int (*get_size)(_Compressor_t&), void *(*init)(GC* gc) = 0,GC* gc = 0
	);
};

#endif