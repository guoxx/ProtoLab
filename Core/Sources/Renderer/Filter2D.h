#pragma once

class Filter2D
{
public:
	Filter2D(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexDecl, ID3D11PixelShader* pixelShader);
	~Filter2D();

	void apply();

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11InputLayout* _vertexDecl;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
};

