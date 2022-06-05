#pragma once
#include "FreeListFixedSize.h"
#include <array>

//composite of freelists
template<size_t initialSize, size_t step, size_t numberOfSteps>
class Segregator final
{
public:
	Segregator(const size_t chunksPerList)
	{
		for (size_t i = 0; i < numberOfSteps; ++i)
		{
			_segregatedFreeLists[i] = new FreeListFixedSize(initialSize + i * step, chunksPerList);
		}
	}
	~Segregator()
	{
		for (size_t i = 0; i < numberOfSteps; ++i)
		{
			delete _segregatedFreeLists[i];
		}
	}

	//allocates if and only if there is a free list of this size, returns nullptr in other cases
	void* allocate(size_t size)
	{
		if ((size - initialSize) % step != 0 || size - initialSize < 0 || (size - initialSize) / step >= numberOfSteps)
		{
			//there is no appropriate size in segregator
			return nullptr;
		}

		const size_t appropriateSizeListIndex = (size - initialSize) / step;
		if (size < initialSize || appropriateSizeListIndex >= numberOfSteps)
		{
			return nullptr;
		}
		return _segregatedFreeLists[appropriateSizeListIndex]->allocate(size);
	}

	void free(void* ptr)
	{
		for (auto& freeList : _segregatedFreeLists)
		{
			if (freeList->owns(ptr))
			{
				freeList->free(ptr);
				return;
			}
		}
		assert(false && "Trying to free memory that wasn`t allocated with this segregator");
	}


private:
	//sorted array of free lists
	FreeListFixedSize* _segregatedFreeLists[numberOfSteps];
	//std::array<FreeListFixedSize, numberOfSteps> _segregatedFreeLists;
};