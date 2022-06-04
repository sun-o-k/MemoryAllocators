#include <iostream>
#include "Utils.h"
#include "StackAllocator.h"

void stackTester();

int main()
{
	stackTester();
}

void stackTester()
{
	static constexpr size_t aligment = 8;
	static constexpr size_t stackSize = 512;

	StackAllocator<aligment> myStackAllocator(stackSize);
	const size_t arrayLength = 16;
	int* array[arrayLength];
	for (int i = 0; i < arrayLength; ++i)
	{
		void* freeMemory = myStackAllocator.allocate(sizeof(int));
		array[i] = new(freeMemory) int{ i };
	}
	for (int i = arrayLength - 1; i > 0; --i)
	{
		myStackAllocator.free(array[i]);
		array[i] = nullptr;
	}
	void* freeMemory = myStackAllocator.allocate(sizeof(int));
	array[0] = new(freeMemory) int{ 0 };
	freeMemory = myStackAllocator.allocate(sizeof(int));
	array[1] = new(freeMemory) int{ 1 };
	myStackAllocator.free(array[1]);
	myStackAllocator.free(array[0]);
}