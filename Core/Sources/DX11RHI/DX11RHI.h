#pragma once

#include "DX11RenderTarget.h"
#include "DX11DepthStencilRenderTarget.h"
#include "DX11GraphicContext.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

using Microsoft::WRL::ComPtr;

// TODO:
// 2. use draw command list to cache each draw call so that we can flush it on another thread
// 3. deferred context
class DX11RHI : public Noncopyable, Nonmovable
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

	ID3D11VertexShader* createVertexShaderFromBytecodes(const void *bytecode, size_t bytecodeLength);
	ID3D11PixelShader* createPixelShaderFromBytecodes(const void *bytecode, size_t bytecodeLength);
	ID3D11InputLayout* createVertexDeclaration(D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3DBlob* vertexShaderBytecode);

	// number of mipmap levels include base
	DX11RenderTarget* createRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT rtvFormat);
	DX11RenderTarget* createRenderTargetFromSwapChain(IDXGISwapChain* swapChain);
	DX11DepthStencilRenderTarget* createDepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT dsvFormat);

	// resources deletion
	void destroyDeviceContext(ID3D11DeviceContext* ctxToDelete);
	void destroySwapChain(IDXGISwapChain* swapChainToDelete);
	void destroyResource(ID3D11Resource* resourceToDelete);
	void destroyVertexShader(ID3D11VertexShader* shaderToDelete);
	void destroyVertexDeclaration(ID3D11InputLayout* declToDelete);
	void destroyPixelShader(ID3D11PixelShader* shaderToDelete);
	void destroyView(ID3D11View* viewToDelete);
	void destroyBlob(ID3DBlob* blobToDelete);
	void destroyRenderTarget(DX11RenderTarget* renderTargetToDelete);
	void destroyDepthStencilRenderTarget(DX11DepthStencilRenderTarget* renderTargetToDelete);

	// render states
	void setDefaultRHIStates();

	// gfx contexts
	std::shared_ptr<DX11GraphicContext> getContext();

	// shader compilation
	ID3DBlob* compileShader(const wchar_t* filename, const char* entryPoint, const char* profile);
	ID3DBlob* compileVertexShader(const wchar_t* filename, const char* entryPoint);
	ID3DBlob* compilePixelShader(const wchar_t* filename, const char* entryPoint);

	void submit();

private:

	// default hardware states
	ID3D11RasterizerState*			_defaultRasterizerState{nullptr};
	ID3D11DepthStencilState*		_defaultDepthStencilState{nullptr};

	// render resources
	ComPtr<ID3D11Device>				_device{nullptr};
	std::shared_ptr<DX11GraphicContext>	_immediateContext{nullptr};
	std::shared_ptr<DX11GraphicContext>	_deferredContext{nullptr};


private:
	DX11RHI();
	~DX11RHI();
};
