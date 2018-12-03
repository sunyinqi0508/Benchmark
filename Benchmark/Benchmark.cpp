
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <roaring/roaring.h>
#include <roaring/cpp/roaring.hh>
#include "Parameters.h"
#include "Benchmark.h"
#include "LIndex.h"
#include "zipfian_distribution.hpp"
#include "FileIO.h"
#include "EWAH/ewah.h"
#include "EWAH/boolarray.h"
#include "FastPFor/fastpfor.h"
#include "FastPFor/bitpacking.h"

using namespace std;
using namespace LIndexUnwarpped;

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
  printf("%x %d\n", rawdata, array_test[u_rand(engine)]);

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
  //test_vector();
  //return 0;
	int ii = 0;

  //return 0;
  GC *gc = new GC();
  gc->start();
  random_device device{};
  mt19937 engine{ device() };
  ZipfianDistribution<unsigned int> zipf_distribution(Parameters::n_groups, engine, 1.f);
  unsigned int *rawdata = zipf_distribution.get_rawdata(Parameters::n_data);
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

  return 0;

}