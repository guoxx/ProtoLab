#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&_projMat, DirectX::XMMatrixIdentity());
}

Camera::~Camera()
{
}

void Camera::lookAt(DirectX::XMVECTOR eye, DirectX::XMVECTOR target, DirectX::XMVECTOR up)
{
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtRH(eye, target, up);
	DirectX::XMMATRIX mModel = DirectX::XMMatrixInverse(nullptr, mView);
	DirectX::XMStoreFloat4x4(&_localMatrix, mModel);

	// TODO: better code
	// this will cause _localMatrix been updated again
	// and lose precision due to float arithmetic
	DirectX::XMMatrixDecompose(&_scale, &_rotationQuat, &_translation, mModel);
	_updateWorldMatrixDeferred();

	getForward();
}

void Camera::setViewParams(float fovy, float aspectRatio, float zNear, float zFar)
{
	DirectX::XMMATRIX mProj = DirectX::XMMatrixPerspectiveFovRH(fovy, aspectRatio, zNear, zFar);
	DirectX::XMStoreFloat4x4(&_projMat, mProj);
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	DirectX::XMMATRIX mModel = getWorldMatrix();
	DirectX::XMMATRIX mView = DirectX::XMMatrixInverse(nullptr, mModel);
	return mView;
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
	DirectX::XMMATRIX viewMat = getViewMatrix();
	DirectX::XMMATRIX projMat = getProjectionMatrix();
	DirectX::XMMATRIX viewPorjMat = DirectX::XMMatrixMultiply(viewMat, projMat);
	return viewPorjMat;
}

DirectX::XMMATRIX Camera::getInvViewProjectionMatrix() const
{
	return DirectX::XMMatrixInverse(nullptr, getViewProjectionMatrix());
}
