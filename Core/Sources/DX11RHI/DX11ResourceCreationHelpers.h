#pragma once

D3D11_BUFFER_DESC createDx11BufferDesc(D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, UINT byteWidth)
{
	D3D11_BUFFER_DESC bufferDesc;	
	memset(&bufferDesc, 0x00, sizeof(bufferDesc));
	bufferDesc.Usage = usage;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	return bufferDesc;
}

D3D11_BUFFER_DESC createDx11DynamicBufferDesc(D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, UINT byteWidth)
{
	D3D11_BUFFER_DESC bufferDesc;	
	memset(&bufferDesc, 0x00, sizeof(bufferDesc));
	bufferDesc.Usage = usage;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	return bufferDesc;
}

D3D11_SUBRESOURCE_DATA createDx11SubresourceData(const void* memPtr)
{
	D3D11_SUBRESOURCE_DATA resData;
	memset(&resData, 0x00, sizeof(resData));
	resData.pSysMem = memPtr;
	resData.SysMemPitch = 0;
	resData.SysMemSlicePitch = 0;
	return resData;
}

DXGI_SWAP_CHAIN_DESC createDxgiSwapChainDesc(HWND hwnd, uint32_t frameCnt, uint32_t frameWidth, uint32_t frameHeight)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0x00, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = frameCnt;
	swapChainDesc.BufferDesc.Width = frameWidth;
	swapChainDesc.BufferDesc.Height = frameHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 30;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	return swapChainDesc;
}

D3D11_TEXTURE2D_DESC createDx11Texture2dDesc(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texelFormat)
{
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0x00, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = numMipmap;
	texDesc.ArraySize = 1;
	texDesc.Format = texelFormat;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	return texDesc;
}

D3D11_SHADER_RESOURCE_VIEW_DESC createDx11ShaderResourceViewDescTex2d(DXGI_FORMAT texelFormat, uint32_t mipLevels)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0x00, sizeof(srvDesc));
	srvDesc.Format = texelFormat;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = mipLevels;
	return srvDesc;
}

D3D11_RENDER_TARGET_VIEW_DESC createDx11RenderTargetViewDescTex2d(DXGI_FORMAT texelFormat, uint32_t mipSlice)
{
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0x00, sizeof(rtvDesc));
	rtvDesc.Format = texelFormat;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = mipSlice;
	return rtvDesc;
}

D3D11_DEPTH_STENCIL_VIEW_DESC createDx11DepthStencilViewDescTex2d(DXGI_FORMAT texelFormat, uint32_t mipSlice)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	memset(&dsvDesc, 0x00, sizeof(dsvDesc));
	dsvDesc.Format = texelFormat;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;
	dsvDesc.Texture2D.MipSlice = mipSlice;
	return dsvDesc;
}

D3D11_SAMPLER_DESC createDx11SamplerDesc(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode)
{
	CD3D11_SAMPLER_DESC sampDesc{CD3D11_DEFAULT{}};
	sampDesc.Filter = filter;
	sampDesc.AddressU = addressMode;
	sampDesc.AddressV = addressMode;
	sampDesc.AddressW = addressMode;
	return sampDesc;
}