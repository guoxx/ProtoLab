#pragma once

class DX11GraphicContext : public Noncopyable
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

	void mapResource(ID3D11Resource* resource, uint32_t subresource, D3D11_MAP mapType, uint32_t mapFlags, D3D11_MAPPED_SUBRESOURCE *mappedResource);
	void unmapResource(ID3D11Resource* resource, uint32_t subresource);

	void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth = 1.0, uint8_t stencil = 0);
	void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

public:
	friend class DX11RHI;

	ID3D11DeviceContext* _context{nullptr};
};

