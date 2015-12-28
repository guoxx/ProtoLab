#include "stdafx.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Mesh.h"


Mesh::Mesh()
	: _primitives{}
	, _materiels{}
{
}


Mesh::~Mesh()
{
}

void Mesh::LoadMeshFromFile(const wchar_t* objFileName, bool ccw)
{
	std::string errmsg;
	char filename[1024];
	std::wcstombs(filename, objFileName, sizeof(filename));
	if (!tinyobj::LoadObj(_primitives, _materiels, errmsg, filename))
	{
		std::cout << errmsg << std::endl;
	}
}
