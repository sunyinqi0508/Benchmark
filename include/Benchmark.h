#ifndef _BENCHMARK_H
#define _BENCHMARK_H
#include "Parameters.h"
#include "gc.h"
#include <chrono>
#include "roaring/roaring.h"
//typedef  int(*_Intellisense_T1)(Roaring &);
//typedef  void *(*_Intellisense_T2)(GC * gc);
//typedef  void(*_Intellisense_T3)(Roaring&, unsigned int& i);

template<class _Compressor_t>
auto _Default_initizer = [](GC*) -> _Compressor_t* {return new _Compressor_t(); };
template<class _Compressor_t>
auto _Default_finalizer = [](_Compressor_t&) {};
template<class _Compressor_t, class _Init_t, class _Emplace_t, class _GetSize_t, class _Finalization_t>
class Benchmark {

private:
	
	//int (*get_size)(_Compressor_t&);
	//void *(*init)(GC * gc);
	//void (*emplace_back)(_Compressor_t&, unsigned int& i);

public:	
	constexpr Benchmark(const _Compressor_t*, const char* method_name, unsigned int* rawdata, unsigned int n_data,
		_Emplace_t emplace_back, _GetSize_t get_size, _Init_t init, _Finalization_t fin, 
		GC* gc = 0
	) noexcept;
	constexpr Benchmark(const _Compressor_t *, const char * method_name, unsigned int * rawdata, unsigned int n_data,
		_Emplace_t emplace_back, _GetSize_t get_size, _Init_t init, _Finalization_t fin,
		unsigned int batch_size, GC * gc) noexcept;


};


#endif