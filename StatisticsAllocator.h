#pragma once
#include <unordered_map>
#include <string>
#include <stdio.h>

template<typename Alloc>
concept Allocator = requires(Alloc a, size_t size, void* ptr)
{
	a.allocate(size);
	a.free(ptr);
};

template <class Allocator>
class StatisticsAllocator final
{
public:

	StatisticsAllocator(Allocator& mainAllocator)
		:_mainAllocator(mainAllocator) {}

	void* allocate(size_t size)
	{
		++_numberOfAllocations;
		if (_allocationSizeToQuantity.contains(size))
		{
			++_allocationSizeToQuantity[size];
		}
		else
		{
			_allocationSizeToQuantity.emplace(size, 1);
		}
		void* chunk = _mainAllocator.allocate(size);
		if (chunk != nullptr)
		{
			++_numberOfSuccesfulAllocations;
		}
		return chunk;
	}

	void free(void* ptr)
	{
		++_numberOfDeallocations;
		return _mainAllocator.free(ptr);
	}

	void printStatistics() const
	{
		std::printf("numberOfAllocations: %i\nnumberOfSuccesfulAllocations: %i\nnumberOfDeallocations: %i\n ", _numberOfAllocations, _numberOfSuccesfulAllocations, _numberOfDeallocations);
	}

private:
	int _numberOfAllocations = 0;
	int _numberOfSuccesfulAllocations = 0;
	int _numberOfDeallocations = 0;
	std::unordered_map<size_t, int> _allocationSizeToQuantity;

	Allocator& _mainAllocator;
};