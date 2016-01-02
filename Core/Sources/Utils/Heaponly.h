#pragma once

class Heaponly
{
public:
	Heaponly() = default;
	~Heaponly() = default;

	void* operator new(std::size_t count) = delete;
	void* operator new[](std::size_t count) = delete;
};

