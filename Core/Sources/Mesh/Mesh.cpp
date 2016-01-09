#include "stdafx.h"
#include "Primitive.h"
#include "Material.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "../Objects/Camera.h"
#include "../Objects/PointLight.h"
#include <DirectXTK/Inc/GeometricPrimitive.h>


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::draw(DirectX::CXMMATRIX mModel, const Camera* camera, std::shared_ptr<PointLight> pointLight) const
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();

	for (auto prim : _primitives)
	{
		if (prim->_materialId == Primitive::MATERIAL_ID::MATERIAL_VERTEX_COLOR)
		{
			_drawPerVertexColor(gfxContext, prim, mModel, camera);
		}
		else if (prim->_materialId == Primitive::MATERIAL_ID::MATERIAL_DEFAULT)
		{
			_drawBaseMaterial(mModel, camera, pointLight, gfxContext, prim);
		}
		else if (prim->_materialId == Primitive::MATERIAL_ID::MATERIAL_EMISSIVE)
		{
			_drawEmissive(gfxContext, prim, mModel, camera);
		}
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
		prim->_positionBuffer = RHI::getInst().createVertexBuffer(shape.mesh.positions.data(), uint32_t(shape.mesh.positions.size()*sizeof(float)));
		if (shape.mesh.normals.size() > 0)
		{
			prim->_normalBuffer = RHI::getInst().createVertexBuffer(shape.mesh.normals.data(), uint32_t(shape.mesh.normals.size()*sizeof(float)));
		}
		if (shape.mesh.texcoords.size() > 0)
		{
			prim->_texcoordBuffer = RHI::getInst().createVertexBuffer(shape.mesh.texcoords.data(), uint32_t(shape.mesh.texcoords.size()*sizeof(float)));
		}
		prim->_indexBuffer = RHI::getInst().createIndexBuffer(shape.mesh.indices.data(), uint32_t(shape.mesh.indices.size()*sizeof(unsigned int)));
		prim->_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		prim->_indicesFormat = DXGI_FORMAT_R32_UINT;
		prim->_indicesCount = static_cast<uint32_t>(shape.mesh.indices.size());

		_primitives.push_back(prim);
	}

	_material = Material::createMaterialBase();
}

void Mesh::_drawBaseMaterial(DirectX::CXMMATRIX mModel, const Camera* camera, std::shared_ptr<PointLight> pointLight, std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim) const
{
	auto mat = _materiels[prim->_matIdx];

	{
		// update view constant buffer
		ID3D11Buffer* viewCB = _material->getVsConstantBuffer(MaterialCB::BaseMaterial::ViewReg);
		D3D11_MAPPED_SUBRESOURCE viewRes;
		gfxContext->mapResource(viewCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &viewRes);
		MaterialCB::BaseMaterial::View* dataPtr = (MaterialCB::BaseMaterial::View*)viewRes.pData;
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMMATRIX mModelViewInv = DirectX::XMMatrixInverse(nullptr, mModelView);
		DirectX::XMMATRIX mModelViewInvTrans = DirectX::XMMatrixTranspose(mModelViewInv);
		DirectX::XMStoreFloat4x4(&dataPtr->mModelView, DirectX::XMMatrixTranspose(mModelView));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewProj, DirectX::XMMatrixTranspose(mModelViewProj));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewInvTrans, DirectX::XMMatrixTranspose(mModelViewInvTrans));
		gfxContext->unmapResource(viewCB, 0);
	}

	{
		ID3D11Buffer* materialCB = _material->getPsConstantBuffer(MaterialCB::BaseMaterial::MaterialReg);
		// update material constant buffer
		D3D11_MAPPED_SUBRESOURCE materialRes;
		gfxContext->mapResource(materialCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &materialRes);
		MaterialCB::BaseMaterial::Material* matPtr = (MaterialCB::BaseMaterial::Material*)materialRes.pData;
		matPtr->ambient = DirectX::XMFLOAT4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 0);
		matPtr->diffuse = DirectX::XMFLOAT4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 0);
		matPtr->specular = DirectX::XMFLOAT4(mat.specular[0], mat.specular[1], mat.specular[2], 0);
		matPtr->transmittance = DirectX::XMFLOAT4(mat.transmittance[0], mat.transmittance[1], mat.transmittance[2], 0);
		matPtr->emission = DirectX::XMFLOAT4(mat.emission[0], mat.emission[1], mat.emission[2], 0);
		matPtr->shininess = mat.shininess;
		matPtr->ior = mat.ior;
		matPtr->dissolve = mat.dissolve;
		matPtr->illum = mat.illum;
		gfxContext->unmapResource(materialCB, 0);
	}

	{
		ID3D11Buffer* pointLightCB = _material->getPsConstantBuffer(MaterialCB::BaseMaterial::PointLightReg);
		// update point light constant buffer
		D3D11_MAPPED_SUBRESOURCE pointLightRes;
		gfxContext->mapResource(pointLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pointLightRes);
		MaterialCB::BaseMaterial::PointLight* dataPtr = (MaterialCB::BaseMaterial::PointLight*)pointLightRes.pData;
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMVECTOR lightPositionInLocalSpace = pointLight->getPosition();
		lightPositionInLocalSpace = DirectX::XMVectorSetW(lightPositionInLocalSpace, 1);
		DirectX::XMVECTOR lightPositionInCameraSpace = DirectX::XMVector4Transform(lightPositionInLocalSpace, mModelView);
		DirectX::XMStoreFloat4(&dataPtr->lightPositionInCameraSpace, lightPositionInCameraSpace);
		DirectX::XMFLOAT3 intensity = pointLight->getIntensity();
		dataPtr->intensity = DirectX::XMFLOAT4{intensity.x, intensity.y, intensity.z, 0};
		dataPtr->radiusStart = pointLight->getRadiusStart();
		dataPtr->radiusEnd = pointLight->getRadiusEnd();
		gfxContext->unmapResource(pointLightCB, 0);
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::NORMAL, prim->_normalBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::TEXCOORD0, prim->_texcoordBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer, prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);
}

