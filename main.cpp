#include <iostream>
#include "Utils.h"
#include "StackAllocator.h"
#include "FreeList.h"

void stackTester();
void freeListTester();

int main()
{
	stackTester();
	freeListTester();
}

struct EightByteStruct
{
	int a;
	int b;
};

std::ostream& operator<<(std::ostream& os, const EightByteStruct& eightByteStruct)
{
	return os << eightByteStruct.a << ' ' << eightByteStruct.b;
}

void freeListTester()
{
	static constexpr size_t aligment = 8;
	static constexpr size_t chunkSize = 8;
	static constexpr size_t numberOfChunks = 20;
	FreeListFixedSize<chunkSize> freeList{ numberOfChunks };

	assert(sizeof(EightByteStruct) == chunkSize);

	EightByteStruct** container = new EightByteStruct*[numberOfChunks];
	for (int i = 0; i < numberOfChunks; ++i)
	{
		void* freeBlock = freeList.allocate(sizeof(EightByteStruct));
		container[i] = new(freeBlock) EightByteStruct {i, i + 1};
	}
	for (int i = 0; i < numberOfChunks; ++i)
	{
		std::cout << *container[i] << '\n';
	}
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