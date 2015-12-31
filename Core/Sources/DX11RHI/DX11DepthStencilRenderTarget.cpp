#include "stdafx.h"
#include "DX11DepthStencilRenderTarget.h"


DX11DepthStencilRenderTarget::DX11DepthStencilRenderTarget()
{
}


DX11DepthStencilRenderTarget::~DX11DepthStencilRenderTarget()
{
}


ID3D11DepthStencilView* DX11DepthStencilRenderTarget::getRenderTarget(uint32_t mipmap)
{
	return _depthStencilRenderTargets[mipmap];
}


uint32_t DX11DepthStencilRenderTarget::getNumOfMipmaps()
{
	return _depthStencilRenderTargets.size();
}