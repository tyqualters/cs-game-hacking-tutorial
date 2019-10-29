#pragma once
#include <ctime>

template<class T = uintptr_t, class U = void*>
T dereference_pointer(const U ptr)
{
	if ((void*)ptr != nullptr)
		return *(T*)ptr;
	else
		return { 0 };
}
template<class T = uintptr_t, class U = void*>
bool set_pointer(const U ptr, const T value)
{
	if ((void*)ptr != nullptr)
	{
		*(T*)ptr = value;
		return true;
	}
	else
	{
		return false;
	}
}

void fApplyRandomOffset(float& target, const float fMin, const float fMax)
{
	srand(time(NULL));
	target = target + (fMin + ((float)rand() / fMax) * (fMax - fMin));
}