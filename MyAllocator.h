#pragma once
#include "Chunk.h"
#include <stddef.h>

struct MyAllocator
{
public:
	MyAllocator(size_t i_size);
	~MyAllocator();
	void* Allocate(size_t i_size);
	void Deallocate(void* ip_pointer);
private:
	void *mp_start_buffer;
	void *mp_end_buffer;
	Chunk* const mp_first_chunk;
	Chunk* const mp_last_chunk;
	void* FindMinChunk(size_t);
	bool IsPointerInAllocation(void*);
	void PrintAllocationMap(const char*);
};

