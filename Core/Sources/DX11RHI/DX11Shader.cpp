#include "stdafx.h"
#include "DX11Shader.h"


DX11Shader::DX11Shader(ComPtr<ID3DBlob> bin)
	: _binaryData{bin}
{
}

DX11Shader::~DX11Shader()
{
}

ComPtr<ID3DBlob> DX11Shader::getBinaryData() const
{
	return _binaryData;
}