void Mesh::_drawPerVertexColor(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, const Camera* camera) const
{
	auto mat = _materiels[prim->_matIdx];

	{
		// update view constant buffer
		ID3D11Buffer* viewCB = _material->getVsConstantBuffer(MaterialCB::PerVertexColor::ViewReg);
		D3D11_MAPPED_SUBRESOURCE viewRes;
		gfxContext->mapResource(viewCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &viewRes);
		MaterialCB::PerVertexColor::View* dataPtr = (MaterialCB::PerVertexColor::View*)viewRes.pData;
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMStoreFloat4x4(&dataPtr->modelViewProjMatrix, DirectX::XMMatrixTranspose(mModelViewProj));
		gfxContext->unmapResource(viewCB, 0);
	}

	{
		// update material constant buffer
		ID3D11Buffer* materialCB = _material->getPsConstantBuffer(MaterialCB::PerVertexColor::MaterialReg);
		D3D11_MAPPED_SUBRESOURCE matSubResource;
		gfxContext->mapResource(materialCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &matSubResource);
		MaterialCB::PerVertexColor::Material* matPtr = (MaterialCB::PerVertexColor::Material*)matSubResource.pData;
		matPtr->ambient = DirectX::XMFLOAT4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 0);
		matPtr->diffuse = DirectX::XMFLOAT4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 0);
		matPtr->specular = DirectX::XMFLOAT4(mat.specular[0], mat.specular[1], mat.specular[2], 0);
		matPtr->transmittance = DirectX::XMFLOAT4(mat.transmittance[0], mat.transmittance[1], mat.transmittance[2], 0);
		matPtr->emission = DirectX::XMFLOAT4(mat.emission[0], mat.emission[1], mat.emission[2], 0);
		matPtr->shininess = mat.shininess;
		matPtr->ior = mat.ior;
		matPtr->dissolve = mat.dissolve;
		matPtr->illum = mat.illum;
		gfxContext->unmapResource(materialCB, 0);
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::NORMAL, prim->_normalBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::TEXCOORD0, prim->_texcoordBuffer, sizeof(DirectX::XMFLOAT3), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer, prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);
}

void Mesh::_drawEmissive(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, const Camera* camera) const
{
	gfxContext->OMSetBlendState(RHI::getInst().getRenderStateSet()->Additive());

	{
		// update view constant buffer
		ID3D11Buffer* viewCB = _material->getVsConstantBuffer(MaterialCB::EmissiveMaterial::ViewReg);
		D3D11_MAPPED_SUBRESOURCE viewRes;
		gfxContext->mapResource(viewCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &viewRes);
		MaterialCB::EmissiveMaterial::View* dataPtr = (MaterialCB::EmissiveMaterial::View*)viewRes.pData;
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMStoreFloat4x4(&dataPtr->mModelView, DirectX::XMMatrixTranspose(mModelView));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewProj, DirectX::XMMatrixTranspose(mModelViewProj));
		gfxContext->unmapResource(viewCB, 0);
	}

	{
		ID3D11Buffer* pointLightCB = _material->getPsConstantBuffer(MaterialCB::EmissiveMaterial::PointLightReg);
		// update point light constant buffer
		D3D11_MAPPED_SUBRESOURCE pointLightRes;
		gfxContext->mapResource(pointLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &pointLightRes);
		MaterialCB::EmissiveMaterial::PointLight* dataPtr = (MaterialCB::EmissiveMaterial::PointLight*)pointLightRes.pData;
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMVECTOR lightPositionInLocalSpace = hackPointLight->getPosition();
		lightPositionInLocalSpace = DirectX::XMVectorSetW(lightPositionInLocalSpace, 1);
		DirectX::XMVECTOR lightPositionInCameraSpace = DirectX::XMVector4Transform(lightPositionInLocalSpace, mModelView);
		DirectX::XMStoreFloat4(&dataPtr->lightPositionInCameraSpace, lightPositionInCameraSpace);
		DirectX::XMFLOAT3 intensity = hackPointLight->getIntensity();
		dataPtr->intensity = DirectX::XMFLOAT4{ intensity.x, intensity.y, intensity.z, 0 };
		dataPtr->radiusStart = hackPointLight->getRadiusStart();
		dataPtr->radiusEnd = hackPointLight->getRadiusEnd();
		gfxContext->unmapResource(pointLightCB, 0);
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer, sizeof(DirectX::VertexPositionNormalTexture), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer, prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);

	gfxContext->OMSetBlendState(RHI::getInst().getRenderStateSet()->Opaque());
}
