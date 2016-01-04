#include "stdafx.h"
#include "DX11RenderTarget.h"


DX11RenderTarget::DX11RenderTarget()
{
}


DX11RenderTarget::~DX11RenderTarget()
{
	RHI::getInst().destroyResource(_texture);
	RHI::getInst().destroyView(_textureSRV);

	for (auto rtv : _renderTargets)
	{
		RHI::getInst().destroyView(rtv);
	}
}

ID3D11ShaderResourceView * DX11RenderTarget::getTextureSRV() const
{
	return _textureSRV;
}

ID3D11RenderTargetView* DX11RenderTarget::getRenderTarget(uint32_t mipmap) const
{
	return _renderTargets[mipmap];
}


uint32_t DX11RenderTarget::getNumOfMipmaps()
{
	return _renderTargets.size();
}
