#pragma once

#include "Actor.h"

class Camera;
class DX11GraphicContext;
class DX11DepthStencilRenderTarget;

class ILight : public Actor
{
public:
	ILight();
	virtual ~ILight();


	virtual DX11DepthStencilRenderTarget* getShadowMapRenderTarget() = 0;
	virtual void debugDraw(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera) = 0;
};

