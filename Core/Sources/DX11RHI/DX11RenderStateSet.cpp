#include "stdafx.h"
#include "DX11RenderStateSet.h"


DX11RenderStateSet::DX11RenderStateSet(ComPtr<ID3D11Device> device)
	: CommonStates{device.Get()}
	, _device{device}
{
}

DX11RenderStateSet::~DX11RenderStateSet()
{
}
