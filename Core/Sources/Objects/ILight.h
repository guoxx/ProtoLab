#pragma once

#include "Actor.h"

class ILight : public Actor
{
public:
	ILight();
	virtual ~ILight();

	virtual void debugDraw() = 0;
};

