#include "stdafx.h"
#include "DX11RenderStateCache.h"


DX11RenderStateCache::DX11RenderStateCache()
{
}


DX11RenderStateCache::~DX11RenderStateCache()
{
}

void DX11RenderStateCache::setRenderTarget(std::shared_ptr<DX11RenderTarget> rt, uint32_t slot)
{
	CHECK(slot >= 0 && slot < DX11Limits::MAX_RENDER_TARGETS_PER_DRAW);
	_renderTargets[slot] = rt;
}

void DX11RenderStateCache::setDepthRenderTarget(std::shared_ptr<DX11DepthStencilRenderTarget> depthRT)
{
	_depthRenderTarget = depthRT;
}
