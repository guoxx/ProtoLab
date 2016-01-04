#pragma once

class DX11Shader
{
public:
	DX11Shader(ID3DBlob* bin);
	virtual ~DX11Shader();

	ID3DBlob* getBinaryData() const;

protected:
	ID3DBlob* _binaryData{nullptr};
};

