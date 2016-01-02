#pragma once

#include "DX11RenderTarget.h"
#include "DX11DepthStencilRenderTarget.h"

using Microsoft::WRL::ComPtr;

// TODO:
// 2. use draw command list to cache each draw call so that we can flush it on another thread
// 3. deferred context
// 4. make data private
class DX11RHI
{
public:
	enum class RHI_CLEAR_FLAG
	{
		RHI_CLEAR_DEPTH = D3D11_CLEAR_DEPTH,
		RHI_CLEAR_STENCIL = D3D11_CLEAR_STENCIL,
		RHI_CLEAR_DEPTH_STENCIL = RHI_CLEAR_DEPTH | RHI_CLEAR_STENCIL
	};

	static DX11RHI& getInst();

	void initialize();
	void finalize();

	void initializeDefaultRHIStates();

	// resources creation
	ID3D11DeviceContext* createDeviceContext();
	IDXGISwapChain* createSwapChain(HWND hwnd, uint32_t frameCount, uint32_t winWidth, uint32_t winHeight);

	ID3D11Buffer* createVertexBuffer(void* memPtr, uint32_t memSize);
	ID3D11Buffer* createIndexBuffer(void* memPtr, uint32_t memSize);
	ID3D11Buffer* createConstantBuffer(void* memPtr, uint32_t memSize);

	ID3D11VertexShader* createVertexShaderFromFile(const wchar_t* filename, const char* entryPoint, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl);
	ID3D11VertexShader* createVertexShaderFromBytecodes(const void *bytecode, size_t bytecodeLength, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl);
	ID3D11PixelShader* createPixelShaderFromFile(const wchar_t* filename, const char* entryPoint);
	ID3D11PixelShader* createPixelShaderFromBytecodes(const void *bytecode, size_t bytecodeLength);

	// number of mipmap levels include base
	DX11RenderTarget* createRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texelFormat);
	DX11RenderTarget* createRenderTargetFromSwapChain(IDXGISwapChain* swapChain);
	DX11DepthStencilRenderTarget* createDepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texelFormat);

	// resources deletion
	void destroyDeviceContext(ID3D11DeviceContext* ctxToDelete);
	void destroySwapChain(IDXGISwapChain* swapChainToDelete);
	void destroyResource(ID3D11Resource* resourceToDelete);
	void destroyVertexShader(ID3D11VertexShader* shaderToDelete);
	void destroyVertexDeclaration(ID3D11InputLayout* declToDelete);
	void destroyPixelShader(ID3D11PixelShader* shaderToDelete);
	void destroyView(ID3D11View* viewToDelete);
	void destroyRenderTarget(DX11RenderTarget* renderTargetToDelete);
	void destroyDepthStencilRenderTarget(DX11DepthStencilRenderTarget* renderTargetToDelete);

	// render states
	void setDefaultRHIStates();

	void setViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height);

	// draws
	void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth = 1.0f, uint8_t stencil = 0);
	void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

	void submit();

public:

	// default hardware states
	ID3D11RasterizerState*			_defaultRasterizerState{nullptr};
	ID3D11DepthStencilState*		_defaultDepthStencilState{nullptr};

	// render states stuffs
	D3D11_VIEWPORT					_viewport{};

	// render resources
	ComPtr<ID3D11Device>			_device{nullptr};
	ComPtr<ID3D11DeviceContext>		_immediateContext{nullptr};
	ComPtr<ID3D11DeviceContext>		_deferredContext{nullptr};

private:
	DX11RHI();
	~DX11RHI();

	DX11RHI(const DX11RHI&) = delete;
	DX11RHI& operator=(const DX11RHI&) = delete;

	DX11RHI(const DX11RHI&&) = delete;
	DX11RHI& operator=(const DX11RHI&&) = delete;
};
