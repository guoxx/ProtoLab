#pragma once

#include "Actor.h"

using namespace DirectX;
#include "DirectXMesh/Utilities/WaveFrontReader.h"

class Mesh : public Actor
{
public:
	Mesh();
	~Mesh();

	void LoadMeshFromFile(const wchar_t* objFileName, bool ccw = true);
	void LoadMaterielFromFile(const wchar_t* mtlFileName);

//private:

	std::shared_ptr<WaveFrontReader<uint32_t>> _rawMeshData;
};

