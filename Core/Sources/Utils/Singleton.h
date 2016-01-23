#pragma once

template<class T>
class Singleton : public Noncopyable, Nonmovable
{
public:
	static T& getInstance()
	{
		static T inst{};
		return inst;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
};