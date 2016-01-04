#include "stdafx.h"
#include "Filter2D.h"
#include <DirectXTK/Inc/VertexTypes.h>


Filter2D::Filter2D(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexDecl, ID3D11PixelShader* pixelShader)
	: _vertexShader{vertexShader}
	, _vertexDecl{vertexDecl}
	, _pixelShader{pixelShader}
{
	DirectX::VertexPositionTexture vertexs[] = {
		{DirectX::XMFLOAT3{-1, -1, 0}, DirectX::XMFLOAT2{0, 1}},
		{DirectX::XMFLOAT3{-1,  3, 0}, DirectX::XMFLOAT2{0, -1}},
		{DirectX::XMFLOAT3{ 3, -1, 0}, DirectX::XMFLOAT2{2, 1}},
	};

	_vertexBuffer = RHI::getInst().createVertexBuffer(vertexs, sizeof(vertexs));
}

Filter2D::~Filter2D()
{
	RHI::getInst().destroyVertexShader(_vertexShader);
	RHI::getInst().destroyVertexDeclaration(_vertexDecl);
	RHI::getInst().destroyPixelShader(_pixelShader);
}

void Filter2D::apply()
{
	std::shared_ptr<DX11GraphicContext> gfxContext = RHI::getInst().getContext();
	gfxContext->IASetVertexBuffers(0, 1, &_vertexBuffer, nullptr, nullptr);
	gfxContext->IASetInputLayout(_vertexDecl);
	gfxContext->draw(3, 0);
}
