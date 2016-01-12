#pragma once

class DX11RenderStateCache
{
public:
	DX11RenderStateCache();
	~DX11RenderStateCache();

	void setRenderTarget(std::shared_ptr<DX11RenderTarget> rt, uint32_t slot);
	void setDepthRenderTarget(std::shared_ptr<DX11DepthStencilRenderTarget> depthRT);

private:
	ComPtr<ID3D11BlendState> _blendState;
	ComPtr<ID3D11DepthStencilState> _depthStencilState;

	std::array<std::shared_ptr<DX11RenderTarget>, DX11Limits::MAX_RENDER_TARGETS_PER_DRAW> _renderTargets;
	std::shared_ptr<DX11DepthStencilRenderTarget> _depthRenderTarget;
};

