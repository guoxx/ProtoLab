#pragma once

#include "DX11Device.h"
#include "DX11RenderTarget.h"
#include "DX11DepthStencilRenderTarget.h"
#include "DX11GraphicContext.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"
#include "DX11RenderStateSet.h"


// TODO:
// 2. use draw command list to cache each draw call so that we can flush it on another thread
// 3. deferred context
class DX11RHI : public Noncopyable, Nonmovable
{
public:
	enum class RHI_CLEAR_FLAG
	{
		RHI_CLEAR_DEPTH = D3D11_CLEAR_DEPTH,
		RHI_CLEAR_STENCIL = D3D11_CLEAR_STENCIL,
		RHI_CLEAR_DEPTH_STENCIL = RHI_CLEAR_DEPTH | RHI_CLEAR_STENCIL
	};

	static DX11RHI& getInst();

	void initialize();
	void finalize();

	std::shared_ptr<DX11Device> getDevice() const;
	std::shared_ptr<DX11GraphicContext> getContext() const;
	std::shared_ptr<DX11RenderStateSet> getRenderStateSet() const;

	// TODO: move to shader compiler class
	// shader compilation
	ComPtr<ID3DBlob> compileShader(const wchar_t* filename, const char* entryPoint, const char* profile);
	ComPtr<ID3DBlob> compileVertexShader(const wchar_t* filename, const char* entryPoint);
	ComPtr<ID3DBlob> compilePixelShader(const wchar_t* filename, const char* entryPoint);

	void submit();

private:
	std::shared_ptr<DX11Device>			_device{nullptr};
	std::shared_ptr<DX11GraphicContext>	_immediateContext{nullptr};
	std::shared_ptr<DX11GraphicContext>	_deferredContext{nullptr};
	std::shared_ptr<DX11RenderStateSet> _renderStateSet{nullptr};

private:
	DX11RHI();
	~DX11RHI();
};
