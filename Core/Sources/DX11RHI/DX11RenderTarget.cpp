#include "stdafx.h"
#include "DX11RenderTarget.h"


DX11RenderTarget::DX11RenderTarget()
{
}


DX11RenderTarget::~DX11RenderTarget()
{
	RHI::GetInst().destroyResource(_texture);

	for (auto rtv : _renderTargets)
	{
		RHI::GetInst().destroyView(rtv);
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
