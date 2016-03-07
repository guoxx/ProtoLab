#include "stdafx.h"
#include "DX11Device.h"
#include "DX11ResourceCreationHelpers.h"


DX11Device::DX11Device(ComPtr<ID3D11Device> device)
	: _device{device}
{
}

DX11Device::~DX11Device()
{
}

ComPtr<ID3D11DeviceContext> DX11Device::createDeferredContext(uint32_t contextFlags)
{
	ComPtr<ID3D11DeviceContext> context;
	CHECK(_device->CreateDeferredContext(contextFlags, context.GetAddressOf()) == S_OK);
	return context;
}


ComPtr<IDXGISwapChain> DX11Device::createSwapChain(HWND hwnd, uint32_t frameCount, uint32_t winWidth, uint32_t winHeight)
{
	HRESULT hr;

	ComPtr<IDXGIDevice> pDXGIDevice;
	hr = _device.CopyTo(pDXGIDevice.GetAddressOf());

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter));

	ComPtr<IDXGIFactory> pFactory;
	pDXGIAdapter->GetParent(IID_PPV_ARGS(&pFactory));

	ComPtr<IDXGISwapChain> outSwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = createDxgiSwapChainDesc(hwnd, frameCount, winWidth, winHeight);
	hr = pFactory->CreateSwapChain(_device.Get(), &swapChainDesc, outSwapChain.GetAddressOf());
	CHECK(hr == S_OK);
	return outSwapChain;
}

ComPtr<ID3D11Buffer> DX11Device::createVertexBuffer(const void* memPtr, uint32_t memSize)
{
	D3D11_BUFFER_DESC desc = createDx11BufferDesc(D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, memSize);
	D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
	ComPtr<ID3D11Buffer> buffer;
	_device->CreateBuffer(&desc, &resData, buffer.GetAddressOf());
	return buffer;
}

ComPtr<ID3D11Buffer> DX11Device::createIndexBuffer(const void* memPtr, uint32_t memSize)
{
	D3D11_BUFFER_DESC desc = createDx11BufferDesc(D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, memSize);
	D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
	ComPtr<ID3D11Buffer> buffer;
	_device->CreateBuffer(&desc, &resData, buffer.GetAddressOf());
	return buffer;
}

ComPtr<ID3D11Buffer> DX11Device::createConstantBuffer(const void* memPtr, uint32_t memSize)
{
	ComPtr<ID3D11Buffer> buffer;
	D3D11_BUFFER_DESC desc = createDx11DynamicBufferDesc(D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, memSize);
	if (memPtr != nullptr)
	{
		D3D11_SUBRESOURCE_DATA resData = createDx11SubresourceData(memPtr);
		_device->CreateBuffer(&desc, &resData, buffer.GetAddressOf());
	}
	else
	{
		_device->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	}
	return buffer;
}

ComPtr<ID3D11VertexShader> DX11Device::createVertexShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength)
{
	ComPtr<ID3D11VertexShader> outShader;
	_device->CreateVertexShader(bytecode, bytecodeLength, nullptr, outShader.GetAddressOf());
	return outShader;
}

ComPtr<ID3D11PixelShader> DX11Device::createPixelShaderFromBytecodes(const void *bytecode, std::size_t bytecodeLength)
{
	ComPtr<ID3D11PixelShader> outShader;
	_device->CreatePixelShader(bytecode, bytecodeLength, nullptr, outShader.GetAddressOf());
	return outShader;
}

ComPtr<ID3D11InputLayout> DX11Device::createInputLayout(const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ComPtr<ID3DBlob> vertexShaderBytecode)
{
	ComPtr<ID3D11InputLayout> inputLayout;
	_device->CreateInputLayout(desc, descElemCnt, vertexShaderBytecode->GetBufferPointer(), vertexShaderBytecode->GetBufferSize(), inputLayout.GetAddressOf());
	return inputLayout;
}

