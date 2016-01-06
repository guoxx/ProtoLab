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

	ID3D11VertexShader* createVertexShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength);
	ID3D11PixelShader* createPixelShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength);
	ID3D11InputLayout* createVertexDeclaration(const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3DBlob* vertexShaderBytecode);

	ID3D11SamplerState* createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode);

	// number of mipmap levels include base
	ID3D11Texture2D* createTexture2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags);
	ID3D11Texture2D* createTexture2DFromSwapChain(IDXGISwapChain* swapChain);
	ID3D11ShaderResourceView* createShaderResourceViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT srvFormat, uint32_t numMipmap);
	ID3D11RenderTargetView* createRenderTargetViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice);
	ID3D11DepthStencilView* createDepthStencilViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice);

	// resources deletion
	void destroyDeviceContext(ID3D11DeviceContext* ctxToDelete);
	void destroySwapChain(IDXGISwapChain* swapChainToDelete);
	void destroyResource(ID3D11Resource* resourceToDelete);
	void destroyVertexShader(ID3D11VertexShader* shaderToDelete);
	void destroyVertexDeclaration(ID3D11InputLayout* declToDelete);
	void destroySamplerState(ID3D11SamplerState* sampToDelete);
	void destroyPixelShader(ID3D11PixelShader* shaderToDelete);
	void destroyView(ID3D11View* viewToDelete);
	void destroyBlob(ID3DBlob* blobToDelete);

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
