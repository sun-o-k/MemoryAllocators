#pragma once

namespace Utils
{
	size_t getAligned(size_t size, size_t aligment)
	{
		if (size == 0)
		{
			/*
			Still allocating even if request is of zero size. Why? Think of alternative.
			If request fails the same way as it fails when no memory is left in buffer then how to distinct
			between these two cases. One more reason is that malloc has same behavior
			*/
			return aligment;
		}
		return size + (aligment - (size % aligment)) % aligment; //second operator % is used to handle cases when size % aligment == 0
	}
}