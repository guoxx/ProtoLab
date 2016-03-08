#pragma once

#include "DX11Shader.h"

class DX11GeometryShader : public DX11Shader
{
public:
	DX11GeometryShader(const wchar_t* filename, const char* entryPoint);
	DX11GeometryShader(const void* bytecode, std::size_t bytecodeLength);
	virtual ~DX11GeometryShader();

	ComPtr<ID3D11GeometryShader> getShader() const;

private:
	ComPtr<ID3D11GeometryShader> _geometryShader;
};
