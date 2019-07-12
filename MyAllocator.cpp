#include "MyAllocator.h"
#include <new>
#include <iostream>

MyAllocator::MyAllocator(size_t i_size)
	: mp_start_buffer{ new char[i_size] }
	, mp_end_buffer{ (char*)mp_start_buffer + i_size }
	, mp_first_chunk{ new(mp_start_buffer) Chunk((Chunk*)mp_start_buffer, (Chunk*)mp_end_buffer, true) }
	, mp_last_chunk {(Chunk*)mp_end_buffer}
{
}


MyAllocator::~MyAllocator()
{
	delete[] mp_start_buffer;
}


void* MyAllocator::Allocate(size_t i_size)
{
	Chunk* chunk = (Chunk*)FindMinChunk(i_size);
	if (chunk == nullptr) throw std::bad_alloc();
	size_t chunk_available_size = Chunk::GetAvailableSize(chunk);
	if (chunk_available_size > i_size)
	{
		void* new_chunk_place = (char*)chunk + sizeof(Chunk) + i_size;
		Chunk* new_chunk = new (new_chunk_place) Chunk(chunk, chunk->mp_next_chunk, true);
		if ((void*)chunk->mp_next_chunk != mp_end_buffer)
		{
			chunk->mp_next_chunk->mp_previous_chunk = new_chunk;
		}
		chunk->mp_next_chunk = new_chunk;
	}
	chunk->m_is_free = false;

	// tmp
	PrintAllocationMap("allocate");
	return chunk;
}

void* MyAllocator::FindMinChunk(size_t i_size) {
	size_t curr_size = Chunk::GetAvailableSize(mp_first_chunk);

	Chunk *min_chunk = nullptr;
	size_t min_size = (char*)mp_end_buffer - (char*)mp_start_buffer;
	for (Chunk *curr_chunk = mp_first_chunk;
		curr_chunk != mp_last_chunk;
		curr_chunk = curr_chunk->mp_next_chunk)
	{
		curr_size = Chunk::GetAvailableSize(curr_chunk);
		if (curr_chunk->m_is_free && curr_size < min_size && curr_size >= i_size) {
			min_size = curr_size;
			min_chunk = curr_chunk;
		}
	}
	return min_chunk;
}


void MyAllocator::Deallocate(void* ip_pointer) {
	try
	{
		if (!ip_pointer || !IsPointerInAllocation(ip_pointer))
		{
			throw std::bad_alloc();
		}
		Chunk *chunk = (Chunk*)ip_pointer;
		if (chunk->mp_previous_chunk != (Chunk*)mp_start_buffer && !IsPointerInAllocation(chunk->mp_previous_chunk)
			|| chunk->mp_next_chunk != (Chunk*)mp_end_buffer && !IsPointerInAllocation(chunk->mp_next_chunk))
		{
			throw std::bad_alloc();
		}

		bool is_previous_chunk_free = (chunk != mp_first_chunk || chunk->mp_previous_chunk == mp_first_chunk) & chunk->mp_previous_chunk->m_is_free;
		bool is_next_chunk_free = (chunk->mp_next_chunk != (Chunk*)mp_end_buffer) & chunk->mp_next_chunk->m_is_free;
		// union free chunks
		switch ((is_previous_chunk_free << 1) + is_next_chunk_free) {
		case 0: // BFB
			chunk->m_is_free = true;
			break;
		case 1: // BFF
			chunk->mp_next_chunk = chunk->mp_next_chunk->mp_next_chunk;
			chunk->mp_next_chunk->mp_previous_chunk = chunk;
			chunk->m_is_free = true;
			break;
		case 2: // FFB
			chunk->mp_previous_chunk->mp_next_chunk = chunk->mp_next_chunk;
			chunk->mp_next_chunk->mp_previous_chunk = chunk->mp_previous_chunk;
			break;
		case 3: // FFF
			chunk->mp_previous_chunk->mp_next_chunk = chunk->mp_next_chunk->mp_next_chunk;
			chunk->mp_next_chunk->mp_next_chunk->mp_previous_chunk = chunk->mp_previous_chunk;
			break;
		}
	}
	catch (...) {}

	//tmp
	PrintAllocationMap("deallocate");
	return;
}


bool MyAllocator::IsPointerInAllocation(void* ip_pointer) {
	return ip_pointer >= mp_start_buffer && ip_pointer <= mp_end_buffer;
}


void MyAllocator::PrintAllocationMap(const char* word) {
	std::cout << "================begin===================" << word << std::endl;
	int i = 0;
	for (Chunk* curr_chunk = mp_first_chunk;
		curr_chunk != mp_last_chunk;
		curr_chunk = curr_chunk->mp_next_chunk) {
		std::cout << i++ << "=> prev: " << curr_chunk->mp_previous_chunk
			<< " curr: " << curr_chunk
			<< " next: " << curr_chunk->mp_next_chunk
			<< " size: " << static_cast<double>(Chunk::GetAvailableSize(curr_chunk)) / 1024 / 1024
			<< "M is_free: " << curr_chunk->m_is_free << " " << std::endl;
	}
	std::cout << "================end====================" << std::endl;
}