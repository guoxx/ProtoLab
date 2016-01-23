#include "stdafx.h"
#include "DX11PixelShader.h"


DX11PixelShader::DX11PixelShader(const wchar_t* filename, const char* entryPoint)
	: DX11Shader{nullptr}
{
	_binaryData = RHI::getInstance().compilePixelShader(filename, entryPoint);
	_pixelShader = RHI::getInstance().getDevice()->createPixelShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11PixelShader::DX11PixelShader(const void* bytecode, std::size_t bytecodeLength)
	: DX11Shader{nullptr}
{
	D3DCreateBlob(bytecodeLength, &_binaryData);
	std::memcpy(_binaryData->GetBufferPointer(), bytecode, bytecodeLength);

	_pixelShader = RHI::getInstance().getDevice()->createPixelShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11PixelShader::~DX11PixelShader()
{
}

ComPtr<ID3D11PixelShader> DX11PixelShader::getShader() const
{
	return _pixelShader;
}
