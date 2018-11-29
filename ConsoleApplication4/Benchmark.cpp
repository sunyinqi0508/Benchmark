
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

using namespace std;
using namespace LIndexUnwarpped;

int test_vector()
{
  random_device device{};
  mt19937 engine{device()};
  uniform_int_distribution u_rand(0, (int)(Parameters::n_data - 1));
  ZipfianDistribution<unsigned int> zipf_distribution(Parameters::n_groups, engine, 1.f);
  unsigned int *rawdata  = zipf_distribution.get_rawdata(Parameters::n_data);
  LIndex lid_test, *hybrid_test;
  vector<RID> vector_test;
  _Size32_t lineage_capacity = 2;
  _Size32_t lineage_size = 0;
  RID* lineage_ids = 0;
  lid_init(lineage_ids, lineage_capacity);

  auto time = chrono::steady_clock::now(); 
  RID* array_test = new RID[Parameters::n_data];
  memcpy(array_test, rawdata, 4 * Parameters::n_data);
  printf("time array: %lf ms\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  array_test[u_rand(engine)] = u_rand(engine);
  printf("%x %d\n", rawdata, array_test[u_rand(engine)]);

  time = chrono::steady_clock::now();
  
  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	lid_emplace_back(lineage_ids, lineage_capacity, lineage_size, rawdata[i]);
  printf("time lid: %lf ms , space lid: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * lineage_capacity);
  delete[] lineage_ids;

  hybrid_test = new LIndex();
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	//lid_emplace_back(hybrid_test, rawdata[i]);
	lid_emplace_back(((RID**)hybrid_test)[1], ((_Size32_t*)hybrid_test)[0], ((_Size32_t*)hybrid_test)[1], rawdata[i]);

  printf("time lid_hybrid: %lf ms , space lid_hybrid: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * hybrid_test->capacity());
  delete hybrid_test;

  hybrid_test = new LIndex();
  time = chrono::steady_clock::now();
  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	//lid_emplace_back(hybrid_test, rawdata[i]);
	lid_emplace_back(hybrid_test, rawdata[i]);

  printf("time lid_hybrid2: %lf ms , space lid_hybrid2: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * hybrid_test->capacity());
  delete hybrid_test;

  time = chrono::steady_clock::now();

  for (int i = 0; i < Parameters::n_data; ++i)
	//lid_test.emplace_back(rawdata[i]);
	lid_test.emplace_back(rawdata[i]);
  printf("time lid_warpped: %lf ms , space lid_warpped: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(RID) * lid_test.capacity());
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
  test_vector();
  return 0;
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
  printf("time vector: %lf ms",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  for (auto& lidx : lineage)
  {
	space_consumption += lidx.second->capacity();
	//delete[] lidx.second;
  }
  printf(" space vector: %u Bytes\n", space_consumption * sizeof(RID));

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
  printf("time roaring: %lf ms",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count());
  space_consumption = 0;

  for (auto& lidx : lineage_r)
  {
	space_consumption += lidx.second->getSizeInBytes();
	//delete[] lidx.second;
  }
  printf(" space roaring: %u Bytes\n", space_consumption * sizeof(RID));
  lineage_r.clear();
  return 0;

}