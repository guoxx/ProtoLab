#include "stdafx.h"
#include "DX11GraphicContext.h"


DX11GraphicContext::DX11GraphicContext(ComPtr<ID3D11DeviceContext> ctx)
	: _context{ctx}
{
	_context.As(&_perf);
}

DX11GraphicContext::~DX11GraphicContext()
{
}

void DX11GraphicContext::resetDefaultRenderStates(std::shared_ptr<DX11RenderStateSet> stateSet)
{
	_context->RSSetState(stateSet->CullClockwise());
	_context->OMSetDepthStencilState(stateSet->DepthDefault(), 0);
}

void DX11GraphicContext::IASetInputLayout(ID3D11InputLayout* inputLayout)
{
	_context->IASetInputLayout(inputLayout);
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
	_context->VSSetShader(pVertexShader->getShader().Get(), ppClassInstances, numClassInstances);
}

void DX11GraphicContext::VSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppConstantBuffers)
{
	_context->VSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
}

void DX11GraphicContext::VSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView * const * ppShaderResourceViews)
{
	_context->VSSetShaderResources(startSlot, numViews, ppShaderResourceViews);
}

void DX11GraphicContext::GSSetShader(DX11GeometryShader * pGeomShader, ID3D11ClassInstance * const * ppClassInstances, uint32_t numClassInstances)
{
	_context->GSSetShader(pGeomShader->getShader().Get(), ppClassInstances, numClassInstances);
}

void DX11GraphicContext::GSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppConstantBuffers)
{
	_context->GSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
}

void DX11GraphicContext::GSUnsetShader()
{
	_context->GSSetShader(nullptr, nullptr, 0);
}

void DX11GraphicContext::SOSetTargets(uint32_t numBuffers, ID3D11Buffer * const * ppSOTargets, const uint32_t * pOffsets)
{
	_context->SOSetTargets(numBuffers, ppSOTargets, pOffsets);
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
	_context->PSSetShader(pPixelShader->getShader().Get(), ppClassInstances, numClassInstances);
}

void DX11GraphicContext::PSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer * const * ppConstantBuffers)
{
	_context->PSSetConstantBuffers(startSlot, numBuffers, ppConstantBuffers);
}

void DX11GraphicContext::PSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView * const * ppShaderResourceViews)
{
	_context->PSSetShaderResources(startSlot, numViews, ppShaderResourceViews);
}

void DX11GraphicContext::PSSetSamplers(uint32_t startSlot, uint32_t numSamplers, ID3D11SamplerState *const *ppSamplers)
{
	_context->PSSetSamplers(startSlot, numSamplers, ppSamplers);
}

void DX11GraphicContext::OMSetDepthStencilState(ID3D11DepthStencilState * pDepthStencilState, uint32_t stencilRef)
{
	_context->OMSetDepthStencilState(pDepthStencilState, stencilRef);
}

void DX11GraphicContext::OMSetBlendState(ID3D11BlendState * pBlendState)
{
	static const float blendFactor[4] = {1, 1, 1, 1};
	OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
}

void DX11GraphicContext::OMSetBlendState(ID3D11BlendState * pBlendState, const float blendFactor[4], uint32_t sampleMask)
{
	_context->OMSetBlendState(pBlendState, blendFactor, sampleMask);
}

void DX11GraphicContext::OMSetRenderTargets(uint32_t numViews, ID3D11RenderTargetView * const * ppRenderTargetViews, ID3D11DepthStencilView * pDepthStencilView)
{
	_context->OMSetRenderTargets(numViews, ppRenderTargetViews, pDepthStencilView);
}

void DX11GraphicContext::mapResource(ID3D11Resource* resource, uint32_t subresource, D3D11_MAP mapType, uint32_t mapFlags, D3D11_MAPPED_SUBRESOURCE* mappedResource)
{
	_context->Map(resource, subresource, mapType, mapFlags, mappedResource);
}

void DX11GraphicContext::unmapResource(ID3D11Resource * resource, uint32_t subresource)
{
	_context->Unmap(resource, subresource);
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

void DX11GraphicContext::drawAuto()
{
	_context->DrawAuto();
}

HRESULT DX11GraphicContext::finishCommandList(bool restoreDeferredContextState, ID3D11CommandList ** ppCommandList)
{
	return _context->FinishCommandList(restoreDeferredContextState, ppCommandList);
}

void DX11GraphicContext::executeCommandList(ComPtr<ID3D11CommandList> pCommandList, bool restoreContextState)
{
	_context->ExecuteCommandList(pCommandList.Get(), restoreContextState);
}

void DX11GraphicContext::begin(ID3D11Asynchronous* async)
{
	_context->Begin(async);
}

void DX11GraphicContext::end(ID3D11Asynchronous* async)
{
	_context->End(async);
}

HRESULT DX11GraphicContext::getData(ID3D11Asynchronous* async, void * pData, uint32_t dataSize, uint32_t getDataFlags)
{
	return _context->GetData(async, pData, dataSize, getDataFlags);
}

void DX11GraphicContext::pushMarker(const wchar_t * name)
{
	_perf->BeginEvent(name);
}

void DX11GraphicContext::popMarker()
{
	_perf->EndEvent();
}
