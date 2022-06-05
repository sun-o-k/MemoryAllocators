#pragma once

/*
Non growing free list supporting various sizes
*/

template<size_t aligment>
class FreeList final
{
public:
	FreeList(size_t bufferSize);
	~FreeList();

	void* allocate(size_t)
	{

	}
};