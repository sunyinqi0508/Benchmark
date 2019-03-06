#ifndef _LINDEX_H
#define _LINDEX_H
#include <limits>
#include "gc.h"
//#include <vector>
//#include <cstddef>
//template <typename _Ty>
#ifndef _MSC_VER
#define __forceinline //[[gnu::always_inline]] inline
#endif
using _Size32_t = unsigned int;
typedef _Size32_t RID;
class LIndex;
namespace LIndexUnwarpped {
	extern __forceinline void lid_emplace_back(
		RID*& rlist, _Size32_t& capacity, _Size32_t& _size, const RID r, GC *gc = 0);
	extern __forceinline void lid_emplace_back(LIndex* lindex, const RID r, GC *gc = 0);

	extern __forceinline void lid_init(RID *&rlist, const _Size32_t _capacity);
	extern __forceinline void lid_deinit(RID *&rlist);
}

class LIndex
{
private:
	static constexpr _Size32_t initial_size = 2;
	static constexpr _Size32_t max_capacity = std::numeric_limits<_Size32_t>::max();

	_Size32_t _capacity = 0;
	_Size32_t _size = 0;
	RID* container = nullptr;
	GC *garbage_collector = 0;
public:
	friend void LIndexUnwarpped::lid_emplace_back(LIndex*, const RID, GC*);
	int _alloc_count = 0;

	LIndex(GC *gc = 0);
	LIndex(RID r, GC *gc = 0);
	~LIndex();
	void emplace_back(const RID r);
	_Size32_t size() const;
	_Size32_t capacity() const;
	void reserve(const _Size32_t _capacity);
	const RID* data() const;
};

#endif