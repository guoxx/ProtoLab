#include "stdafx.h"
#include "Material.h"
#include "../DX11RHI/DX11GraphicContext.h"

#include "../../Shaders/BaseMaterial_vs.h"
#include "../../Shaders/BaseMaterial_ps.h"
#include "../../Shaders/PerVertexColor_vs.h"
#include "../../Shaders/PerVertexColor_ps.h"


std::shared_ptr<Material> Material::createMaterialBase()
{
	std::shared_ptr<Material> mat = std::make_shared<Material>();

	mat->createVsConstantsBuffer(nullptr, sizeof(MaterialCB::BaseMaterial::View), MaterialCB::BaseMaterial::ViewReg);
	mat->createPsConstantsBuffer(nullptr, sizeof(MaterialCB::BaseMaterial::Material), MaterialCB::BaseMaterial::MaterialReg);
	mat->createPsConstantsBuffer(nullptr, sizeof(MaterialCB::BaseMaterial::PointLight), MaterialCB::BaseMaterial::PointLightReg);

	auto vertShader = std::make_shared<DX11VertexShader>(g_BaseMaterial_vs, sizeof(g_BaseMaterial_vs));
	auto fragShader = std::make_shared<DX11PixelShader>(g_BaseMaterial_ps, sizeof(g_BaseMaterial_ps));

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	mat->initialize(vertShader, fragShader, inputDesc, COUNT_OF_C_ARRAY(inputDesc));
	return mat;
}

std::shared_ptr<Material> Material::createMaterialPerVeretxColor()
{
	std::shared_ptr<Material> mat = std::make_shared<Material>();

	mat->createVsConstantsBuffer(nullptr, sizeof(MaterialCB::PerVertexColor::View), MaterialCB::PerVertexColor::ViewReg);
	mat->createPsConstantsBuffer(nullptr, sizeof(MaterialCB::PerVertexColor::Material), MaterialCB::PerVertexColor::MaterialReg);

	auto vertShader = std::make_shared<DX11VertexShader>(g_PerVertexColor_vs, sizeof(g_PerVertexColor_vs));
	auto fragShader = std::make_shared<DX11PixelShader>(g_PerVertexColor_ps, sizeof(g_PerVertexColor_ps));

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	mat->initialize(vertShader, fragShader, inputDesc, COUNT_OF_C_ARRAY(inputDesc));
	return mat;
}

//std::shared_ptr<Material> Material::createMaterialUnlit()
//{
//}

Material::Material()
{
}

Material::~Material()
{
	RHI::getInst().destroyVertexDeclaration(_vertDecl);
	for (auto desc : _vsConstantBuffers)
	{
		if (desc._used)
		{
			RHI::getInst().destroyResource(desc._buffer);
		}
	}
	for (auto desc : _psConstantBuffers)
	{
		if (desc._used)
		{
			RHI::getInst().destroyResource(desc._buffer);
		}
	}
}

void Material::initialize(std::shared_ptr<DX11VertexShader> vertShader, std::shared_ptr<DX11PixelShader> fragShader, const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt)
{
	_vertShader = vertShader;
	_fragShader = fragShader;
	_vertDecl = RHI::getInst().createVertexDeclaration(desc, descElemCnt, _vertShader->getBinaryData());
}

void Material::createVsConstantsBuffer(const void* memPtr, uint32_t memSize, uint32_t reg)
{
	ID3D11Buffer* buffer = RHI::getInst().createConstantBuffer(memPtr, memSize);
	ConstantBufferDesc& desc = _vsConstantBuffers[reg];
	CHECK(!desc._used);
	desc._used = true;	
	desc._buffer = buffer;
}

void Material::createPsConstantsBuffer(const void* memPtr, uint32_t memSize, uint32_t reg)
{
	ID3D11Buffer* buffer = RHI::getInst().createConstantBuffer(memPtr, memSize);
	ConstantBufferDesc& desc = _psConstantBuffers[reg];
	CHECK(!desc._used);
	desc._used = true;	
	desc._buffer = buffer;
}

ID3D11Buffer* Material::getVsConstantBuffer(uint32_t reg)
{
	ConstantBufferDesc& desc = _vsConstantBuffers[reg];
	CHECK(desc._used);
	return desc._buffer;
}

ID3D11Buffer* Material::getPsConstantBuffer(uint32_t reg)
{
	ConstantBufferDesc& desc = _psConstantBuffers[reg];
	CHECK(desc._used);
	return desc._buffer;
}

void Material::setVertexBuffer(VEX_INPUT_SLOT slot, ID3D11Buffer* buffer, uint32_t stride, uint32_t offset)
{
	VertBufferDesc& desc = _vertexBuffers[slot];
	CHECK(!desc._used);
	desc._buffer = buffer;
	desc._stride = stride;
	desc._offset = offset;
	desc._used = true;
	_vertexBuffers[slot] = desc;
}

void Material::unsetVertexBuffers()
{
	for(VertBufferDesc& desc : _vertexBuffers)
	{
		desc._used = false;
	}
}

void Material::apply(std::shared_ptr<DX11GraphicContext> gfxContext)
{
	// TODO: more runtime checks
	uint32_t numBuffers = 0;
	ID3D11Buffer* buffers[VEX_INPUT_SLOT_NUM];
	uint32_t strides[VEX_INPUT_SLOT_NUM];
	uint32_t offsets[VEX_INPUT_SLOT_NUM];
	for (auto desc : _vertexBuffers)
	{
		if (desc._used)
		{
			uint32_t idxBuffers = numBuffers;
			numBuffers += 1;

			buffers[idxBuffers] = desc._buffer;
			strides[idxBuffers] = desc._stride;
			offsets[idxBuffers] = desc._offset;
		}
	}

	// IA
	gfxContext->IASetVertexBuffers(0, numBuffers, buffers, strides, offsets);
	gfxContext->IASetInputLayout(_vertDecl);

	// VS
	gfxContext->VSSetShader(_vertShader.get(), nullptr, 0);
	for (uint32_t i = 0; i < _vsConstantBuffers.size(); ++i)
	{
		ConstantBufferDesc& desc = _vsConstantBuffers[i];
		if (desc._used)
		{
			gfxContext->VSSetConstantBuffers(i, 1, &desc._buffer);
		}
	}

	// PS
	gfxContext->PSSetShader(_fragShader.get(), nullptr, 0);
	for (uint32_t i = 0; i < _vsConstantBuffers.size(); ++i)
	{
		ConstantBufferDesc& desc = _psConstantBuffers[i];
		if (desc._used)
		{
			gfxContext->PSSetConstantBuffers(i, 1, &desc._buffer);
		}
	}

	// clean 
	unsetVertexBuffers();
}
