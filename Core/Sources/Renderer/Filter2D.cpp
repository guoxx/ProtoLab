#include "stdafx.h"
#include "Filter2D.h"
#include <DirectXTK/Inc/VertexTypes.h>


Filter2D::Filter2D(std::shared_ptr<DX11VertexShader> vs, std::shared_ptr<DX11PixelShader> ps)
	: _vertexShader{vs}
	, _pixelShader{ps}
{
	DirectX::VertexPositionTexture vertexs[] = {
		{DirectX::XMFLOAT3{-1,  3, 0}, DirectX::XMFLOAT2{0, -1}},
		{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
		{DirectX::XMFLOAT3{ 3, -1, 0}, DirectX::XMFLOAT2{2, 1}},
	};

	_vertexBuffer = RHI::getInst().createVertexBuffer(vertexs, sizeof(vertexs));
	_inputLayout = RHI::getInst().createInputLayout(DirectX::VertexPositionTexture::InputElements, DirectX::VertexPositionTexture::InputElementCount, _vertexShader->getBinaryData());
	_textureSamp = RHI::getInst().createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
}

Filter2D::~Filter2D()
{
	RHI::getInst().destroyResource(_vertexBuffer);
	RHI::getInst().destroyInputLayout(_inputLayout);
	RHI::getInst().destroySamplerState(_textureSamp);
}

void Filter2D::apply(std::shared_ptr<DX11RenderTarget> source, std::shared_ptr<DX11RenderTarget> dest)
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();

	ID3D11RenderTargetView* rtvs[] = {dest->getRenderTarget()};
	gfxContext->OMSetRenderTargets(1, rtvs, nullptr);

	uint32_t strides[] = {sizeof(DirectX::VertexPositionTexture)};
	uint32_t offsets[] = {0};
	gfxContext->IASetVertexBuffers(0, 1, &_vertexBuffer, strides, offsets);
	gfxContext->IASetInputLayout(_inputLayout);
	gfxContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfxContext->VSSetShader(_vertexShader.get(), nullptr, 0);
	gfxContext->PSSetShader(_pixelShader.get(), nullptr, 0);
	ID3D11ShaderResourceView* textures[] = {source->getTextureSRV()};
	gfxContext->PSSetShaderResources(0, 1, textures);
	ID3D11SamplerState* samps[] = {_textureSamp};
	gfxContext->PSSetSamplers(0, 1, samps);
	gfxContext->draw(3, 0);
}
