#pragma once

#include "ILight.h"

class Mesh;

class PointLight : public ILight
{
public:
	enum AXIS
	{
		AXIS_START = 0,
		POSITIVE_X = AXIS_START,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z,
		AXIS_END,
	};

	PointLight();
	virtual ~PointLight();

	void setIntensity(DirectX::XMFLOAT3 intensity);
	DirectX::XMFLOAT3 getIntensity() const;

	void setRadius(float rStart, float rEnd);
	float getRadiusStart() const;
	float getRadiusEnd() const;

	DirectX::XMMATRIX getViewProj(AXIS axis) const;

	virtual DX11DepthStencilRenderTarget* getShadowMapRenderTarget() override final;
	virtual void debugDraw(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera) override final;

private:
	DirectX::XMFLOAT3 _intensity;
	float _radiusStart;
	float _radiusEnd;

	std::shared_ptr<Mesh> _sphere;
};

