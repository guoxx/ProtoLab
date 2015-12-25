#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh()
	: _rawMeshData{nullptr}
{
}


Mesh::~Mesh()
{
}

void Mesh::LoadMeshFromFile(const wchar_t* objFileName, bool ccw)
{
	_rawMeshData = std::make_shared<WaveFrontReader<uint32_t>>();
	_rawMeshData->Load(objFileName);

}

void Mesh::LoadMaterielFromFile(const wchar_t* mtlFileName)
{
	_rawMeshData->LoadMTL(mtlFileName);
}
