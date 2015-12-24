#include "stdafx.h"
#include "D3D11HelloTriangle.h"

D3D11HelloTriangle::D3D11HelloTriangle(UINT width, UINT height, std::wstring name) :
	DXSample(width, height, name),
	m_frameIndex(0),
	m_viewport(),
	m_scissorRect()
{
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MaxDepth = 1.0f;

	m_scissorRect.right = static_cast<LONG>(width);
	m_scissorRect.bottom = static_cast<LONG>(height);
}

void D3D11HelloTriangle::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

// Load the rendering pipeline dependencies.
void D3D11HelloTriangle::LoadPipeline()
{
	HRESULT hr;

	uint32_t d3dFlag = D3D11_CREATE_DEVICE_DEBUG;
	//d3dFlag |= D3D11_CREATE_DEVICE_DEBUGGABLE;

	const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,
												D3D_FEATURE_LEVEL_11_0};

	ComPtr<IDXGIFactory1> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr)) { abort(); }

	hr = D3D11CreateDevice(nullptr,
							D3D_DRIVER_TYPE_HARDWARE,
							nullptr,
							d3dFlag,
							featureLevels,
							sizeof(featureLevels)/sizeof(featureLevels[0]),
							D3D11_SDK_VERSION,
							m_device.GetAddressOf(),
							nullptr,
							m_context.GetAddressOf());
	if (FAILED(hr)) { abort(); }

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	memset(&swapChainDesc, 0x00, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.OutputWindow = Win32Application::GetHwnd();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	hr = factory->CreateSwapChain(m_device.Get(), &swapChainDesc, m_swapChain.GetAddressOf());
	if (FAILED(hr)) { abort(); }

	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&m_renderTargets));
	m_device->CreateRenderTargetView(m_renderTargets.Get(), nullptr, rtvHanble.GetAddressOf());

	m_frameIndex = 0;
}

// Load the sample assets.
void D3D11HelloTriangle::LoadAssets()
{
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
}

void D3D11HelloTriangle::OnPresent()
{
	m_swapChain->Present(0, 0);
}

void D3D11HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.

	CloseHandle(m_fenceEvent);
}

