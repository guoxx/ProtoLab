#pragma once

#include "DX11RenderTarget.h"
#include "DX11DepthStencilRenderTarget.h"

using Microsoft::WRL::ComPtr;

#ifdef _MSC_VER
#define cbuffer struct __declspec(align(16))
#else
#define cbuffer alignas(16)
#endif

// TODO:
// 1. use singleton to have const functions
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

	static void initialize();
	static void finalize();

	static void initializeDefaultRHIStates();

	// resources creation
	static IDXGISwapChain* createSwapChain(HWND hwnd, uint32_t frameCount, uint32_t winWidth, uint32_t winHeight);

	static ID3D11Buffer* createVertexBuffer(void* memPtr, uint32_t memSize);
	static ID3D11Buffer* createIndexBuffer(void* memPtr, uint32_t memSize);
	static ID3D11Buffer* createConstantBuffer(void* memPtr, uint32_t memSize);

	static ID3D11VertexShader* createVertexShaderFromFile(const wchar_t* filename, const char* entryPoint, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl);
	static ID3D11VertexShader* createVertexShaderFromBytecodes(const void *bytecode, size_t bytecodeLength, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl);
	static ID3D11PixelShader* createPixelShaderFromFile(const wchar_t* filename, const char* entryPoint);
	static ID3D11PixelShader* createPixelShaderFromBytecodes(const void *bytecode, size_t bytecodeLength);

	// number of mipmap levels include base
	static DX11RenderTarget* createRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texelFormat);
	static DX11DepthStencilRenderTarget* createDepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texelFormat);
	static DX11RenderTarget* createRenderTargetViewFromSwapChain(IDXGISwapChain* swapChain);

	// resources deletion
	static void destroySwapChain(IDXGISwapChain* swapChainToDelete);
	static void destroyResource(ID3D11Resource* resourceToDelete);
	static void destroyVertexShader(ID3D11VertexShader* shaderToDelete);
	static void destroyVertexDeclaration(ID3D11InputLayout* declToDelete);
	static void destroyPixelShader(ID3D11PixelShader* shaderToDelete);
	static void destroyView(ID3D11View* viewToDelete);
	static void destroyRenderTarget(DX11RenderTarget* renderTargetToDelete);
	static void destroyDepthStencilRenderTarget(DX11DepthStencilRenderTarget* renderTargetToDelete);

	// render states
	static void setDefaultRHIStates();

	static void setViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height);

	// draws
	static void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	static void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth = 1.0f, uint8_t stencil = 0);
	static void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

public:

	// default hardware states
	static ID3D11RasterizerState*			_defaultRasterizerState;
	static ID3D11DepthStencilState*			_defaultDepthStencilState;

	// render states stuffs
	static D3D11_VIEWPORT					_viewport;

	// render resources
	static ComPtr<ID3D11Device>				_device;
	static ComPtr<ID3D11DeviceContext>		_context;

private:
	DX11RHI() {};
	~DX11RHI() {};
};