ComPtr<ID3D11SamplerState> DX11Device::createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode)
{
	ComPtr<ID3D11SamplerState> samp;
	D3D11_SAMPLER_DESC sampDesc = createDx11SamplerDesc(filter, addressMode);
	_device->CreateSamplerState(&sampDesc, samp.GetAddressOf());
	return samp;
}

ComPtr<ID3D11Texture2D> DX11Device::createTexture2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags)
{
	ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC desc = createDx11Texture2dDesc(width, height, numMipmap, texFormat);
	desc.BindFlags = bindFlags;
	_device->CreateTexture2D(&desc, nullptr, tex.GetAddressOf());
	return tex;
}

ComPtr<ID3D11Texture2D> DX11Device::createTexture2DFromSwapChain(ComPtr<IDXGISwapChain> swapChain)
{
	ComPtr<ID3D11Texture2D> tex;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&tex));
	return tex;
}

ComPtr<ID3D11ShaderResourceView> DX11Device::createShaderResourceViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT srvFormat, uint32_t numMipmap)
{
	ComPtr<ID3D11ShaderResourceView> textureSRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = createDx11ShaderResourceViewDescTex2d(srvFormat, numMipmap);
	_device->CreateShaderResourceView(texture.Get(), &srvDesc, textureSRV.GetAddressOf());
	return textureSRV;
}

ComPtr<ID3D11RenderTargetView> DX11Device::createRenderTargetViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice)
{
	ComPtr<ID3D11RenderTargetView> rtv;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = createDx11RenderTargetViewDescTex2d(rtvFormat, mipSlice);
	_device->CreateRenderTargetView(texture.Get(), &rtvDesc, rtv.GetAddressOf());
	return rtv;
}

ComPtr<ID3D11DepthStencilView> DX11Device::createDepthStencilViewTex2d(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice)
{
	ComPtr<ID3D11DepthStencilView> dsv;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = createDx11DepthStencilViewDescTex2d(dsvFormat, mipSlice);
	_device->CreateDepthStencilView(texture.Get(), &dsvDesc, dsv.GetAddressOf());
	return dsv;
}

ComPtr<ID3D11Texture2D> DX11Device::createTextureCube(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags)
{
	ComPtr<ID3D11Texture2D> tex;
	D3D11_TEXTURE2D_DESC desc = createDx11Texture2dDesc(width, height, numMipmap, texFormat);
	desc.BindFlags = bindFlags;
	_device->CreateTexture2D(&desc, nullptr, tex.GetAddressOf());
	return tex;
}

ComPtr<ID3D11ShaderResourceView> DX11Device::createShaderResourceViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT srvFormat, uint32_t numMipmap)
{
	ComPtr<ID3D11ShaderResourceView> srv;
	auto srvDesc = createDx11ShaderResourceViewDescTexCube(srvFormat, numMipmap);
	_device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf());
	return srv;
}

ComPtr<ID3D11RenderTargetView> DX11Device::createRenderTargetViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice)
{
	ComPtr<ID3D11RenderTargetView> rtv;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = createDx11RenderTargetViewDescTexCube(rtvFormat, mipSlice);
	_device->CreateRenderTargetView(texture.Get(), &rtvDesc, rtv.GetAddressOf());
	return rtv;
}

ComPtr<ID3D11DepthStencilView> DX11Device::createDepthStencilViewTexCube(ComPtr<ID3D11Texture2D> texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice)
{
	ComPtr<ID3D11DepthStencilView> dsv;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = createDx11DepthStencilViewDescTexCube(dsvFormat, mipSlice);
	_device->CreateDepthStencilView(texture.Get(), &dsvDesc, dsv.GetAddressOf());
	return dsv;
}

ComPtr<ID3D11Query> DX11Device::createQuery(D3D11_QUERY query, uint32_t miscFlags)
{
	D3D11_QUERY_DESC desc;
	desc.Query = query;
	desc.MiscFlags = miscFlags;

	ComPtr<ID3D11Query> outQuery;
	_device->CreateQuery(&desc, outQuery.GetAddressOf());
	return outQuery;
}
