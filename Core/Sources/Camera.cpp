#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::lookAt(DirectX::XMVECTOR eye, DirectX::XMVECTOR target, DirectX::XMVECTOR up)
{
	DirectX::XMStoreFloat4x4(&_viewMat, DirectX::XMMatrixLookAtRH(eye, target, up));
}

void Camera::setViewParams(float fovy, float aspectRatio, float zNear, float zFar)
{
	DirectX::XMStoreFloat4x4(&_projMat, DirectX::XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar));
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	return DirectX::XMLoadFloat4x4(&_viewMat);
}

DirectX::XMMATRIX Camera::getInvViewMatrix() const
{
	return DirectX::XMMatrixInverse(nullptr, getViewMatrix());
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	return DirectX::XMLoadFloat4x4(&_projMat);
}

DirectX::XMMATRIX Camera::getInvProjectionMatrix() const
{
	return DirectX::XMMatrixInverse(nullptr, getProjectionMatrix());
}

DirectX::XMMATRIX Camera::getViewProjectionMatrix() const
{
	DirectX::XMMATRIX viewMat = DirectX::XMLoadFloat4x4(&_viewMat);
	DirectX::XMMATRIX projMat = DirectX::XMLoadFloat4x4(&_projMat);
	return DirectX::XMMatrixMultiply(viewMat, projMat);
}

DirectX::XMMATRIX Camera::getInvViewProjectionMatrix() const
{
	return DirectX::XMMatrixInverse(nullptr, getViewProjectionMatrix());
}
