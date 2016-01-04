#include "stdafx.h"
#include "DX11PixelShader.h"


DX11PixelShader::DX11PixelShader(const wchar_t* filename, const char* entryPoint)
	: DX11Shader{nullptr}
{
	_binaryData = RHI::getInst().compilePixelShader(filename, entryPoint);
	_pixelShader = RHI::getInst().createPixelShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11PixelShader::DX11PixelShader(const void* bytecode, uint32_t bytecodeLength)
	: DX11Shader{nullptr}
{
	D3DCreateBlob(bytecodeLength, &_binaryData);
	std::memcpy(_binaryData->GetBufferPointer(), bytecode, bytecodeLength);

	_pixelShader = RHI::getInst().createPixelShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11PixelShader::~DX11PixelShader()
{
	RHI::getInst().destroyBlob(_binaryData);
	RHI::getInst().destroyPixelShader(_pixelShader);
}
