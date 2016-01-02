#pragma once

class Nonmovable
{
public:
	Nonmovable() = default;
	~Nonmovable() = default;

	Nonmovable(const Nonmovable&&) = delete;
	Nonmovable& operator=(const Nonmovable&&) = delete;
};

