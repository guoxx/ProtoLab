#include "stdafx.h"
#include "Mesh.h"
#include "Primitive.h"

#include "../../Shaders/PerVertexColor_vs.h"
#include "../../Shaders/PerVertexColor_ps.h"

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
	
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_vertexShader = std::make_shared<DX11VertexShader>(g_PerVertexColor_vs, sizeof(g_PerVertexColor_vs));
	_pixelShader = std::make_shared<DX11PixelShader>(g_PerVertexColor_ps, sizeof(g_PerVertexColor_ps));
	_vertexDecl = RHI::getInst().createVertexDeclaration(inputDesc, COUNT_OF_C_ARRAY(inputDesc), _vertexShader->getBinaryData());
}
