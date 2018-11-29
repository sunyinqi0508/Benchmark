
#include <random>
#include <iostream>
#include <vector>
#include <chrono>


using namespace std;

int main()
{

  vector<unsigned> vector_test;

  auto time = chrono::steady_clock::now();

  for (int i = 0; i < 12345678; ++i)
	vector_test.emplace_back(i);

  printf("time stdvector: %lf ms , space stdvector: %u\n",
	chrono::duration<double, milli>(chrono::steady_clock::now() - time).count(),
	sizeof(unsigned) * (size_t)vector_test.capacity());
 // vector_test.~vector();

  return 0;
}
