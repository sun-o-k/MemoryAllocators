#pragma once

struct FLNode
{
	FLNode* _next;
};


class FreeListFixedSize
{
public:
	FreeListFixedSize(size_t chunkSize, size_t numOfElements)
		:_bufferSize(numOfElements* chunkSize), _bufferStart(malloc(numOfElements * chunkSize)), _firstFreeNode(nullptr)
#ifdef DEBUG
		, _chunkSize(chunkSize)
#endif // DEBUG
 
	{
		assert(numOfElements > 0); // if number of elements is 0 malloced memory leaks
		union // as it is useful only in context of this function made inner
		{
			char* asChar;
			FLNode* asNode;
		};

		asChar = static_cast<char*>(_bufferStart); //initializing with the start of the buffer
		FLNode* currentChunk = _firstFreeNode = asNode;
		for (int i = 0; i < numOfElements; ++i)
		{
			currentChunk->_next = asNode;
			currentChunk = currentChunk->_next;
			asChar += chunkSize;//shifting to next free chunk
		}
		currentChunk->_next = nullptr; //last has no next, points to nullptr
	}
	~FreeListFixedSize() 
	{
		free(_bufferStart); 
	}

	//allocators can not be copied
	FreeListFixedSize(const FreeListFixedSize& other) = delete;
	FreeListFixedSize& operator=(const FreeListFixedSize& other) = delete;

	//but can be moved
	FreeListFixedSize(FreeListFixedSize&& other)
		:_bufferSize(other._bufferSize), _bufferStart(other._bufferStart), _firstFreeNode(other._firstFreeNode)
	{
		other._bufferStart = nullptr;
		other._firstFreeNode = nullptr;
	}
	FreeListFixedSize& operator=(FreeListFixedSize&& other)
	{
		_bufferSize = other._bufferSize;
		_bufferStart = other._bufferStart;
		_firstFreeNode = other._firstFreeNode;

		other._bufferStart = nullptr;
		other._firstFreeNode = nullptr;
	}


	//as simple as returning next pointer to user, time complexity is O(1)(really fast!)
	void* allocate(size_t size)//size is added only for interface consistency 
	{
#ifdef DEBUG
		assert(size == _chunkSize);
#endif // DEBUG
		if (_firstFreeNode == nullptr)
		{
			//buffer is exhausted
			return nullptr;
		}
		FLNode* freeChunk = _firstFreeNode; //extracting free chunk for user
		/*
		  as next chunk will be returned to user(and it is not free anymore)
		  redirecting next pointer to chunk after the one we will give to user to support linked list structure
		*/
		_firstFreeNode = freeChunk->_next;
		return freeChunk;
	}

	//freeing is also really fast
	void free(void* ptr)
	{
		FLNode* newFreeBlock = static_cast<FLNode*>(ptr);
		newFreeBlock->_next = _firstFreeNode;//inserting new block just after the start
		_firstFreeNode = newFreeBlock;
	}

	//checks if allocation of a particular block was made by this allocator
	bool owns(void* ptr)
	{
		char* startAsChar = static_cast<char*>(_bufferStart);
		return ptr >= startAsChar && ptr <= startAsChar + _bufferSize;
	}

private:
	size_t _bufferSize; //stores only for being able to implement owns function
	void* _bufferStart;
	FLNode* _firstFreeNode;
#ifdef DEBUG
	size_t _chunkSize;
#endif // DEBUG

};


