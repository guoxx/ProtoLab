#include "stdafx.h"
#include "DX11RenderStateSet.h"


DX11RenderStateSet::DX11RenderStateSet(ID3D11Device* device)
	: CommonStates{device}
{
}

DX11RenderStateSet::~DX11RenderStateSet()
{
}
