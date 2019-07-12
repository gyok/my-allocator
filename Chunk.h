#pragma once
struct Chunk
{
	Chunk* mp_previous_chunk;
	Chunk* mp_next_chunk;
	bool m_is_free;
	Chunk() : Chunk(nullptr, nullptr, false) {};
	Chunk(Chunk* ip_previous_chunk, Chunk* ip_next_chunk, bool i_is_free)
		: mp_previous_chunk{ip_previous_chunk}, mp_next_chunk{ip_next_chunk}, m_is_free{i_is_free}
	{};
	static size_t GetAvailableSize(Chunk *chunk) { return (char*)chunk->mp_next_chunk - (char*)chunk - sizeof(Chunk); }
};

