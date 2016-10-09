#pragma once

#include "Actor.h"

class Camera : public Actor
{
public:
	Camera();
	~Camera();

	void lookAt(DirectX::XMVECTOR eye, DirectX::XMVECTOR target, DirectX::XMVECTOR up);

	void setViewParams(float fovy, float aspectRatio, float zNear, float zFar);

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getInvViewMatrix() const;

	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getInvProjectionMatrix() const;

	DirectX::XMMATRIX getViewProjectionMatrix() const;
	DirectX::XMMATRIX getInvViewProjectionMatrix() const;

private:

	// column-major matrix data
	DirectX::XMFLOAT4X4 _projMat;
};

