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

void DX11GraphicContext::IASetInputLayout(ID3D11InputLayout * vertexDecl)
{
	_context->IASetInputLayout(vertexDecl);
}

void DX11GraphicContext::IASetVertexBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppVertexBuffers, const uint32_t * pStrides, const uint32_t * pOffsets)
{
	_context->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void DX11GraphicContext::IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, uint32_t offset)
{
	_context->IASetIndexBuffer(pIndexBuffer, format, offset);
}

void DX11GraphicContext::IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	_context->IASetPrimitiveTopology(topology);
}

void DX11GraphicContext::VSSetShader(DX11VertexShader* pVertexShader, ID3D11ClassInstance * const * ppClassInstances, uint32_t numClassInstances)
{
	_context->VSSetShader(pVertexShader->getShader(), ppClassInstances, numClassInstances);
}

void DX11GraphicContext::VSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppConstantBuffers)
{
	_context->VSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
}

void DX11GraphicContext::VSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView * const * ppShaderResourceViews)
{
	_context->VSSetShaderResources(startSlot, numViews, ppShaderResourceViews);
}

void DX11GraphicContext::RSSetState(ID3D11RasterizerState * pRasterizerState)
{
	_context->RSSetState(pRasterizerState);
}

void DX11GraphicContext::RSSetViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minDepth, float maxDepth)
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = static_cast<float>(topLeftX);
	vp.TopLeftY = static_cast<float>(topLeftY);
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MaxDepth = maxDepth;
	vp.MinDepth = minDepth;
	RSSetViewports(1, &vp);
}

void DX11GraphicContext::RSSetViewports(uint32_t numViewports, const D3D11_VIEWPORT * pViewports)
{
	_context->RSSetViewports(numViewports, pViewports);
}

void DX11GraphicContext::PSSetShader(DX11PixelShader* pPixelShader, ID3D11ClassInstance * const * ppClassInstances, uint32_t numClassInstances)
{
	_context->PSSetShader(pPixelShader->getShader(), ppClassInstances, numClassInstances);
}

void DX11GraphicContext::PSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppConstantBuffers)
{
	_context->PSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
}

void DX11GraphicContext::PSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView * const * ppShaderResourceViews)
{
	_context->PSSetShaderResources(startSlot, numViews, ppShaderResourceViews);
}

void DX11GraphicContext::OMSetDepthStencilState(ID3D11DepthStencilState * pDepthStencilState, uint32_t stencilRef)
{
	_context->OMSetDepthStencilState(pDepthStencilState, stencilRef);
}

void DX11GraphicContext::OMSetRenderTargets(uint32_t numViews, ID3D11RenderTargetView * const * ppRenderTargetViews, ID3D11DepthStencilView * pDepthStencilView)
{
	_context->OMSetRenderTargets(numViews, ppRenderTargetViews, pDepthStencilView);
}

void DX11GraphicContext::mapResource(ID3D11Resource* resource, uint32_t subresource, D3D11_MAP mapType, uint32_t mapFlags, D3D11_MAPPED_SUBRESOURCE* mappedResource)
{
	_context->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, mappedResource);
}

void DX11GraphicContext::unmapResource(ID3D11Resource * resource, uint32_t subresource)
{
	_context->Unmap(resource, 0);
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

void DX11GraphicContext::draw(uint32_t vertexCount, uint32_t startVertexLocation)
{
	_context->Draw(vertexCount, startVertexLocation);
}

void DX11GraphicContext::drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation)
{
	_context->DrawIndexed(indexCount, startIndexLoccation, baseVertexLocation);
}

HRESULT DX11GraphicContext::finishCommandList(bool restoreDeferredContextState, ID3D11CommandList ** ppCommandList)
{
	return _context->FinishCommandList(restoreDeferredContextState, ppCommandList);
}

void DX11GraphicContext::executeCommandList(ID3D11CommandList * pCommandList, bool restoreContextState)
{
	_context->ExecuteCommandList(pCommandList, restoreContextState);
}
