#pragma once
#include "Utils.h"
#include <assert.h>

template <int aligment> 
class StackAllocator final
{
public:
	StackAllocator(size_t bufferSize);
	~StackAllocator();

	void* allocate(size_t size);
	void free(void*);
	void reset();
private:
	//where the buffer initially starts
	char* _bufferStart; //char* is for convenience of pointer addition
	//where the buffer initially ends
	char* _bufferEnd;
	//pointer to first free byte in buffer
	char* _currPosition;
};

template <int aligment>
StackAllocator<aligment>::StackAllocator(size_t bufferSize)
	:_bufferStart(reinterpret_cast<char*>(malloc(bufferSize))), _bufferEnd(_bufferStart + bufferSize),
	_currPosition(_bufferStart)
{

}

template <int aligment>
StackAllocator<aligment>::~StackAllocator()
{
	::free(_bufferStart);
}

template <int aligment>
void* StackAllocator<aligment>::allocate(size_t size)
{
	const size_t allignedSize = Utils::getAligned(size, aligment);
	/*
	every allocation has overhead of aligned sizeof(size_t) to support free operation
	free has only pointer as a parameter, size has to be deducted from elsewhere(in our case from allocation header )
	*/
	static constexpr size_t headerSize = sizeof(size_t);
	const size_t allocationSize = allignedSize + headerSize;
	if (_currPosition + allocationSize > _bufferEnd)
	{
		//buffer is exhausted
		return nullptr;
	}

	union
	{
		void* asVoid;
		size_t* asSize;
		char* asChar;
	};
	asVoid = _currPosition;
	*asSize = allignedSize;//saving 
	_currPosition = asChar + allocationSize;
	asChar += headerSize;//returning address after header to user(he doesn`t need to know about header existence)
	return asVoid;
}

template <int aligment>
void StackAllocator<aligment>::free(void* ptrToFree)
{
	if (ptrToFree == nullptr)
	{
		return;
	}
	union
	{
		void* asVoid;
		size_t* asSize;
		char* asChar;
	};
	asVoid = ptrToFree;
	asSize -= 1;
	const bool isTheLastAllocation = asChar + sizeof(size_t) + *asSize == _currPosition;
	//only last allocated chunk can be freed
	if (isTheLastAllocation)
	{
		_currPosition = asChar;
	}
	else
	{
		assert(false && "Deallocation is done not in LIFO fashion");
	}
}

template <int aligment>
void StackAllocator<aligment>::reset()
{
	_currPosition = _bufferStart;
}