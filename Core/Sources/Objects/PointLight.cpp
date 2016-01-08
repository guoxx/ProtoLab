#include "stdafx.h"
#include "PointLight.h"


PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

void PointLight::setIntensity(DirectX::XMFLOAT3 intensity)
{
	_intensity = intensity;
}

DirectX::XMFLOAT3 PointLight::getIntensity() const
{
	return _intensity;
}

void PointLight::setRadius(float rStart, float rEnd)
{
	_radiusStart = rStart;
	_radiusEnd = rEnd;
}

float PointLight::getRadiusStart() const
{
	return _radiusStart;
}

float PointLight::getRadiusEnd() const
{
	return _radiusEnd;
}

void PointLight::debugDraw()
{
}
