#pragma once

class DX11VertexShader;
class DX11PixelShader;
class DX11RenderStateSet;


class DX11GraphicContext : public Noncopyable
{
public:
	enum class RHI_CLEAR_FLAG
	{
		RHI_CLEAR_DEPTH = D3D11_CLEAR_DEPTH,
		RHI_CLEAR_STENCIL = D3D11_CLEAR_STENCIL,
		RHI_CLEAR_DEPTH_STENCIL = RHI_CLEAR_DEPTH | RHI_CLEAR_STENCIL
	};

	explicit DX11GraphicContext(ComPtr<ID3D11DeviceContext> ctx);
	~DX11GraphicContext();

	void resetDefaultRenderStates(std::shared_ptr<DX11RenderStateSet> stateSet);

	// input assembler
	void IASetInputLayout(ID3D11InputLayout* inputLayout);
	void IASetVertexBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer *const *ppVertexBuffers, const uint32_t *pStrides, const uint32_t *pOffsets);
	void IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, uint32_t offset);
	void IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);

	// vertex stage
	void VSSetShader(DX11VertexShader* pVertexShader, ID3D11ClassInstance *const *ppClassInstances, uint32_t numClassInstances);
	void VSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void VSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);

	// rasterizer stage
	void RSSetState(ID3D11RasterizerState *pRasterizerState);
	void RSSetViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height, float minDepth = 0.0f, float maxDepth = 1.0f);
	void RSSetViewports(uint32_t numViewports, const D3D11_VIEWPORT *pViewports);

	// pixel stage
	void PSSetShader(DX11PixelShader* pPixelShader, ID3D11ClassInstance *const *ppClassInstances, uint32_t numClassInstances);
	void PSSetConstantBuffers(uint32_t startSlot, uint32_t numBuffers, ID3D11Buffer *const *ppConstantBuffers);
	void PSSetShaderResources(uint32_t startSlot, uint32_t numViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void PSSetSamplers(uint32_t startSlot, uint32_t numSamplers, ID3D11SamplerState *const *ppSamplers);

	// ouput merge
	void OMSetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, uint32_t stencilRef);
	void OMSetBlendState(ID3D11BlendState* pBlendState);
	void OMSetBlendState(ID3D11BlendState* pBlendState, const float blendFactor[4], uint32_t sampleMask);
	void OMSetRenderTargets(uint32_t numViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView);

	// update resources
	void mapResource(ID3D11Resource* resource, uint32_t subresource, D3D11_MAP mapType, uint32_t mapFlags, D3D11_MAPPED_SUBRESOURCE *mappedResource);
	void unmapResource(ID3D11Resource* resource, uint32_t subresource);

	// draws
	void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth = 1.0, uint8_t stencil = 0);
	void draw(uint32_t vertexCount, uint32_t startVertexLocation);
	void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

	// command list
	HRESULT finishCommandList(bool restoreDeferredContextState, ID3D11CommandList **ppCommandList);
	void executeCommandList(ComPtr<ID3D11CommandList> pCommandList, bool restoreContextState);

	// query api
	void begin(ComPtr<ID3D11Asynchronous> async);
	void end(ComPtr<ID3D11Asynchronous> async);
	void getData(ComPtr<ID3D11Asynchronous> async, void *pData, uint32_t dataSize, uint32_t getDataFlags);

private:
	ComPtr<ID3D11DeviceContext>	_context;
};


class DX11ResourceMapGuard : public Noncopyable, Nonmovable, Heaponly
{
public:
	explicit DX11ResourceMapGuard(DX11GraphicContext* ctx, ID3D11Resource* resource, uint32_t subresource = 0, D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD, uint32_t mapFlags = 0)
		: _context{ ctx }
		, _resource{ resource }
		, subresource{ subresource }
	{
		_context->mapResource(resource, subresource, mapType, mapFlags, &_mappedSubresource);
	}

	~DX11ResourceMapGuard()
	{
		_context->unmapResource(_resource, subresource);
	}

	template<class T>
	T* getPtr()
	{
		return static_cast<T*>(_mappedSubresource.pData);
	}

private:
	DX11GraphicContext* _context;
	ID3D11Resource* _resource;
	uint32_t subresource;
	D3D11_MAPPED_SUBRESOURCE _mappedSubresource;
};
