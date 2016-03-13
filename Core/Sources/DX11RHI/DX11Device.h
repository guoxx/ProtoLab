#pragma once

class DX11Device : public Noncopyable, Nonmovable
{
public:
	explicit DX11Device(ComPtr<ID3D11Device> device);
	~DX11Device();

	ComPtr<ID3D11DeviceContext> createDeferredContext(uint32_t contextFlags);

	ComPtr<IDXGISwapChain> createSwapChain(HWND hwnd, uint32_t frameCount, uint32_t winWidth, uint32_t winHeight);

	ComPtr<ID3D11Buffer> createVertexBuffer(const void* memPtr, uint32_t memSize);
	ComPtr<ID3D11Buffer> createStreamOutputVertexBuffer(const void* memPtr, uint32_t memSize);
	ComPtr<ID3D11Buffer> createIndexBuffer(const void* memPtr, uint32_t memSize);
	ComPtr<ID3D11Buffer> createConstantBuffer(const void* memPtr, uint32_t memSize);

	ComPtr<ID3D11VertexShader> createVertexShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength);
	ComPtr<ID3D11GeometryShader> createGeometryShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength);
	ComPtr<ID3D11GeometryShader> createGeometryShaderWithStreamOutputFromBytecodes(const void *bytecode,
																					std::size_t bytecodeLength,
																					const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
																					uint32_t numEntries,
																					const uint32_t *pBufferStrides,
																					uint32_t numStrides,
																					uint32_t rasterizedStream);
	ComPtr<ID3D11PixelShader> createPixelShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength);
	ComPtr<ID3D11InputLayout> createInputLayout(const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ComPtr<ID3DBlob> vertexShaderBytecode);

	ComPtr<ID3D11SamplerState> createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, D3D11_COMPARISON_FUNC cmpFunc = D3D11_COMPARISON_NEVER);

	// number of mipmap levels include base
	ComPtr<ID3D11Texture2D> createTexture2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags);
	ComPtr<ID3D11Texture2D> createTexture2DFromSwapChain(ComPtr<IDXGISwapChain> swapChain);
	ComPtr<ID3D11ShaderResourceView> createShaderResourceViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT srvFormat, uint32_t numMipmap);
	ComPtr<ID3D11RenderTargetView> createRenderTargetViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice);
	ComPtr<ID3D11DepthStencilView> createDepthStencilViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice);

	ComPtr<ID3D11Texture2D> createTextureCube(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags);
	ComPtr<ID3D11ShaderResourceView> createShaderResourceViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT srvFormat, uint32_t numMipmap);
	ComPtr<ID3D11RenderTargetView> createRenderTargetViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice);
	ComPtr<ID3D11DepthStencilView> createDepthStencilViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice);

	// query interface
	ComPtr<ID3D11Query> createQuery(D3D11_QUERY query, uint32_t miscFlags = 0);

private:
	ComPtr<ID3D11Device> _device;
};

