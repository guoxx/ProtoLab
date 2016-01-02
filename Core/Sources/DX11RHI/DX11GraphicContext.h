#pragma once

class DX11GraphicContext
{
public:
	enum class RHI_CLEAR_FLAG
	{
		RHI_CLEAR_DEPTH = D3D11_CLEAR_DEPTH,
		RHI_CLEAR_STENCIL = D3D11_CLEAR_STENCIL,
		RHI_CLEAR_DEPTH_STENCIL = RHI_CLEAR_DEPTH | RHI_CLEAR_STENCIL
	};

	DX11GraphicContext(ID3D11DeviceContext* ctx);
	~DX11GraphicContext();

	DX11GraphicContext(const DX11GraphicContext&) = delete;
	DX11GraphicContext& operator=(const DX11RenderTarget&) = delete;
	DX11GraphicContext(const DX11GraphicContext&&) = delete;
	DX11GraphicContext& operator=(const DX11RenderTarget&&) = delete;

	void mapResource();
	void unmapResource();

	void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth, uint8_t stencil);
	void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

private:

	ID3D11DeviceContext* _context{nullptr};
};

