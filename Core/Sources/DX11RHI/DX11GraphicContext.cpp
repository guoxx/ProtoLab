#include "stdafx.h"
#include "DX11GraphicContext.h"


DX11GraphicContext::DX11GraphicContext(ID3D11DeviceContext * ctx)
	: _context{ctx}
{
}

DX11GraphicContext::~DX11GraphicContext()
{
	RHI::getInst().destroyDeviceContext(_context);
}

void DX11GraphicContext::clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a)
{
	const float clearColor[4] = { r, g, b, a };
	_context->ClearRenderTargetView(rtv, clearColor);
}

void DX11GraphicContext::clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth, uint8_t stencil)
{
	_context->ClearDepthStencilView(dsv, static_cast<uint32_t>(clearFlag), depth, stencil);
}

void DX11GraphicContext::drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation)
{
	_context->DrawIndexed(indexCount, startIndexLoccation, baseVertexLocation);
}
