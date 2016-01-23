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

	_vertexBuffer = RHI::getInstance().getDevice()->createVertexBuffer(vertexs, sizeof(vertexs));
	_inputLayout = RHI::getInstance().getDevice()->createInputLayout(DirectX::VertexPositionTexture::InputElements, DirectX::VertexPositionTexture::InputElementCount, _vertexShader->getBinaryData());
	_textureSamp = RHI::getInstance().getDevice()->createSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
}

Filter2D::~Filter2D()
{
}

void Filter2D::apply(std::shared_ptr<DX11RenderTarget> source, std::shared_ptr<DX11RenderTarget> dest)
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInstance().getContext();

	ID3D11RenderTargetView* rtvs[] = {dest->getRenderTarget().Get()};
	gfxContext->OMSetRenderTargets(1, rtvs, nullptr);

	ID3D11Buffer* vbuffers[] = {_vertexBuffer.Get()};
	uint32_t strides[] = {sizeof(DirectX::VertexPositionTexture)};
	uint32_t offsets[] = {0};
	gfxContext->IASetVertexBuffers(0, 1, vbuffers, strides, offsets);
	gfxContext->IASetInputLayout(_inputLayout.Get());
	gfxContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfxContext->VSSetShader(_vertexShader.get(), nullptr, 0);
	gfxContext->PSSetShader(_pixelShader.get(), nullptr, 0);
	ID3D11ShaderResourceView* textures[] = {source->getTextureSRV().Get()};
	gfxContext->PSSetShaderResources(0, 1, textures);
	ID3D11SamplerState* samps[] = {_textureSamp.Get()};
	gfxContext->PSSetSamplers(0, 1, samps);
	gfxContext->draw(3, 0);
}
