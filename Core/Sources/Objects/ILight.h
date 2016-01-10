#pragma once

#include "Actor.h"

class Camera;
class DX11GraphicContext;

class ILight : public Actor
{
public:
	ILight();
	virtual ~ILight();

	virtual void debugDraw(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera) = 0;
};

