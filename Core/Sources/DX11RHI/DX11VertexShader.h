#pragma once

#include "DX11Shader.h"

class DX11VertexShader : public DX11Shader
{
public:
	DX11VertexShader(const wchar_t* filename, const char* entryPoint);
	DX11VertexShader(const void* bytecode, std::size_t bytecodeLength);
	virtual ~DX11VertexShader();

	ID3D11VertexShader* getShader() const;

private:
	ID3D11VertexShader* _vertexShader;
};
