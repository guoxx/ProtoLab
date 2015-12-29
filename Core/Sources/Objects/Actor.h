#pragma once

#include <vector>

class Actor
{
public:
	enum ACTOR_DIRTY_FLAGS
	{
		ACTOR_DIRTY_LOCAL_MATRIX = 0x01 << 0,
		ACTOR_DIRTY_WORLD_MATRIX = 0x01 << 1,
	};

	Actor();
	~Actor();

	void update();

	void setPosition(DirectX::XMVECTOR position);
	DirectX::XMVECTOR getPosition() const;

	void setRotationPitchYawRoll(DirectX::XMVECTOR pitchYawRollInDegrees);
	DirectX::XMVECTOR getRotationPitchYawRoll() const;

	void setScale(DirectX::XMVECTOR scale);
	DirectX::XMVECTOR getScale() const;

	DirectX::XMMATRIX getLocalMatrix() const;
	DirectX::XMMATRIX getWorldMatrix() const;

	// get directions in local space
	DirectX::XMVECTOR getUp() const;
	DirectX::XMVECTOR getDown() const;
	DirectX::XMVECTOR getLeft() const;
	DirectX::XMVECTOR getRight() const;
	DirectX::XMVECTOR getForward() const;
	DirectX::XMVECTOR getBackward() const;

	// transformation in local space
	void move(DirectX::XMVECTOR direction, float distance);
	// rotate in local space, angle is measured by degrees
	void rotatePitchYawRoll(float pitch, float yaw, float roll);

protected:

	void _updateLocalMatrixImmediate();
	void _updateWorldMatrixImmediate();

	void _updateLocalMatrixDeferred();
	void _updateWorldMatrixDeferred();

private:

	std::weak_ptr<Actor> _parent;
	std::vector<std::shared_ptr<Actor>> _children;

protected:

	uint32_t _dirtyFlags;

	DirectX::XMVECTOR _translation;
	DirectX::XMVECTOR _rotationPitchYawRoll;
	DirectX::XMVECTOR _scale;

	DirectX::XMFLOAT4X4 _localMatrix;
	DirectX::XMFLOAT4X4 _worldMatrix;
};

