#include "stdafx.h"
#include "DX11RHI.h"
#include "DX11ResourceCreationHelpers.h"


DX11RHI& DX11RHI::getInst()
{
	static DX11RHI _inst;
	return _inst;
}

void DX11RHI::initialize()
{
	HRESULT hr;

	uint32_t creationFlag = 0;
#ifdef _DEBUG
	creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
	//creationFlag |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif

	const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1,
												D3D_FEATURE_LEVEL_11_0};

	ComPtr<IDXGIFactory1> factory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	CHECK(hr == S_OK);

	ID3D11DeviceContext* immediateCtx;
	hr = D3D11CreateDevice(nullptr,
							D3D_DRIVER_TYPE_HARDWARE,
							nullptr,
							creationFlag,
							featureLevels,
							sizeof(featureLevels)/sizeof(featureLevels[0]),
							D3D11_SDK_VERSION,
							_device.GetAddressOf(),
							nullptr,
							&immediateCtx);
	CHECK(hr == S_OK);
	_immediateContext = std::make_shared<DX11GraphicContext>(immediateCtx);

	ID3D11DeviceContext* deferredCtx;
	hr = _device->CreateDeferredContext(0, &deferredCtx);
	CHECK(hr == S_OK);
	_deferredContext = std::make_shared<DX11GraphicContext>(deferredCtx);

	initializeDefaultRHIStates();

#ifdef _DEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	hr = _device.As(&d3dDebug);
	if (hr == S_OK)
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		hr = d3dDebug.As(&d3dInfoQueue);
		CHECK(hr == S_OK);

		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
		D3D11_MESSAGE_ID hide[] =
		{
			D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
			// TODO: Add more message IDs here as needed 
		};
		D3D11_INFO_QUEUE_FILTER filter;
		memset(&filter, 0, sizeof(filter));
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;
		d3dInfoQueue->AddStorageFilterEntries(&filter);
	}
 #endif
}


void DX11RHI::finalize()
{
	// TODO: clean up resources
}

void DX11RHI::initializeDefaultRHIStates()
{
	CD3D11_RASTERIZER_DESC rasterizerDesc{CD3D11_DEFAULT{}};	
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = false;
	_device->CreateRasterizerState(&rasterizerDesc, &_defaultRasterizerState);

	CD3D11_DEPTH_STENCIL_DESC depthStencilDesc{CD3D11_DEFAULT{}};
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	_device->CreateDepthStencilState(&depthStencilDesc, &_defaultDepthStencilState);
}

ID3D11DeviceContext * DX11RHI::createDeviceContext()
{
	ID3D11DeviceContext* ctx{nullptr};
	CHECK(_device->CreateDeferredContext(0, &ctx) == S_OK);
	return ctx;
}

IDXGISwapChain* DX11RHI::createSwapChain(HWND hwnd, uint32_t frameCount, uint32_t winWidth, uint32_t winHeight)
{
	HRESULT hr;

	ComPtr<IDXGIDevice> pDXGIDevice;
	hr = _device.CopyTo( pDXGIDevice.GetAddressOf() );

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(IID_PPV_ARGS(&pDXGIAdapter));

	ComPtr<IDXGIFactory> pFactory;
	pDXGIAdapter->GetParent(IID_PPV_ARGS(&pFactory));

	IDXGISwapChain* outSwapChain;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = createDxgiSwapChainDesc(hwnd, frameCount, winWidth, winHeight);
	hr = pFactory->CreateSwapChain(_device.Get(), &swapChainDesc, &outSwapChain);
	CHECK(hr == S_OK);
	return outSwapChain;
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

ID3D11VertexShader* DX11RHI::createVertexShaderFromBytecodes(const void *bytecode, size_t bytecodeLength)
{
	ID3D11VertexShader* outShader{nullptr};
	_device->CreateVertexShader(bytecode, bytecodeLength, nullptr, &outShader);
	return outShader;
}

ID3D11PixelShader* DX11RHI::createPixelShaderFromBytecodes(const void *bytecode, size_t bytecodeLength)
{
	ID3D11PixelShader* outShader{nullptr};
	_device->CreatePixelShader(bytecode, bytecodeLength, nullptr, &outShader);
	return outShader;
}

ID3D11InputLayout* DX11RHI::createVertexDeclaration(const D3D11_INPUT_ELEMENT_DESC* desc, uint32_t descElemCnt, ID3DBlob* vertexShaderBytecode)
{
	ID3D11InputLayout* vertexDecl{nullptr};
	_device->CreateInputLayout(desc, descElemCnt, vertexShaderBytecode->GetBufferPointer(), vertexShaderBytecode->GetBufferSize(), &vertexDecl);
	return vertexDecl;
}

ID3D11SamplerState* DX11RHI::createSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode)
{
	ID3D11SamplerState* samp{nullptr};
	D3D11_SAMPLER_DESC sampDesc = createDx11SamplerDesc(filter, addressMode);
	_device->CreateSamplerState(&sampDesc, &samp);
	return samp;
}

