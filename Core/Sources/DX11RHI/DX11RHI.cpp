#include "stdafx.h"
#include "DX11RHI.h"


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

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> immediateCtx;
	hr = D3D11CreateDevice(nullptr,
							D3D_DRIVER_TYPE_HARDWARE,
							nullptr,
							creationFlag,
							featureLevels,
							sizeof(featureLevels)/sizeof(featureLevels[0]),
							D3D11_SDK_VERSION,
							device.GetAddressOf(),
							nullptr,
							immediateCtx.GetAddressOf());
	CHECK(hr == S_OK);
	_device = std::make_shared<DX11Device>(device);

	_immediateContext = std::make_shared<DX11GraphicContext>(immediateCtx);

	ComPtr<ID3D11DeviceContext> deferredCtx;
	hr = device->CreateDeferredContext(0, deferredCtx.GetAddressOf());
	CHECK(hr == S_OK);
	_deferredContext = std::make_shared<DX11GraphicContext>(deferredCtx);

	_renderStateSet = std::make_shared<DX11RenderStateSet>(device.Get());

#ifdef _DEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	hr = device.As(&d3dDebug);
	if (hr == S_OK)
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		hr = d3dDebug.As(&d3dInfoQueue);
		CHECK(hr == S_OK);

		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true);
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

std::shared_ptr<DX11Device> DX11RHI::getDevice() const
{
	return _device;
}

std::shared_ptr<DX11GraphicContext> DX11RHI::getContext() const
{
	// TODO: each need to have own context
	return _deferredContext;
}

std::shared_ptr<DX11RenderStateSet> DX11RHI::getRenderStateSet() const
{
	return _renderStateSet;
}

ComPtr<ID3DBlob> DX11RHI::compileShader(const wchar_t* filename, const char* entryPoint, const char* profile)
{
	uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DBlob> err;
	D3DCompileFromFile(filename, nullptr, nullptr, entryPoint, profile, compileFlags, 0, blob.GetAddressOf(), err.GetAddressOf());
	if (err != nullptr)
	{
		char mbsFilename[1024];
		std::wcstombs(mbsFilename, filename, wcslen(filename) + 1);

		size_t sz = err->GetBufferSize();
		char* msg = new char[sz + 1];
		msg[sz] = '\0';
		memcpy(msg, err->GetBufferPointer(), sz);
		PRINT("failed to compile vertex shader %s, error : %s", mbsFilename, msg);
		delete[] msg;
	}

	return blob;
}

ComPtr<ID3DBlob> DX11RHI::compileVertexShader(const wchar_t* filename, const char* entryPoint)
{
	return compileShader(filename, entryPoint, "vs_5_0");
}

ComPtr<ID3DBlob> DX11RHI::compilePixelShader(const wchar_t* filename, const char* entryPoint)
{
	return compileShader(filename, entryPoint, "ps_5_0");
}

void DX11RHI::submit()
{
	ComPtr<ID3D11CommandList> cmdlist;
	_deferredContext->finishCommandList(false, cmdlist.GetAddressOf());
	_immediateContext->executeCommandList(cmdlist, false);
}


DX11RHI::DX11RHI()
{
}


DX11RHI::~DX11RHI()
{
}
