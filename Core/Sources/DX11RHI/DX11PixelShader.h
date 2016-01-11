#pragma once

#include "DX11Shader.h"

class DX11PixelShader : public DX11Shader
{
public:
	DX11PixelShader(const wchar_t* filename, const char* entryPoint);
	DX11PixelShader(const void* bytecode, std::size_t bytecodeLength);
	virtual ~DX11PixelShader();

	ComPtr<ID3D11PixelShader> getShader() const;

private:
	ComPtr<ID3D11PixelShader> _pixelShader;
};
