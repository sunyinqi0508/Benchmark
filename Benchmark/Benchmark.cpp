
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <roaring/roaring.h>
#include <roaring/cpp/roaring.hh>
#include <filesystem>
#include "Parameters.h"
#include "Benchmark.h"
#include "LIndex.h"
#include "DataGen.hpp"
#include "FileIO.h"
#include "EWAH/ewah.h"
#include "EWAH/boolarray.h"
#include "FastPFor/fastpfor.h"
#include "FastPFor/bitpacking.h"
#include "bm.h"
#define _TYPE_OF(_Ty) ((_Ty *)NULL)
#define _TIME_(ret, x) {auto _in_scope_timing_ = chrono::steady_clock::now();\
(x)\
(ret) = chrono::duration_cast<double, milli>\
	(chrono::steady_clock::now() - _in_scope_timing_).count();}

using namespace std;
using namespace LIndexUnwarpped;
FileIO fio;
Endl fendl;
Break fbreak;



template<class _Compressor_t, class _Init_t, class _Emplace_t, class _GetSize_t>
constexpr Benchmark<_Compressor_t, _Init_t, _Emplace_t, _GetSize_t>::Benchmark(const _Compressor_t*,
	const char * method_name, unsigned int * rawdata, 
	_Emplace_t emplace_back, _GetSize_t get_size, _Init_t init, 
	GC * gc) noexcept
{
	fio << method_name;

	unordered_map<int, _Compressor_t *>lineage;
	chrono::time_point<chrono::steady_clock > time = chrono::steady_clock::now();
	for (unsigned int i = 0; i < Parameters::n_data; ++i)
	{
		auto l = lineage.find(rawdata[i]);
		if (l == lineage.end())
		{
			auto back = lineage.insert(std::make_pair(
				rawdata[i], init(gc)));
			emplace_back(*(back.first)->second, i);
		}
		else
			emplace_back((*l->second), i);
	}

	unsigned int space_consumption = 0;
	const float _time = chrono::duration<double, milli>(chrono::steady_clock::now() - time).count();
	fio << _time;
	printf("time %s: %lf ms\nGroup space usage: ",
		method_name, _time);
	int ii = 0;
	for (auto& lidx : lineage)
	{
		const unsigned int size = get_size(*lidx.second);
		space_consumption += size;
		printf("Group %d, %d; ", ii++, size);
		fio << size;
		delete lidx.second;
	}
	printf("\ntotal space: %u Bytes\n", space_consumption);
	fio << space_consumption << fendl;

	lineage.clear();
}

template<class _Compressor_t, class _Init_t, class _Emplace_t, class _GetSize_t>
inline void Benchmark<_Compressor_t, _Init_t, _Emplace_t, _GetSize_t>::operator()()
{

}




