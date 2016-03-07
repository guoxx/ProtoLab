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
	_shadowMapMaterial = Material::createMaterialShadowMap();
}

Mesh::~Mesh()
{
}

std::shared_ptr<Material> Mesh::getMaterial() const
{
	return _material;
}

void Mesh::draw(DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera, std::shared_ptr<PointLight> pointLight) const
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInstance().getContext();

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

void Mesh::drawShadowMap(DirectX::XMMATRIX mModel, DirectX::XMMATRIX mViewProj[6]) const
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInstance().getContext();

	for (auto prim : _primitives)
	{
		_drawShadowMap(gfxContext, prim, mModel, mViewProj);
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
		prim->_positionBuffer = RHI::getInstance().getDevice()->createVertexBuffer(shape.mesh.positions.data(), uint32_t(shape.mesh.positions.size()*sizeof(float)));
		if (shape.mesh.normals.size() > 0)
		{
			prim->_normalBuffer = RHI::getInstance().getDevice()->createVertexBuffer(shape.mesh.normals.data(), uint32_t(shape.mesh.normals.size()*sizeof(float)));
		}
		if (shape.mesh.texcoords.size() > 0)
		{
			prim->_texcoordBuffer = RHI::getInstance().getDevice()->createVertexBuffer(shape.mesh.texcoords.data(), uint32_t(shape.mesh.texcoords.size()*sizeof(float)));
		}
		prim->_indexBuffer = RHI::getInstance().getDevice()->createIndexBuffer(shape.mesh.indices.data(), uint32_t(shape.mesh.indices.size()*sizeof(unsigned int)));
		prim->_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		prim->_indicesFormat = DXGI_FORMAT_R32_UINT;
		prim->_indicesCount = static_cast<uint32_t>(shape.mesh.indices.size());

		_primitives.push_back(prim);
	}

	_material = Material::createMaterialBase();
}

void Mesh::_drawBaseMaterial(DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera, std::shared_ptr<PointLight> pointLight, std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim) const
{
	auto mat = _materiels[prim->_matIdx];

	{
		// update view constant buffer
		ComPtr<ID3D11Buffer> viewCB = _material->getVsConstantBuffer(MaterialCB::BaseMaterial::ViewReg);
		DX11ResourceMapGuard viewRes{ gfxContext.get(), viewCB.Get() };
		MaterialCB::BaseMaterial::View* dataPtr = viewRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMMATRIX mModelViewInv = DirectX::XMMatrixInverse(nullptr, mModelView);
		DirectX::XMMATRIX mModelViewInvTrans = DirectX::XMMatrixTranspose(mModelViewInv);
		DirectX::XMStoreFloat4x4(&dataPtr->mModelView, DirectX::XMMatrixTranspose(mModelView));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewProj, DirectX::XMMatrixTranspose(mModelViewProj));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewInvTrans, DirectX::XMMatrixTranspose(mModelViewInvTrans));
	}

	{
		// update material constant buffer
		ComPtr<ID3D11Buffer> materialCB = _material->getPsConstantBuffer(MaterialCB::BaseMaterial::MaterialReg);
		DX11ResourceMapGuard materialRes{ gfxContext.get(), materialCB.Get() };
		MaterialCB::BaseMaterial::Material* matPtr = materialRes.getPtr<std::remove_pointer_t<decltype(matPtr)>>();
		matPtr->ambient = DirectX::XMFLOAT4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 0);
		matPtr->diffuse = DirectX::XMFLOAT4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 0);
		matPtr->specular = DirectX::XMFLOAT4(mat.specular[0], mat.specular[1], mat.specular[2], 0);
		matPtr->transmittance = DirectX::XMFLOAT4(mat.transmittance[0], mat.transmittance[1], mat.transmittance[2], 0);
		matPtr->emission = DirectX::XMFLOAT4(mat.emission[0], mat.emission[1], mat.emission[2], 0);
		matPtr->shininess = mat.shininess;
		matPtr->ior = mat.ior;
		matPtr->dissolve = mat.dissolve;
		matPtr->illum = mat.illum;
	}

	{
		// update point light constant buffer
		ComPtr<ID3D11Buffer> pointLightCB = _material->getPsConstantBuffer(MaterialCB::BaseMaterial::PointLightReg);
		DX11ResourceMapGuard pointLightRes{ gfxContext.get(), pointLightCB.Get() };
		MaterialCB::BaseMaterial::PointLight* dataPtr = pointLightRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMVECTOR lightPositionInLocalSpace = pointLight->getPosition();
		lightPositionInLocalSpace = DirectX::XMVectorSetW(lightPositionInLocalSpace, 1);
		DirectX::XMVECTOR lightPositionInCameraSpace = DirectX::XMVector4Transform(lightPositionInLocalSpace, mModelView);
		DirectX::XMStoreFloat4(&dataPtr->lightPositionInCameraSpace, lightPositionInCameraSpace);
		DirectX::XMFLOAT3 intensity = pointLight->getIntensity();
		dataPtr->intensity = DirectX::XMFLOAT4{intensity.x, intensity.y, intensity.z, 0};
		dataPtr->radiusStart = pointLight->getRadiusStart();
		dataPtr->radiusEnd = pointLight->getRadiusEnd();
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::NORMAL, prim->_normalBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::TEXCOORD0, prim->_texcoordBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer.Get(), prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);
}

