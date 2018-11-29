#include "LIndex.h"
#include <future>
#include <thread>
LIndex::LIndex() : _capacity(LIndex::initial_size), _size(0){
  container = (RID *)malloc(sizeof(RID) * _capacity);
}

LIndex::LIndex(RID r) : _size(1) {
  if (_capacity < 2)
	_capacity = 2;
  container = (RID *)malloc(sizeof(RID) * _capacity);
  container[0] = r;
}


LIndex::~LIndex()
{
  if (_capacity > 0)
  {
	_capacity = 0;
	delete[] container;
	container = nullptr;
  }
}

void LIndex::emplace_back(const RID r){
  if (_size != _capacity) 
	container[_size++] = r;
  else {
	//if (_size == max_capacity)
	  //throw std::length_error("LIndex too long");
	const _Size32_t n_size = _size + 1;
	const _Size32_t n_capacity = //_capacity > max_capacity - (_capacity >> 1) ? n_size:
	  (n_size > (_capacity + (_capacity >> 1))? n_size : (_capacity + (_capacity >> 1)));
  
	RID *n_container = (RID *)malloc(sizeof(RID) * n_capacity);
	memcpy(n_container, container, sizeof(RID) * _size);
	n_container[_size] = r;

	if (_capacity && container)
	  delete[] container;

	container = n_container;
	_capacity = n_capacity;
	_size = n_size;
  }
}

_Size32_t LIndex::size() const{
  return _size;
}

_Size32_t LIndex::capacity() const{
  return _capacity;
}

void LIndex::reserve(const _Size32_t _capacity)
{
  if (this->_capacity < _capacity && _capacity < max_capacity)
  {
	RID* _container = (RID*)malloc(sizeof(RID) * _capacity);
	if (container)
	{
	 //copy & destroy
	  if(_size > 0)
		memcpy(_container, container, _size * sizeof(RID));
	  
	  delete[] container;
	}
	container = _container;
	this->_capacity = _capacity;
  }
}

const RID * LIndex::data() const{
  return container;
}

__forceinline void LIndexUnwarpped::lid_emplace_back(
  RID *& container, _Size32_t & _capacity, _Size32_t & _size, const RID r)
{
  if (_size != _capacity)
	container[_size++] = r;
  else {
	//if (_size == max_capacity)
	  //throw std::length_error("LIndex too long");
	const _Size32_t n_size = _size + 1;
	const _Size32_t n_capacity = //_capacity > max_capacity - (_capacity >> 1) ? n_size:
	  //n_size > (_capacity + (_capacity >> 1)) ? n_size :
	  (_capacity + (_capacity >> 1));

	RID *n_container = (RID *)malloc(sizeof(RID) * n_capacity);
	memcpy(n_container, container, sizeof(RID) * _size);
	n_container[_size] = r;

	delete[] container;
	container = n_container;
	_capacity = n_capacity;
	_size = n_size;
  }
}

void LIndexUnwarpped::lid_emplace_back(LIndex * lindex, const RID r)
{
  lid_emplace_back(lindex->container, lindex->_capacity, lindex->_size, r);
}

__forceinline void LIndexUnwarpped::lid_init(
  RID *& rlist, const _Size32_t _capacity)
{
  rlist = (RID *)malloc(sizeof(RID) * _capacity);
}

__forceinline void LIndexUnwarpped::lid_deinit(RID *& container)
{ 
  if (container)
  {
	delete[] container;
	container = 0;
  }
}
