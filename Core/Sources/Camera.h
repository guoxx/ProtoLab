#pragma once

#include "Actor.h"

class Camera : public Actor
{
public:
	Camera();
	~Camera();

	void lookAt(DirectX::XMVECTOR eye, DirectX::XMVECTOR target, DirectX::XMVECTOR up);

	void setViewParams(float fovy, float aspectRatio, float zNear, float zFar);

	DirectX::XMMATRIX getViewMatrix();
	DirectX::XMMATRIX getInvViewMatrix();

	DirectX::XMMATRIX getProjectionMatrix();
	DirectX::XMMATRIX getInvProjectionMatrix();

	DirectX::XMMATRIX getViewProjectionMatrix();
	DirectX::XMMATRIX getInvViewProjectionMatrix();

private:

	DirectX::XMFLOAT4X4 _viewMat;
	DirectX::XMFLOAT4X4 _projMat;
};

