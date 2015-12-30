#pragma once

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

	static void initialize(uint32_t frameWidth, uint32_t frameHeight);
	static void finalize();

	static void initializeDefaultRHIStates();

	// resources creation
	static ID3D11Buffer* createVertexBuffer(void* memPtr, uint32_t memSize);
	static ID3D11Buffer* createIndexBuffer(void* memPtr, uint32_t memSize);
	static ID3D11Buffer* createConstantBuffer(void* memPtr, uint32_t memSize);

	static ID3D11VertexShader* createVertexShaderFromFile(const wchar_t* filename, const char* entryPoint, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl);
	static ID3D11PixelShader* createPixelShaderFromFile(const wchar_t* filename, const char* entryPoint);

	// resources deletion
	static void destroyResource(ID3D11Resource* resourceToDelete);
	static void destroyVertexShader(ID3D11VertexShader* shaderToDelete);
	static void destroyVertexDeclaration(ID3D11InputLayout* declToDelete);
	static void destroyPixelShader(ID3D11PixelShader* shaderToDelete);

	// render states
	static void setDefaultRHIStates();

	static void setViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height);

	static ID3D11RenderTargetView* getBackbufferRTV();

	// draws
	static void clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a);
	static void clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth, uint8_t stencil);
	static void drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation);

	// present
	static void present();

public:

	// default hardware states
	static ID3D11RasterizerState*			_defaultRasterizerState;
	static ID3D11DepthStencilState*			_defaultDepthStencilState;

	// render states stuffs
	static D3D11_VIEWPORT					_viewport;
	static int32_t							_frameIndex;

	// render resources
	static ComPtr<ID3D11Device>				_device;
	static ComPtr<ID3D11DeviceContext>		_context;
	static ComPtr<IDXGISwapChain>			_swapChain;
	static ComPtr<ID3D11RenderTargetView>	_backbufferRtvHanble;

	// constants
	const static int32_t					FRAME_COUNT = 2;

private:
	DX11RHI() {};
	~DX11RHI() {};
};
