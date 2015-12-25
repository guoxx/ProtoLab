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
	_camera = std::make_shared<Camera>();
	_camera->lookAt(DirectX::XMVECTOR{1, 1, 1}, DirectX::XMVECTOR{0, 0, 0}, DirectX::XMVECTOR{0, 1, 0});
	_camera->setViewParams(60, m_width * 1.0f / m_height, 0.1f, 1000.0f);

	_dragonMesh = std::make_shared<Mesh>();
	_dragonMesh->LoadMeshFromFile(GetAssetFullPath(L"CornellBox-Original.obj").c_str());
	_dragonMesh->LoadMaterielFromFile(GetAssetFullPath(L"CornellBox-Original.mtl").c_str());


	struct SimpleVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 color;
	};
	
	static SimpleVertex vertexDatas[3]= {
		XMFLOAT3(-0.5, -0.5, 0), XMFLOAT3(1, 0, 0),
		XMFLOAT3( 0.5, -0.5, 0), XMFLOAT3(0, 1, 0),
		XMFLOAT3(-0.5,  0.5, 0), XMFLOAT3(0, 0, 1),
	};

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertexDatas);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertexDatas;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	m_device->CreateBuffer(&bufferDesc, &InitData, &_vbuffer);

	static uint32_t indexDatas[] = {0, 2, 1};

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indexDatas);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA IndexInitData;
	IndexInitData.pSysMem = indexDatas;
	IndexInitData.SysMemPitch = 0;
	IndexInitData.SysMemSlicePitch = 0;
	m_device->CreateBuffer(&indexBufferDesc, &IndexInitData, &_ibuffer);

	uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* vexBlob;
	ID3DBlob* pixBlob;
	D3DCompileFromFile(GetAssetFullPath(L"hello_triangle.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vexBlob, nullptr);
	D3DCompileFromFile(GetAssetFullPath(L"hello_triangle.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixBlob, nullptr);

	m_device->CreateVertexShader(vexBlob->GetBufferPointer(), vexBlob->GetBufferSize(), nullptr, &_vexShader);
	m_device->CreatePixelShader(pixBlob->GetBufferPointer(), pixBlob->GetBufferSize(), nullptr, &_pixShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_device->CreateInputLayout(inputDesc, sizeof(inputDesc)/sizeof(inputDesc[0]), vexBlob->GetBufferPointer(), vexBlob->GetBufferSize(), &_vlayout);
}

// Update frame-based values.
void D3D11HelloTriangle::OnUpdate()
{
}

// Render the scene.
void D3D11HelloTriangle::OnRender()
{
	float clearColor[] = {0, 0, 0, 0};
	m_context->ClearRenderTargetView(rtvHanble.Get(), clearColor);

	ID3D11Buffer* buffers[] = {_vbuffer, _vbuffer};
	uint32_t strides[] = {sizeof(XMFLOAT3)*2, sizeof(XMFLOAT3)*2};
	uint32_t offsets[] = {0, sizeof(XMFLOAT3)*2};
	ID3D11RenderTargetView* rtvs[] = {rtvHanble.Get()};
	//ID3D11Buffer* buffers[] = {_vbuffer};
	//uint32_t strides[] = {sizeof(XMFLOAT3)*2};
	//uint32_t offsets[] = {0};
	//ID3D11RenderTargetView* rtvs[] = {rtvHanble.Get()};

	m_context->RSSetViewports(1, &m_viewport);
	m_context->IASetInputLayout(_vlayout);
	m_context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	m_context->IASetIndexBuffer(_ibuffer, DXGI_FORMAT_R32_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->VSSetShader(_vexShader, 0, 0);
	m_context->PSSetShader(_pixShader, 0, 0);
	m_context->OMSetRenderTargets(1, rtvs, nullptr);
	m_context->DrawIndexed(3, 0, 0);
}

void D3D11HelloTriangle::OnPresent()
{
	m_swapChain->Present(0, 0);
}

void D3D11HelloTriangle::OnDestroy()
{
	// Ensure that the GPU is no longer referencing resources that are about to be
	// cleaned up by the destructor.
}

