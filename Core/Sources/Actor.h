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

	void setRotationQuaternion(DirectX::XMVECTOR quat);
	DirectX::XMVECTOR getRotationQuaternion() const;

	void setScale(DirectX::XMVECTOR scale);
	DirectX::XMVECTOR getScale() const;

	DirectX::XMMATRIX getLocalMatrix() const;
	DirectX::XMMATRIX getWorldMatrix() const;

private:

	void _updateLocalMatrixImmediate();
	void _updateWorldMatrixImmediate();

	void _updateLocalMatrixDeferred();
	void _updateWorldMatrixDeferred();

private:

	uint32_t _dirtyFlags;

	std::weak_ptr<Actor> _parent;
	std::vector<std::shared_ptr<Actor>> _children;

	DirectX::XMVECTOR _translation;
	DirectX::XMVECTOR _rotationQuat;
	DirectX::XMVECTOR _scale;

	DirectX::XMFLOAT4X4 _localMatrix;
	DirectX::XMFLOAT4X4 _worldMatrix;
};

