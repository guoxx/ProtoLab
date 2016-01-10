#include "stdafx.h"
#include "Mesh.h"
#include "Primitive.h"
#include "Material.h"

#include <DirectXTK/Inc/GeometricPrimitive.h>


void Mesh::loadCoordinateSystemFrame()
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};

	Vertex vexBuf[] = {
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(1, 0, 0),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 0, 1),
	};
	uint32_t idxBuf[] = {0, 1};
	Vertex* pVexBuf = vexBuf;

	std::shared_ptr<Primitive> xAxis = std::make_shared<Primitive>();
	xAxis->_name = "x-axis";
	xAxis->_matIdx = 0;
	xAxis->_materialId = Primitive::MATERIAL_ID::MATERIAL_VERTEX_COLOR;
	xAxis->_positionBuffer = RHI::getInst().createVertexBuffer(pVexBuf+0, sizeof(Vertex)*2);
	xAxis->_indexBuffer = RHI::getInst().createIndexBuffer(idxBuf, sizeof(idxBuf));
	xAxis->_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	xAxis->_indicesFormat = DXGI_FORMAT_R32_UINT;
	xAxis->_indicesCount = 2;
	_primitives.push_back(xAxis);

	std::shared_ptr<Primitive> yAxis = std::make_shared<Primitive>();
	yAxis->_name = "y-axis";
	yAxis->_matIdx = 1;
	yAxis->_materialId = Primitive::MATERIAL_ID::MATERIAL_VERTEX_COLOR;
	yAxis->_positionBuffer = RHI::getInst().createVertexBuffer(pVexBuf+2, sizeof(Vertex)*2);
	yAxis->_indexBuffer = RHI::getInst().createIndexBuffer(idxBuf, sizeof(idxBuf));
	yAxis->_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	yAxis->_indicesFormat = DXGI_FORMAT_R32_UINT;
	yAxis->_indicesCount = 2;
	_primitives.push_back(yAxis);

	std::shared_ptr<Primitive> zAxis = std::make_shared<Primitive>();
	zAxis->_name = "z-axis";
	zAxis->_matIdx = 2;
	zAxis->_materialId = Primitive::MATERIAL_ID::MATERIAL_VERTEX_COLOR;
	zAxis->_positionBuffer = RHI::getInst().createVertexBuffer(pVexBuf+4, sizeof(Vertex)*2);
	zAxis->_indexBuffer = RHI::getInst().createIndexBuffer(idxBuf, sizeof(idxBuf));
	zAxis->_topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	zAxis->_indicesFormat = DXGI_FORMAT_R32_UINT;
	zAxis->_indicesCount = 2;
	_primitives.push_back(zAxis);

	tinyobj::material_t matX{};
	matX.diffuse[0] = 1;
	matX.diffuse[1] = 0;
	matX.diffuse[2] = 0;
	_materiels.push_back(matX);

	tinyobj::material_t matY{};
	matY.diffuse[0] = 0;
	matY.diffuse[1] = 1;
	matY.diffuse[2] = 0;
	_materiels.push_back(matY);

	tinyobj::material_t matZ{};
	matZ.diffuse[0] = 0;
	matZ.diffuse[1] = 0;
	matZ.diffuse[2] = 1;
	_materiels.push_back(matZ);
	
	_material = Material::createMaterialPerVeretxColor();
}

std::shared_ptr<Mesh> Mesh::createSphere(float diameter, size_t tessellation, bool rhcoords, bool invertn)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	std::vector<DirectX::VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indices;
	DirectX::GeometricPrimitive::CreateSphere(vertices, indices, diameter, tessellation, rhcoords, invertn);

	std::shared_ptr<Primitive> prim = std::make_shared<Primitive>();
	prim->_positionBuffer = RHI::getInst().createVertexBuffer(vertices.data(), vertices.size() * sizeof(DirectX::VertexPositionNormalTexture));
	prim->_indexBuffer = RHI::getInst().createIndexBuffer(indices.data(), indices.size() * sizeof(uint16_t));
	prim->_matIdx = 0;
	prim->_materialId = Primitive::MATERIAL_ID::MATERIAL_EMISSIVE;
	prim->_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	prim->_indicesFormat = DXGI_FORMAT_R16_UINT;
	prim->_indicesCount = indices.size();

	mesh->_primitives.push_back(prim);
	mesh->_material = Material::createMaterialEmissive();

	return mesh;
}