int test_vector()
{

  GC *gc = new GC();
  gc->start();
  random_device device{};
  mt19937 engine{device()};
  uniform_int_distribution u_rand(0, (int)(Parameters::n_data - 1));
  ZipfianDistribution<unsigned int> zipf_distribution(Parameters::n_groups, engine, 1.f);
  unsigned int *rawdata  = zipf_distribution.get_rawdata(Parameters::n_data);
  LIndex lid_test(gc), *hybrid_test;
  vector<RID> vector_test;
  _Size32_t lineage_capacity = 2;
  _Size32_t lineage_size = 0;
  RID* lineage_ids = 0;
  lid_init(lineage_ids, lineage_capacity);
  RID* array_test = new RID[Parameters::n_data];
 



  auto time = chrono::steady_clock::now();  
  memcpy(array_test, rawdata, 4 * Parameters::n_data);
  printf("time array: %lf ms\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  array_test[u_rand(engine)] = u_rand(engine);
  printf("%lx %d\n", rawdata, array_test[u_rand(engine)]);

  time = chrono::steady_clock::now();
  
  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	lid_emplace_back(lineage_ids, lineage_capacity, lineage_size, rawdata[i], gc);
  printf("time lid: %lf ms , space lid: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * lineage_capacity);
  delete[] lineage_ids;

  hybrid_test = new LIndex(gc);
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	lid_emplace_back(((RID**)hybrid_test)[1], ((_Size32_t*)hybrid_test)[0], ((_Size32_t*)hybrid_test)[1], rawdata[i], gc);

  printf("time lid_hybrid: %lf ms , space lid_hybrid: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * hybrid_test->capacity());
  delete hybrid_test;

  hybrid_test = new LIndex();
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	  lid_emplace_back(((RID**)hybrid_test)[1], ((_Size32_t*)hybrid_test)[0], ((_Size32_t*)hybrid_test)[1], rawdata[i]);

  printf("time lid_hybrid_no_gc: %lf ms , space lid_hybrid: %u\n",
	  chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	  sizeof(RID) * hybrid_test->capacity());
  delete hybrid_test;

  hybrid_test = new LIndex();
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	//lid_emplace_back(hybrid_test, rawdata[i]);
	lid_emplace_back(hybrid_test, rawdata[i], gc);

  printf("time lid_hybrid2: %lf ms , space lid_hybrid2: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * hybrid_test->capacity());
  delete hybrid_test;

  time = chrono::steady_clock::now();

  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	lid_test.emplace_back(rawdata[i]);
  printf("time lid_warpped: %lf ms , space lid_warpped: %u allocs %d\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * lid_test.capacity(), lid_test._alloc_count);
  lid_test.~LIndex();
  
  
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	vector_test.emplace_back(rawdata[i]);

  printf("time stdvector: %lf ms , space stdvector: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * (size_t)vector_test.capacity());
  vector_test.~vector();

  printf("%x\n", rawdata);
  return 0;	
}


int main() {
	PQDump();
	return 0;

  int ii = 0;
  //return 0;

  GC *gc = new GC();
  gc->start();
  random_device device{};
  mt19937 engine{ device() };
  ZipfianDistribution<unsigned int> zipf_distribution(Parameters::n_groups, engine, 1.f);
  uniform_int_distribution<unsigned int> unifrom_distribution{ 0, Parameters::n_groups };

  unsigned int *rawdata = zipf_distribution.get_rawdata(Parameters::n_data);

  char csv_name[1024];
  sprintf(csv_name, "./results/%s.csv", Tools::getTimeHash());
  fio.open(csv_name);
  FILE *fp = fio.getFileHandle();
#pragma region metadata
  //data_gen
  
  fio << "DATA ID:" << 1u << fendl;
  fio << "Data stats"<<fbreak;

  for (int i = 0; i < Parameters::n_groups; ++i)
  {
	  char buf[33];
	  sprintf(buf, "Group %d Size", i);
	  fio << buf;
  };
  fio<<"Total size (Bytes)" << fendl<<fbreak<<fbreak;
  int lastelement = (Parameters::n_data) * (Parameters::n_data - 1) / 2;
  unordered_set<int> set1;
  for (int i = 0; i < Parameters::n_data; ++i)
  {
	  if (set1.find(rawdata[i]) == set1.end())
	  {
		  lastelement -= rawdata[i];
		  set1.insert(rawdata[i]);
		  fio << (unsigned int)std::count(rawdata, rawdata + Parameters::n_data, rawdata[i]) * 4;
		  if (set1.size() == Parameters::n_data - 1)
		  {
			  fio << (unsigned int)std::count(rawdata, rawdata + Parameters::n_data, lastelement) * 4;
			  break;
		  }
	  }
  }
  fio << Parameters::n_data * 4 << fendl;
#pragma endregion

  fio << "RUNID:" << 1u<<fendl;
  fio << "Method Name" << "Time Consumption (ms)";
  for (int i = 0; i < Parameters::n_groups; ++i)
  {
	  char buf[33];
	  sprintf(buf, "Group %d", i);
	  fio << buf;
  };
  fio << "Total Memory Consumption (Bytes)" << fendl;


  Benchmark b1(_TYPE_OF(Roaring), "Roaring", rawdata,
	  [](Roaring& r, unsigned int& i) {r.add(i); },
	  [](Roaring&r) -> int {return r.getSizeInBytes(); },
	  _Default_initizer<Roaring>
  );
  Benchmark b2(_TYPE_OF(EWAHBoolArray<RID>), "EWAH", rawdata,
	  [](EWAHBoolArray<RID>& r, unsigned int& i) {r.addWord(i); },
	  [](EWAHBoolArray<RID>&r) -> int {return r.sizeInBytes(); },
	  _Default_initizer<EWAHBoolArray<RID>>
  ); 
  Benchmark b3(_TYPE_OF(LIndex), "My Vector", rawdata,
	  [](LIndex& r, unsigned int& i) {r.emplace_back(i); },
	  [](LIndex&r) -> int {return r.capacity() * sizeof(RID); },
	  _Default_initizer<LIndex>
  );
  Benchmark b4(_TYPE_OF(LIndex), "My Vector + GC", rawdata,
	  [](LIndex& r, unsigned int& i) {r.emplace_back(i); },
	  [](LIndex&r) -> int {return r.capacity() * sizeof(RID); },
	  [](GC* gc) -> LIndex* {return (new LIndex(gc)); }, gc
  );
  Benchmark b5(_TYPE_OF(bm::bvector<>), "BitMagic", rawdata,
	  [](bm::bvector<>& r, unsigned int& i) {r.set(i); },
	  [](bm::bvector<>&r) -> int {
		  bm::bvector<>::statistics stats;
		  r.calc_stat(&stats);
		  return stats.memory_used;
	  }, 
	  [](GC*) -> bm::bvector<>* {
		  bm::bvector<> *bv = new bm::bvector<>;
		  bv->set_new_blocks_strat(bm::BM_GAP);
		  return (bv);
	  }
  ); 
  //FastPForLib::FastPFor fpf;
  
#pragma region lagacy
  //Lagacy benchmark codes. For best performance.

  unordered_map<int, LIndex*> lineage;
  auto time = chrono::steady_clock::now();

  for (int i = 0; i < Parameters::n_data; ++i)
  {
	auto l = lineage.find(rawdata[i]);
	if (l == lineage.end())
	  lineage.insert(std::make_pair(rawdata[i], new LIndex(i)));
	else {
	  lid_emplace_back((l->second), i);
	 // l->second->emplace_back(rawdata[i]);
	}
  }
  size_t space_consumption = 0;  
  	

  printf("time myvector + gc: %lf ms\n Group space usage:",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  ii = 0;
  for (auto& lidx : lineage)
  {
	space_consumption += lidx.second->capacity();
	printf("Group %d, size in Bytes %d \n", ii++, lidx.second->capacity()* sizeof(RID));

	delete lidx.second;
  }
  printf("total space myvector : %u Bytes\n", space_consumption * sizeof(RID));

  lineage.clear();
 
  unordered_map<int, Roaring*> lineage_r;
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
  {
	auto l = lineage_r.find(rawdata[i]);
	if (l == lineage_r.end())
	  lineage_r.insert(std::make_pair(rawdata[i], new Roaring()));
	else {
	  l->second->add(i);
	}
  }
  printf("time roaring: %lf ms\n Group space usage:",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  space_consumption = 0;
  ii = 0;
  for (auto& lidx : lineage_r)
  {
	space_consumption += lidx.second->getSizeInBytes();
	printf("Group %d, size in Bytes %d \n", ii++, lidx.second->getSizeInBytes());

	delete lidx.second;
	lidx.second = 0;
  }
  printf("total space roaring: %u Bytes\n", space_consumption);
  lineage_r.clear();

  unordered_map<int, EWAHBoolArray<RID>*> lineage_ewah;
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
  {
	  auto l = lineage_ewah.find(rawdata[i]);
	  if (l == lineage_ewah.end())
		  lineage_ewah.insert(std::make_pair(rawdata[i], new EWAHBoolArray<RID>()));
	  else {
		  l->second->addWord(i);
	  }
  }
  printf("time ewah: %lf ms\n Group space usage:",
	  chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  space_consumption = 0;
  ii = 0;
  
  for (auto& lidx : lineage_ewah)
  {
	  space_consumption += lidx.second->sizeInBytes();
	  printf("Group %d, size in Bytes %d \n", ii++, lidx.second->sizeInBytes());
	  delete lidx.second;
	  lidx.second = 0;
  }
  printf("total space ewah: %u Bytes\n", space_consumption );
  lineage_ewah.clear();
#pragma endregion

  fio.close();
  error_code error;
  filesystem::copy_file(filesystem::path(csv_name), filesystem::path("./latest_result.csv"),
	  filesystem::copy_options::overwrite_existing, error);
  if (error)
	  printf("copy result failed, go to results directory to find the latest results.\n");
  printf("results saved to: %s", csv_name);
  
  return 0;
}