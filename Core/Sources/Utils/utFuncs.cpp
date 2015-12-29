#include "stdafx.h"
#include "utFuncs.h"

void print(const char* fmt, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	OutputDebugStringA(buffer);
}

void checkSucceeded(uint32_t result)
{
	if (result)
	{
	}
	else
	{
		abort();
	}
}