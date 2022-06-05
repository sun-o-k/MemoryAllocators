#include <iostream>
#include "Utils.h"
#include "StackAllocator.h"
#include "FreeListFixedSize.h"
#include "Segregator.h"
#include "StatisticsAllocator.h"

void stackTester();
void freeListTester();
void segregatorTester();
void statisticsTester();

int main()
{
	stackTester();
	freeListTester();
	segregatorTester();
	statisticsTester();
}

void statisticsTester()
{
	static constexpr size_t aligment = 8;
	static constexpr size_t stackSize = 512;

	StackAllocator<aligment> myStackAllocator(stackSize);

	StatisticsAllocator<StackAllocator<aligment>> statisticsAllocator{ myStackAllocator };

	const size_t arrayLength = 16;
	int* array[arrayLength];
	for (int i = 0; i < arrayLength; ++i)
	{
		void* freeMemory = statisticsAllocator.allocate(sizeof(int));
		array[i] = new(freeMemory) int{ i };
	}
	for (int i = arrayLength - 1; i >= 0; --i)
	{
		statisticsAllocator.free(array[i]);
		array[i] = nullptr;
	}
	void* freeMemory = statisticsAllocator.allocate(sizeof(int));
	array[0] = new(freeMemory) int{ 0 };
	freeMemory = statisticsAllocator.allocate(sizeof(int));
	array[1] = new(freeMemory) int{ 1 };
	statisticsAllocator.free(array[1]);
	statisticsAllocator.free(array[0]);

	statisticsAllocator.printStatistics();
}

void segregatorTester()
{
	static constexpr size_t initialSize = 8;
	static constexpr size_t step = 4;
	static constexpr size_t numberOfSteps = 4;
	static constexpr size_t numberOfChunksPerList = 20;	

	//segregator will contain 4 lists of chunk sizes 8, 12, 16, 20, every list has 20 chunks
	Segregator<initialSize, step, numberOfSteps> segregator{ numberOfChunksPerList };
	struct EightByteStruct
	{
		int a;
		int b;
	};
	struct TwelveByteStruct
	{
		int a;
		int b;
		int c;
	};

	EightByteStruct** container = new EightByteStruct*[numberOfChunksPerList];
	for (int i = 0; i < numberOfChunksPerList; ++i)
	{
		void* freeBlock = segregator.allocate(sizeof(EightByteStruct));
		container[i] = new(freeBlock) EightByteStruct{ i, i + 1 };
	}

	TwelveByteStruct** containerTwelve = new TwelveByteStruct *[numberOfChunksPerList];
	for (int i = 0; i < numberOfChunksPerList; ++i)
	{
		void* freeBlock = segregator.allocate(sizeof(TwelveByteStruct));
		containerTwelve[i] = new(freeBlock) TwelveByteStruct{ i, i + 1 };
	}

	void* testInapropriateSize = segregator.allocate(sizeof(int));
	assert(testInapropriateSize == nullptr);
	for (int i = 0; i < numberOfChunksPerList; ++i)
	{
		segregator.free(container[i]);
	}
	for (int i = 0; i < numberOfChunksPerList; ++i)
	{
		segregator.free(containerTwelve[i]);
	}

	delete[] container;

}


void freeListTester()
{
	struct EightByteStruct
	{
		int a;
		int b;
	};

	static constexpr size_t aligment = 8;
	static constexpr size_t chunkSize = 8;
	static constexpr size_t numberOfChunks = 20;
	FreeListFixedSize freeList{ chunkSize, numberOfChunks };

	assert(sizeof(EightByteStruct) == chunkSize);

	EightByteStruct** container = new EightByteStruct*[numberOfChunks];
	assert(!freeList.owns(container));
	for (int i = 0; i < numberOfChunks; ++i)
	{
		void* freeBlock = freeList.allocate(sizeof(EightByteStruct));
		assert(freeList.owns(freeBlock));
		container[i] = new(freeBlock) EightByteStruct {i, i + 1};
	}
	for (int i = 0; i < numberOfChunks; ++i)
	{
		//std::cout << container[i]->a << ' ' << container[i]->b << '\n';
	}
	delete[] container;
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
	for (int i = arrayLength - 1; i >= 0; --i)
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