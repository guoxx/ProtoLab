#pragma once

#include "DX11Shader.h"

class DX11GeometryShader : public DX11Shader
{
public:
	DX11GeometryShader(const wchar_t* filename, const char* entryPoint);
	DX11GeometryShader(const void* bytecode, std::size_t bytecodeLength);
	DX11GeometryShader(const void* bytecode,
						std::size_t bytecodeLength,
						const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
						uint32_t numEntries,
						const uint32_t *pBufferStrides,
						uint32_t numStrides,
						uint32_t rasterizedStream);
	virtual ~DX11GeometryShader();

	ComPtr<ID3D11GeometryShader> getShader() const;

private:
	ComPtr<ID3D11GeometryShader> _geometryShader;
};
