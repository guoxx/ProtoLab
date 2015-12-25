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
	_viewMat = DirectX::XMMatrixLookAtRH(eye, target, up);
}

void Camera::setViewParams(float fovy, float aspectRatio, float zNear, float zFar)
{
	_projMat = DirectX::XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar);
}

DirectX::XMMATRIX Camera::getViewMatrix()
{
	return _viewMat;
}

DirectX::XMMATRIX Camera::getInvViewMatrix()
{
	return DirectX::XMMatrixInverse(nullptr, _viewMat);
}

DirectX::XMMATRIX Camera::getProjectionMatrix()
{
	return _projMat;
}

DirectX::XMMATRIX Camera::getInvProjectionMatrix()
{
	return DirectX::XMMatrixInverse(nullptr, _projMat);
}

DirectX::XMMATRIX Camera::getViewProjectionMatrix()
{
	return DirectX::XMMatrixMultiply(_viewMat, _projMat);
}

DirectX::XMMATRIX Camera::getInvViewProjectionMatrix()
{
	return DirectX::XMMatrixInverse(nullptr, getViewProjectionMatrix());
}
