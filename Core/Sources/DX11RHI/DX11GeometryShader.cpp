#include "stdafx.h"
#include "DX11GeometryShader.h"


DX11GeometryShader::DX11GeometryShader(const wchar_t* filename, const char* entryPoint)
	: DX11Shader{nullptr}
{
	_binaryData = RHI::getInstance().compileVertexShader(filename, entryPoint);
	_geometryShader = RHI::getInstance().getDevice()->createGeometryShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11GeometryShader::DX11GeometryShader(const void* bytecode, std::size_t bytecodeLength)
	: DX11Shader{nullptr}
{
	D3DCreateBlob(bytecodeLength, _binaryData.GetAddressOf());
	std::memcpy(_binaryData->GetBufferPointer(), bytecode, bytecodeLength);

	_geometryShader = RHI::getInstance().getDevice()->createGeometryShaderFromBytecodes(_binaryData->GetBufferPointer(), _binaryData->GetBufferSize());
}

DX11GeometryShader::~DX11GeometryShader()
{
}

ComPtr<ID3D11GeometryShader> DX11GeometryShader::getShader() const
{
	return _geometryShader;
}
