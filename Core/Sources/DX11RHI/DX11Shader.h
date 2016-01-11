#pragma once

class DX11Shader
{
public:
	DX11Shader(ComPtr<ID3DBlob> bin);
	virtual ~DX11Shader();

	ComPtr<ID3DBlob> getBinaryData() const;

protected:
	ComPtr<ID3DBlob> _binaryData{nullptr};
};

