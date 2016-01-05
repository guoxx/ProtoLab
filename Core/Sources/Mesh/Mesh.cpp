#include "stdafx.h"
#include "Primitive.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "../Objects/Camera.h"

namespace PerVertexColorCB
{
#include "../../Shaders/PerVertexColor_common.h"
}


Mesh::Mesh()
{
	PerVertexColorCB::View vsConstantsData{};
	_vsConstantsBuffer = RHI::getInst().createConstantBuffer(&vsConstantsData, sizeof(vsConstantsData));

	PerVertexColorCB::Materiel psMaterielData{};
	_psMaterielBuffer = RHI::getInst().createConstantBuffer(&psMaterielData, sizeof(psMaterielData));
}

Mesh::~Mesh()
{
	RHI::getInst().destroyVertexDeclaration(_vertexDecl);
	RHI::getInst().destroyResource(_vsConstantsBuffer);
	RHI::getInst().destroyResource(_psMaterielBuffer);
}

void Mesh::draw(DirectX::CXMMATRIX mModel, const Camera* camera) const
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gfxContext->mapResource(_vsConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	PerVertexColorCB::View* dataPtr = (PerVertexColorCB::View*)mappedResource.pData;
	DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
	DirectX::XMStoreFloat4x4(&dataPtr->modelViewProjMatrix, DirectX::XMMatrixTranspose(mModelViewProj));
	gfxContext->unmapResource(_vsConstantsBuffer, 0);

	for (auto prim : _primitives)
	{
		auto mat = _materiels[prim->_matIdx];

		D3D11_MAPPED_SUBRESOURCE matSubResource;
		gfxContext->mapResource(_psMaterielBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &matSubResource);
		PerVertexColorCB::Materiel* matPtr = (PerVertexColorCB::Materiel*)matSubResource.pData;
		matPtr->ambient = DirectX::XMFLOAT4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 0);
		matPtr->diffuse = DirectX::XMFLOAT4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 0);
		matPtr->specular = DirectX::XMFLOAT4(mat.specular[0], mat.specular[1], mat.specular[2], 0);
		matPtr->transmittance = DirectX::XMFLOAT4(mat.transmittance[0], mat.transmittance[1], mat.transmittance[2], 0);
		matPtr->emission = DirectX::XMFLOAT4(mat.emission[0], mat.emission[1], mat.emission[2], 0);
		matPtr->shininess = mat.shininess;
		matPtr->ior = mat.ior;
		matPtr->dissolve = mat.dissolve;
		matPtr->illum = mat.illum;
		gfxContext->unmapResource(_psMaterielBuffer, 0);

		ID3D11Buffer* buffers[] = { prim->_positionBuffer, prim->_normalBuffer, prim->_texcoordBuffer };
		uint32_t strides[] = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT3) };
		uint32_t offsets[] = { 0, 0, 0 };

		RHI::getInst().setDefaultRHIStates();

		gfxContext->IASetInputLayout(_vertexDecl);
		gfxContext->IASetVertexBuffers(0, 3, buffers, strides, offsets);
		gfxContext->IASetIndexBuffer(prim->_indexBuffer, prim->_indicesFormat, 0);
		gfxContext->IASetPrimitiveTopology(prim->_topology);
		gfxContext->VSSetShader(_vertexShader.get(), 0, 0);
		gfxContext->VSSetConstantBuffers(0, 1, &_vsConstantsBuffer);
		gfxContext->PSSetShader(_pixelShader.get(), 0, 0);
		gfxContext->PSSetConstantBuffers(0, 1, &_psMaterielBuffer);
		gfxContext->drawIndex(prim->_indicesCount, 0, 0);
	}
}

void Mesh::loadMeshFromFile(const wchar_t* objFileName)
{
	std::string errmsg;
	char filename[1024];
	std::wcstombs(filename, objFileName, sizeof(filename));

	std::string wtlBasepath{filename};
	size_t lastSlash = wtlBasepath.rfind('\\');
	wtlBasepath = wtlBasepath.substr(0, lastSlash);
	wtlBasepath.push_back('\\');

	if (!tinyobj::LoadObj(_shapes, _materiels, errmsg, filename, wtlBasepath.c_str()))
	{
		std::cout << errmsg << std::endl;
	}

	for (auto shape : _shapes)
	{
		std::shared_ptr<Primitive> prim = std::make_shared<Primitive>();
		prim->_name = shape.name;
		prim->_matIdx = shape.mesh.material_ids[0];
		prim->_positionBuffer = RHI::getInst().createVertexBuffer(shape.mesh.positions.data(), BYTES_OF_STD_VECTOR(shape.mesh.positions));
		if (shape.mesh.normals.size() > 0)
		{
			prim->_normalBuffer = RHI::getInst().createVertexBuffer(shape.mesh.normals.data(), BYTES_OF_STD_VECTOR(shape.mesh.normals));
		}
		if (shape.mesh.texcoords.size() > 0)
		{
			prim->_texcoordBuffer = RHI::getInst().createVertexBuffer(shape.mesh.texcoords.data(), BYTES_OF_STD_VECTOR(shape.mesh.texcoords));
		}
		prim->_indexBuffer = RHI::getInst().createIndexBuffer(shape.mesh.indices.data(), BYTES_OF_STD_VECTOR(shape.mesh.indices));
		prim->_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		prim->_indicesFormat = DXGI_FORMAT_R32_UINT;
		prim->_indicesCount = shape.mesh.indices.size();

		_primitives.push_back(prim);
	}
}

void Mesh::loadShadersFromFile(const wchar_t* shaderFileName)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_vertexShader = std::make_shared<DX11VertexShader>(shaderFileName, "VSMain");
	_pixelShader = std::make_shared<DX11PixelShader>(shaderFileName, "PSMain");
	_vertexDecl = RHI::getInst().createVertexDeclaration(inputDesc, COUNT_OF_C_ARRAY(inputDesc), _vertexShader->getBinaryData());
}