void Mesh::_drawPerVertexColor(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera) const
{
	auto mat = _materiels[prim->_matIdx];

	{
		// update view constant buffer
		ComPtr<ID3D11Buffer> viewCB = _material->getVsConstantBuffer(MaterialCB::PerVertexColor::ViewReg);
		DX11ResourceMapGuard viewRes{ gfxContext.get(), viewCB.Get() };
		MaterialCB::PerVertexColor::View* dataPtr = viewRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMStoreFloat4x4(&dataPtr->modelViewProjMatrix, DirectX::XMMatrixTranspose(mModelViewProj));
	}

	{
		// update material constant buffer
		ComPtr<ID3D11Buffer> materialCB = _material->getPsConstantBuffer(MaterialCB::PerVertexColor::MaterialReg);
		DX11ResourceMapGuard materialRes{ gfxContext.get(), materialCB.Get() };
		MaterialCB::PerVertexColor::Material* matPtr = materialRes.getPtr<std::remove_pointer_t<decltype(matPtr)>>();
		matPtr->ambient = DirectX::XMFLOAT4(mat.ambient[0], mat.ambient[1], mat.ambient[2], 0);
		matPtr->diffuse = DirectX::XMFLOAT4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 0);
		matPtr->specular = DirectX::XMFLOAT4(mat.specular[0], mat.specular[1], mat.specular[2], 0);
		matPtr->transmittance = DirectX::XMFLOAT4(mat.transmittance[0], mat.transmittance[1], mat.transmittance[2], 0);
		matPtr->emission = DirectX::XMFLOAT4(mat.emission[0], mat.emission[1], mat.emission[2], 0);
		matPtr->shininess = mat.shininess;
		matPtr->ior = mat.ior;
		matPtr->dissolve = mat.dissolve;
		matPtr->illum = mat.illum;
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::NORMAL, prim->_normalBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::TEXCOORD0, prim->_texcoordBuffer.Get(), sizeof(DirectX::XMFLOAT3), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer.Get(), prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);
}

void Mesh::_drawEmissive(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::CXMMATRIX mModel, std::shared_ptr<Camera> camera) const
{
	gfxContext->OMSetBlendState(RHI::getInstance().getRenderStateSet()->Additive());

	{
		// update view constant buffer
		ComPtr<ID3D11Buffer> viewCB = _material->getVsConstantBuffer(MaterialCB::EmissiveMaterial::ViewReg);
		DX11ResourceMapGuard viewRes{ gfxContext.get(), viewCB.Get() };
		MaterialCB::EmissiveMaterial::View* dataPtr = viewRes.getPtr<std::remove_pointer_t<decltype(dataPtr)>>();
		DirectX::XMMATRIX mModelView = DirectX::XMMatrixMultiply(mModel, camera->getViewMatrix());
		DirectX::XMMATRIX mModelViewProj = DirectX::XMMatrixMultiply(mModel, camera->getViewProjectionMatrix());
		DirectX::XMStoreFloat4x4(&dataPtr->mModelView, DirectX::XMMatrixTranspose(mModelView));
		DirectX::XMStoreFloat4x4(&dataPtr->mModelViewProj, DirectX::XMMatrixTranspose(mModelViewProj));
	}

	_material->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer.Get(), sizeof(DirectX::VertexPositionNormalTexture), 0);
	_material->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer.Get(), prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);

	gfxContext->OMSetBlendState(RHI::getInstance().getRenderStateSet()->Opaque());
}

void Mesh::_drawShadowMap(std::shared_ptr<DX11GraphicContext> gfxContext, std::shared_ptr<Primitive> prim, DirectX::XMMATRIX mModel, DirectX::XMMATRIX mViewProj[6]) const
{
	gfxContext->OMSetBlendState(RHI::getInstance().getRenderStateSet()->Opaque());

	ComPtr<ID3D11Buffer> viewCB = _shadowMapMaterial->getVsConstantBuffer(MaterialCB::ShadowMapMaterial::ViewReg);
	DX11ResourceMapGuard viewRes{ gfxContext.get(), viewCB.Get() };
	MaterialCB::ShadowMapMaterial::View* dataPtr;
	viewRes.getPtr(dataPtr);
	DirectX::XMStoreFloat4x4(&dataPtr->mModel, DirectX::XMMatrixTranspose(mModel));
	//for (int i = 0; i < 6; ++i)
	//{
	//	DirectX::XMStoreFloat4x4(&dataPtr->mViewProj[i], DirectX::XMMatrixTranspose(mViewProj[i]));
	//}

	_shadowMapMaterial->setVertexBuffer(Material::VEX_INPUT_SLOT::POSITION, prim->_positionBuffer.Get(), sizeof(float3), 0);
	_shadowMapMaterial->apply(gfxContext);

	gfxContext->IASetIndexBuffer(prim->_indexBuffer.Get(), prim->_indicesFormat, 0);
	gfxContext->IASetPrimitiveTopology(prim->_topology);
	gfxContext->drawIndex(prim->_indicesCount, 0, 0);

}
