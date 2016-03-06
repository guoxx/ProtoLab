#include "stdafx.h"
#include "PointLight.h"
#include "Camera.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Material.h"
#include "../DX11RHI/DX11GraphicContext.h"


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

DirectX::XMMATRIX PointLight::getViewProj(AXIS axis) const
{
	float fov = 90.0f;
	float zNear = 0.01f;
	float zFar = _radiusEnd;
	float aspect = 1.0f;
	assert(zFar > zNear);

	DirectX::XMMATRIX mProj = DirectX::XMMatrixPerspectiveFovRH(fov, aspect, zNear, zFar);

	static const DirectX::XMVECTOR axes[AXIS_END] = {
		{1.0f, 0.0f, 0.0f, 0.0f},		// POSITIVE_X
		{-1.0f, 0.0f, 0.0f, 0.0f},		// NEGATIVE_X
		{0.0f, 1.0f, 0.0f, 0.0f},		// POSITIVE_Y
		{0.0f, -1.0f, 0.0f, 0.0f},		// NEGATIVE_Y
		{0.0f, 0.0f, 1.0f, 0.0f},		// POSITIVE_Z
		{0.0f, 0.0f, -1.0f, 0.0f},		// NEGATIVE_Z
	};
	static const DirectX::XMVECTOR upDir[AXIS_END] = {
		{0.0f, 1.0f, 0.0f, 0.0f},		// POSITIVE_X
		{0.0f, 1.0f, 0.0f, 0.0f},		// NEGATIVE_X
		{0.0f, 0.0f, 1.0f, 0.0f},		// POSITIVE_Y
		{0.0f, 0.0f, -1.0f, 0.0f},		// NEGATIVE_Y
		{0.0f, 1.0f, 0.0f, 0.0f},		// POSITIVE_Z
		{0.0f, 1.0f, 0.0f, 0.0f},		// NEGATIVE_Z
	};

	DirectX::XMVECTOR position = getPosition();
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookToRH(position, axes[axis], upDir[axis]);
	
	DirectX::XMMATRIX mViewProj = DirectX::XMMatrixMultiply(mView, mProj);
	return mViewProj;
}

DX11DepthStencilRenderTarget* PointLight::getShadowMapRenderTarget()
{
	// TODO
	return nullptr;
}

void PointLight::debugDraw(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Camera> camera)
{
	if (!_sphere)
	{
		_sphere = Mesh::createSphere(getRadiusStart(), 16, true);
	}

	DirectX::XMMATRIX mModel = getWorldMatrix();

	{
		// update point light constant buffer
		ComPtr<ID3D11Buffer> pointLightCB = _sphere->getMaterial()->getPsConstantBuffer(MaterialCB::EmissiveMaterial::PointLightReg);
		DX11ResourceMapGuard pointLightRes{ gfxContext.get(), pointLightCB.Get() };
		MaterialCB::EmissiveMaterial::PointLight* dataPtr = pointLightRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();

		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMVECTOR lightPositionInLocalSpace = getPosition();
		lightPositionInLocalSpace = DirectX::XMVectorSetW(lightPositionInLocalSpace, 1);
		DirectX::XMVECTOR lightPositionInCameraSpace = DirectX::XMVector4Transform(lightPositionInLocalSpace, mModelView);
		DirectX::XMStoreFloat4(&dataPtr->lightPositionInCameraSpace, lightPositionInCameraSpace);
		DirectX::XMFLOAT3 intensity = getIntensity();
		dataPtr->intensity = DirectX::XMFLOAT4{ intensity.x, intensity.y, intensity.z, 0 };
		dataPtr->radiusStart = getRadiusStart();
		dataPtr->radiusEnd = getRadiusEnd();
	}

	gfxContext->OMSetBlendState(RHI::getInstance().getRenderStateSet()->Additive());
	_sphere->draw(mModel, camera, nullptr);
	gfxContext->OMSetBlendState(RHI::getInstance().getRenderStateSet()->Opaque());
}
