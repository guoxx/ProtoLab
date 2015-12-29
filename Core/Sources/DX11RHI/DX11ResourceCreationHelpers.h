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

D3D11_SUBRESOURCE_DATA createDx11SubresourceData(void* memPtr)
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