#include "stdafx.h"
#include "DX11RenderTarget.h"


DX11RenderTarget::DX11RenderTarget()
{
}


DX11RenderTarget::~DX11RenderTarget()
{
	RHI::destroyResource(_texture);

	for (auto rtv : _renderTargets)
	{
		RHI::destroyView(rtv);
	}
}


ID3D11RenderTargetView* DX11RenderTarget::getRenderTarget(uint32_t mipmap)
{
	return _renderTargets[mipmap];
}


uint32_t DX11RenderTarget::getNumOfMipmaps()
{
	return _renderTargets.size();
}
