#pragma once

#include "bmem.h"

inline void *operator new(size_t size)
{
	return bmalloc(size);
}

inline void operator delete(void *data)
{
	bfree(data);
}

inline void *operator new[](size_t size)
{
	return bmalloc(size);
}

inline void operator delete[](void *data)
{
	bfree(data);
}
