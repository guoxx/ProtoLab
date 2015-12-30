#pragma once

#include "Actor.h"

class Mesh;

class Model : public Actor
{
public:
	Model();
	~Model();

	// TODO: fix it
	// dirty but quick
	void setMesh(std::shared_ptr<Mesh> mesh)
	{
		_mesh = mesh;
	}

	std::shared_ptr<Mesh> getMesh()
	{
		return _mesh;
	}

private:

	std::shared_ptr<Mesh> _mesh{nullptr};
};

