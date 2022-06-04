#include <iostream>
#include "Utils.h"
#include "StackAllocator.h"

int main()
{
	static constexpr size_t aligment = 8;
	static constexpr size_t stackSize = 512;

	StackAllocator<aligment> myStackAllocator(stackSize);
	const size_t arrayLength = 16;
	int* array[arrayLength];
	for (int i = 0; i < arrayLength; ++i)
	{
		array[i] = new(myStackAllocator.allocate(sizeof(int))) int{i};
	}
	for (int i = arrayLength - 1; i > 0; --i)
	{
		myStackAllocator.free(array[i]);
	}
	
}