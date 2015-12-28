#pragma once

#include "Actor.h"
#include "tiny_obj_loader.h"

class Mesh : public Actor
{
public:
	Mesh();
	~Mesh();

	void LoadMeshFromFile(const wchar_t* objFileName, bool ccw = true);

private:

	std::vector<tinyobj::shape_t> _primitives;
	std::vector<tinyobj::material_t> _materiels;
};

