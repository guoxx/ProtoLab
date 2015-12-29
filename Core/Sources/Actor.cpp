#include "stdafx.h"
#include "Actor.h"


Actor::Actor()
{
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

void Actor::_updateLocalMatrixImmediate()
{
	if (_dirtyFlags & ACTOR_DIRTY_LOCAL_MATRIX)
	{
		DirectX::XMMATRIX mTrans = DirectX::XMMatrixTranslationFromVector(_translation);
		DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(_rotationQuat);
		DirectX::XMMATRIX mScale = DirectX::XMMatrixScalingFromVector(_scale);
		// mTrans * mRot * mScale
		DirectX::XMMATRIX mLocal = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(mTrans, mRot), mScale);
		DirectX::XMStoreFloat4x4(&_localMatrix, mLocal);

		_dirtyFlags &= (~ACTOR_DIRTY_LOCAL_MATRIX);
	}
}

void Actor::_updateWorldMatrixImmediate()
{
	if (_dirtyFlags & ACTOR_DIRTY_WORLD_MATRIX)
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
	_dirtyFlags |= Actor::ACTOR_DIRTY_LOCAL_MATRIX;	
	_dirtyFlags |= Actor::ACTOR_DIRTY_WORLD_MATRIX;
}

void Actor::_updateWorldMatrixDeferred()
{
	_dirtyFlags |= Actor::ACTOR_DIRTY_WORLD_MATRIX;	
}
