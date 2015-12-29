#include "stdafx.h"
#include "Actor.h"


Actor::Actor()
	: _parent{}
	, _children{}
	, _dirtyFlags{0}
{
	_translation = DirectX::XMVectorZero();
	_rotationQuat = DirectX::XMQuaternionIdentity();
	_scale = DirectX::XMVECTOR{1, 1, 1};

	DirectX::XMStoreFloat4x4(&_localMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&_worldMatrix, DirectX::XMMatrixIdentity());
}

Actor::~Actor()
{
}

void Actor::update()
{
	if (_dirtyFlags & ACTOR_DIRTY_LOCAL_MATRIX)
	{
		_updateLocalMatrixImmediate();
	}

	if (_dirtyFlags & ACTOR_DIRTY_WORLD_MATRIX)
	{
		_updateWorldMatrixImmediate();
	}

	for (auto aActor : _children)
	{
		aActor->update();
	}
}

void Actor::setPosition(DirectX::XMVECTOR position)
{
	_translation = position;
	_updateLocalMatrixDeferred();
}

DirectX::XMVECTOR Actor::getPosition() const
{
	return _translation;
}

void Actor::setRotationQuaternion(DirectX::XMVECTOR quat)
{
	_rotationQuat = quat;
	_updateLocalMatrixDeferred();
}

DirectX::XMVECTOR Actor::getRotationQuaternion() const
{
	return _rotationQuat;
}

void Actor::setScale(DirectX::XMVECTOR scale)
{
	_scale = scale;
	_updateLocalMatrixDeferred();
}

DirectX::XMVECTOR Actor::getScale() const
{
	return _scale;
}

DirectX::XMMATRIX Actor::getLocalMatrix() const
{
	CHECK(!(_dirtyFlags & Actor::ACTOR_DIRTY_LOCAL_MATRIX));
	DirectX::XMMATRIX mLocal = DirectX::XMLoadFloat4x4(&_localMatrix);
	return mLocal;
}

DirectX::XMMATRIX Actor::getWorldMatrix() const
{
	CHECK(!(_dirtyFlags & Actor::ACTOR_DIRTY_WORLD_MATRIX));
	DirectX::XMMATRIX mWorld = DirectX::XMLoadFloat4x4(&_worldMatrix);
	return mWorld;
}

DirectX::XMVECTOR Actor::getUp() const
{
	DirectX::XMVECTOR upDir{_localMatrix._21, _localMatrix._22, _localMatrix._23, 0};
	return DirectX::XMVector3Normalize(upDir);
}

DirectX::XMVECTOR Actor::getDown() const
{
	DirectX::XMVECTOR downDir{-_localMatrix._21, -_localMatrix._22, -_localMatrix._23, 0};
	return DirectX::XMVector3Normalize(downDir);
}

DirectX::XMVECTOR Actor::getLeft() const
{
	DirectX::XMVECTOR leftDir{_localMatrix._11, _localMatrix._12, _localMatrix._13, 0};
	return DirectX::XMVector3Normalize(leftDir);
}

DirectX::XMVECTOR Actor::getRight() const
{
	DirectX::XMVECTOR rightDir{-_localMatrix._11, -_localMatrix._12, -_localMatrix._13, 0};
	return DirectX::XMVector3Normalize(rightDir);
}

DirectX::XMVECTOR Actor::getForward() const
{
	// right hand coordinate
	DirectX::XMVECTOR forwardDir{-_localMatrix._31, -_localMatrix._32, -_localMatrix._33, 0};
	return DirectX::XMVector3Normalize(forwardDir);
}

DirectX::XMVECTOR Actor::getBackward() const
{
	// right hand coordinate
	DirectX::XMVECTOR backwardDir{_localMatrix._31, _localMatrix._32, _localMatrix._33, 0};
	return DirectX::XMVector3Normalize(backwardDir);
}

void Actor::move(DirectX::XMVECTOR direction, float distance)
{
	_translation = DirectX::XMVectorMultiplyAdd(direction, DirectX::XMVECTOR{distance, distance, distance, distance}, _translation);
	_updateLocalMatrixDeferred();
}

void Actor::_updateLocalMatrixImmediate()
{
	if (true)
	//if (_dirtyFlags & ACTOR_DIRTY_LOCAL_MATRIX)
	{
		DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslationFromVector(_translation);
		DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(_rotationQuat);
		DirectX::XMMATRIX mScale = DirectX::XMMatrixScalingFromVector(_scale);
		// mRot * mTrans * mScale
		DirectX::XMMATRIX mLocal = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(mRot, mTrans), mScale);
		DirectX::XMStoreFloat4x4(&_localMatrix, mLocal);

		_dirtyFlags &= (~ACTOR_DIRTY_LOCAL_MATRIX);
	}
}

void Actor::_updateWorldMatrixImmediate()
{
	if (true)
	//if (_dirtyFlags & ACTOR_DIRTY_WORLD_MATRIX)
	{
		auto parent = _parent.lock();
		if (parent)
		{
			parent->_updateWorldMatrixImmediate();	
			DirectX::XMMATRIX parentWorldMatrix = parent->getWorldMatrix();
			_updateLocalMatrixImmediate();
			DirectX::XMMATRIX currentLocalMatrix = getLocalMatrix();
			DirectX::XMMATRIX mWorld = DirectX::XMMatrixMultiply(currentLocalMatrix, parentWorldMatrix);
			DirectX::XMStoreFloat4x4(&_worldMatrix, mWorld);
		}
		else
		{
			_updateLocalMatrixImmediate();
			_worldMatrix = _localMatrix;
		}

		_dirtyFlags &= (~ACTOR_DIRTY_WORLD_MATRIX);
	}
}

void Actor::_updateLocalMatrixDeferred()
{
	_updateLocalMatrixImmediate();
	_updateWorldMatrixImmediate();
	//_dirtyFlags |= Actor::ACTOR_DIRTY_LOCAL_MATRIX;	
	//_dirtyFlags |= Actor::ACTOR_DIRTY_WORLD_MATRIX;
}

void Actor::_updateWorldMatrixDeferred()
{
	_updateWorldMatrixImmediate();
	//_dirtyFlags |= Actor::ACTOR_DIRTY_WORLD_MATRIX;	
}
