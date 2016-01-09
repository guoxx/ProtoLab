#pragma once

#include <DirectXTK/Inc/CommonStates.h>

class DX11RenderStateSet : public DirectX::CommonStates
{
public:
	explicit DX11RenderStateSet(ID3D11Device* device);
	virtual ~DX11RenderStateSet();
};

