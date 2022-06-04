#pragma once

template <class Primary, class Fallback>
class FallbackAllocator
{
public:
	void* allocate(size_t);
	void deallocate(void*);

	bool owns(void*);
};

template <class Primary, class Fallback>
void* FallbackAllocator<Primary, Fallback>::allocate(size_t size)
{
	void* ptr = Primary::allocate(size);
	if (ptr == nullptr)
	{
		return Fallback::allocate(n);
	}
	return ptr;
}

template <class Primary, class Fallback>
void FallbackAllocator<Primary, Fallback>::deallocate(void* ptr)
{
	if (Primary::owns(ptr))
	{
		Primary::deallocate(ptr);
	}
	Fallback::deallocate(ptr);
}

template <class Primary, class Fallback>
bool FallbackAllocator<Primary, Fallback>::owns(void* ptr)
{
	return Primary::owns(ptr) || Fallback::deallocate(ptr);
}