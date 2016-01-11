#pragma once

#include <DirectXTK/Inc/CommonStates.h>

class DX11RenderStateSet : public DirectX::CommonStates
{
public:
	explicit DX11RenderStateSet(ComPtr<ID3D11Device> device);
	virtual ~DX11RenderStateSet();

private:
	// explicit keep a reference to device
	ComPtr<ID3D11Device> _device;
};

