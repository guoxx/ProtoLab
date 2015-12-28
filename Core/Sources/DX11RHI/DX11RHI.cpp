#include "stdafx.h"
#include "DX11RHI.h"
#include "DX11ResourceCreationHelpers.h"
#include "../Win32Application.h"

ID3D11RasterizerState*				DX11RHI::_defaultRasterizerState{nullptr};

D3D11_VIEWPORT						DX11RHI::_viewport{};
int32_t								DX11RHI::_frameIndex{0};

ComPtr<ID3D11Device>				DX11RHI::_device{nullptr};
ComPtr<ID3D11DeviceContext>			DX11RHI::_context{nullptr};
ComPtr<IDXGISwapChain>				DX11RHI::_swapChain{nullptr};
ComPtr<ID3D11RenderTargetView>		DX11RHI::_backbufferRtvHanble{nullptr};


void DX11RHI::initialize(uint32_t frameWidth, uint32_t frameHeight)
{
	HRESULT hr;

	uint32_t creationFlag = D3D11_CREATE_DEVICE_DEBUG;
	//creationFlag |= D3D11_CREATE_DEVICE_DEBUGGABLE;

	const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,
												D3D_FEATURE_LEVEL_11_0};

	ComPtr<IDXGIFactory1> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr)) { abort(); }

	hr = D3D11CreateDevice(nullptr,
							D3D_DRIVER_TYPE_HARDWARE,
							nullptr,
							creationFlag,
							featureLevels,
							sizeof(featureLevels)/sizeof(featureLevels[0]),
							D3D11_SDK_VERSION,
							_device.GetAddressOf(),
							nullptr,
							_context.GetAddressOf());
	if (FAILED(hr)) { abort(); }

	DXGI_SWAP_CHAIN_DESC swapChainDesc = createDxgiSwapChainDesc(Win32Application::GetHwnd(), FRAME_COUNT, frameWidth, frameHeight);
	hr = factory->CreateSwapChain(_device.Get(), &swapChainDesc, _swapChain.GetAddressOf());
	if (FAILED(hr)) { abort(); }

	ComPtr<ID3D11Texture2D> renderTexture;
	_swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTexture));
	_device->CreateRenderTargetView(renderTexture.Get(), nullptr, _backbufferRtvHanble.GetAddressOf());

	_frameIndex = 0;

	_viewport.Width = 0;
	_viewport.Height = 0;
	_viewport.MaxDepth = 1.0f;

	initializeDefaultRHIStates();
}


void DX11RHI::finalize()
{
	// TODO: clean up resources
}

void DX11RHI::initializeDefaultRHIStates()
{
	CD3D11_RASTERIZER_DESC rasterizerDesc{CD3D11_DEFAULT{}};	
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	_device->CreateRasterizerState(&rasterizerDesc, &_defaultRasterizerState);
}

ID3D11Buffer* DX11RHI::createVertexBuffer(void* memPtr, uint32_t memSize)
{
	D3D11_BUFFER_DESC desc = createDx11BufferDesc(D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, memSize);
	D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
	ID3D11Buffer* buffer{nullptr};
	_device->CreateBuffer(&desc, &resData, &buffer);
	return buffer;
}

ID3D11Buffer* DX11RHI::createIndexBuffer(void* memPtr, uint32_t memSize)
{
	D3D11_BUFFER_DESC desc = createDx11BufferDesc(D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, memSize);
	D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
	ID3D11Buffer* buffer{nullptr};
	_device->CreateBuffer(&desc, &resData, &buffer);
	return buffer;
}

ID3D11Buffer* DX11RHI::createConstantBuffer(void* memPtr, uint32_t memSize)
{
	D3D11_BUFFER_DESC desc = createDx11DynamicBufferDesc(D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, memSize);
	D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
	ID3D11Buffer* buffer{nullptr};
	_device->CreateBuffer(&desc, &resData, &buffer);
	return buffer;
}

ID3D11VertexShader* DX11RHI::createVertexShaderFromFile(const wchar_t* filename, const char* entryPoint, D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3D11InputLayout* &vertexDecl)
{
	ID3D11VertexShader* outShader{nullptr};

	uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* blob{nullptr};
	D3DCompileFromFile(filename, nullptr, nullptr, entryPoint, "vs_5_0", compileFlags, 0, &blob, nullptr);

	_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &outShader);
	_device->CreateInputLayout(desc, descElemCnt, blob->GetBufferPointer(), blob->GetBufferSize(), &vertexDecl);

	return outShader;
}

ID3D11PixelShader* DX11RHI::createPixelShaderFromFile(const wchar_t* filename, const char* entryPoint)
{
	ID3D11PixelShader* outShader{nullptr};

	uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* blob{nullptr};
	D3DCompileFromFile(filename, nullptr, nullptr, entryPoint, "ps_5_0", compileFlags, 0, &blob, nullptr);

	_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &outShader);

	return outShader;
}

void DX11RHI::destroyResource(ID3D11Resource* resourceToDelete)
{
	if (resourceToDelete)
		resourceToDelete->Release();
}

void DX11RHI::destroyVertexShader(ID3D11VertexShader* shaderToDelete)
{
	if (shaderToDelete)
		shaderToDelete->Release();
}

void DX11RHI::destroyVertexDeclaration(ID3D11InputLayout* declToDelete)
{
	if (declToDelete)
		declToDelete->Release();
}

void DX11RHI::destroyPixelShader(ID3D11PixelShader* shaderToDelete)
{
	if (shaderToDelete)
		shaderToDelete->Release();
}

void DX11RHI::setDefaultRHIStates()
{
	_context->RSSetState(_defaultRasterizerState);
}

void DX11RHI::setViewport(uint32_t topLeftX, uint32_t topLeftY, uint32_t width, uint32_t height)
{
	_viewport.TopLeftX = static_cast<float>(topLeftX);
	_viewport.TopLeftY = static_cast<float>(topLeftY);
	_viewport.Width = static_cast<float>(width);
	_viewport.Height = static_cast<float>(height);
	_context->RSSetViewports(1, &_viewport);
}

ID3D11RenderTargetView* DX11RHI::getBackbufferRTV()
{
	return _backbufferRtvHanble.Get();
}

void DX11RHI::clear(ID3D11RenderTargetView* rtv, float r, float g, float b, float a)
{
	const float clearColor[4] = {r, g, b, a};
	_context->ClearRenderTargetView(rtv, clearColor);
}

void DX11RHI::clear(ID3D11DepthStencilView* dsv, RHI_CLEAR_FLAG clearFlag, float depth, uint8_t stencil)
{
	_context->ClearDepthStencilView(dsv, static_cast<uint32_t>(clearFlag), depth, stencil);
}

void DX11RHI::drawIndex(uint32_t indexCount, uint32_t startIndexLoccation, uint32_t baseVertexLocation)
{
	_context->DrawIndexed(indexCount, startIndexLoccation, baseVertexLocation);
}

void DX11RHI::present()
{
	_swapChain->Present(0, 0);
}
