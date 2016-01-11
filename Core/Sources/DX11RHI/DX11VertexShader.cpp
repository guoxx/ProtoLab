#include "stdafx.h"
#include "DX11VertexShader.h"


DX11VertexShader::DX11VertexShader(const wchar_t* filename, const char* entryPoint)
	: DX11Shader{nullptr}
{
	_binaryData = RHI::getInst().compileVertexShader(filename, entryPoint);
	_vertexShader = RHI::getInst().getDevice()->createVertexShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11VertexShader::DX11VertexShader(const void* bytecode, std::size_t bytecodeLength)
	: DX11Shader{nullptr}
{
	D3DCreateBlob(bytecodeLength, _binaryData.GetAddressOf());
	std::memcpy(_binaryData->GetBufferPointer(), bytecode, bytecodeLength);

	_vertexShader = RHI::getInst().getDevice()->createVertexShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11VertexShader::~DX11VertexShader()
{
}

ComPtr<ID3D11VertexShader> DX11VertexShader::getShader() const
{
	return _vertexShader;
}