ID3D11Texture2D* DX11RHI::createTexture2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, uint32_t bindFlags)
{
	ID3D11Texture2D* tex{nullptr};
	D3D11_TEXTURE2D_DESC desc = createDx11Texture2dDesc(width, height, numMipmap, texFormat);
	desc.BindFlags = bindFlags;
	_device->CreateTexture2D(&desc, nullptr, &tex);
	return tex;
}

ID3D11Texture2D* DX11RHI::createTexture2DFromSwapChain(IDXGISwapChain* swapChain)
{
	ID3D11Texture2D* tex{nullptr};
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&tex));
	return tex;
}

ID3D11ShaderResourceView* DX11RHI::createShaderResourceViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT srvFormat, uint32_t numMipmap)
{
	ID3D11ShaderResourceView* textureSRV{nullptr};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = createDx11ShaderResourceViewDescTex2d(srvFormat, numMipmap);
	_device->CreateShaderResourceView(texture, &srvDesc, &textureSRV);
	return textureSRV;
}

ID3D11RenderTargetView* DX11RHI::createRenderTargetViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT rtvFormat, uint32_t mipSlice)
{
	ID3D11RenderTargetView* rtv{nullptr};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = createDx11RenderTargetViewDescTex2d(rtvFormat, mipSlice);
	_device->CreateRenderTargetView(texture, &rtvDesc, &rtv);
	return rtv;
}

ID3D11DepthStencilView* DX11RHI::createDepthStencilViewTex2d(ID3D11Texture2D* texture, DXGI_FORMAT dsvFormat, uint32_t mipSlice)
{
	ID3D11DepthStencilView* dsv{nullptr};
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = createDx11DepthStencilViewDescTex2d(dsvFormat, mipSlice);
	_device->CreateDepthStencilView(texture, &dsvDesc, &dsv);
	return dsv;
}

void DX11RHI::destroyDeviceContext(ID3D11DeviceContext * ctxToDelete)
{
	if (ctxToDelete)
		ctxToDelete->Release();
}

void DX11RHI::destroySwapChain(IDXGISwapChain* swapChainToDelete)
{
	if (swapChainToDelete)
		swapChainToDelete->Release();
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

void DX11RHI::destroySamplerState(ID3D11SamplerState* sampToDelete)
{
	if (sampToDelete)
		sampToDelete->Release();
}

void DX11RHI::destroyPixelShader(ID3D11PixelShader* shaderToDelete)
{
	if (shaderToDelete)
		shaderToDelete->Release();
}

void DX11RHI::destroyView(ID3D11View* viewToDelete)
{
	if (viewToDelete)
		viewToDelete->Release();
}

void DX11RHI::destroyBlob(ID3DBlob * blobToDelete)
{
	if (blobToDelete)
		blobToDelete->Release();
}

void DX11RHI::setDefaultRHIStates()
{
	_deferredContext->RSSetState(_defaultRasterizerState);
	_deferredContext->OMSetDepthStencilState(_defaultDepthStencilState, 0);
}

std::shared_ptr<DX11GraphicContext> DX11RHI::getContext()
{
	// TODO: each need to have own context
	return _deferredContext;
}

ID3DBlob* DX11RHI::compileShader(const wchar_t* filename, const char* entryPoint, const char* profile)
{
	uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* blob{nullptr};
	ID3DBlob* err{nullptr};
	D3DCompileFromFile(filename, nullptr, nullptr, entryPoint, profile, compileFlags, 0, &blob, &err);
	if (err != nullptr)
	{
		char mbsFilename[1024];
		std::wcstombs(mbsFilename, filename, wcslen(filename));

		size_t sz = err->GetBufferSize();
		char* msg = new char[sz + 1];
		msg[sz] = '\0';
		memcpy(msg, err->GetBufferPointer(), sz);
		PRINT("failed to compile vertex shader %s, error : %s", mbsFilename, msg);
		delete[] msg;
	}

	return blob;
}

ID3DBlob* DX11RHI::compileVertexShader(const wchar_t* filename, const char* entryPoint)
{
	return compileShader(filename, entryPoint, "vs_5_0");
}

ID3DBlob* DX11RHI::compilePixelShader(const wchar_t* filename, const char* entryPoint)
{
	return compileShader(filename, entryPoint, "ps_5_0");
}

void DX11RHI::submit()
{
	ID3D11CommandList* cmdlist;
	_deferredContext->finishCommandList(false, &cmdlist);
	_immediateContext->executeCommandList(cmdlist, false);
	cmdlist->Release();
}


DX11RHI::DX11RHI()
{
}


DX11RHI::~DX11RHI()
{
	_defaultRasterizerState->Release();
	_defaultDepthStencilState->Release();
}
