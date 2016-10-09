#include "stdafx.h"
#include "Camera.h"

inline void EulerianToEulerAngle(const DirectX::FXMVECTOR q, double& pitch, double& yaw, double& roll)
{
	float qx = DirectX::XMVectorGetX(q);
	float qy = DirectX::XMVectorGetY(q);
	float qz = DirectX::XMVectorGetZ(q);
	float qw = DirectX::XMVectorGetW(q);

	double ysqr = qy * qy;
	double t0 = -2.0f * (ysqr + qz * qz) + 1.0f;
	double t1 = +2.0f * (qx * qy + qw * qz);
	double t2 = -2.0f * (qx * qz - qw * qy);
	double t3 = +2.0f * (qy * qz + qw * qx);
	double t4 = -2.0f * (qx * qx + ysqr) + 1.0f;

	t2 = t2 > 1.0f ? 1.0f : t2;
	t2 = t2 < -1.0f ? -1.0f : t2;

	pitch = std::asin(t2);
	roll = std::atan2(t3, t4);
	yaw = std::atan2(t1, t0);
}

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
	DirectX::XMVECTOR rotationQuat;
	DirectX::XMMatrixDecompose(&_scale, &rotationQuat, &_translation, mModel);
	double pitch, yaw, roll;
	EulerianToEulerAngle(rotationQuat, pitch, yaw, roll);
	_rotationPitchYawRoll = DirectX::XMVECTOR{ static_cast<float>(roll), static_cast<float>(yaw), static_cast<float>(pitch), 0.0f };

	_updateWorldMatrixDeferred();
}

void Camera::setViewParams(float fovy, float aspectRatio, float zNear, float zFar)
{
	DirectX::XMMATRIX mProj = DirectX::XMMatrixPerspectiveFovRH(DirectX::XMConvertToRadians(fovy), aspectRatio, zNear, zFar);
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
