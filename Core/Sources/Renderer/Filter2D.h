#pragma once

class Filter2D
{
public:
	Filter2D(std::shared_ptr<DX11VertexShader> vs, std::shared_ptr<DX11PixelShader> ps);
	Filter2D(std::shared_ptr<DX11VertexShader> vs, std::shared_ptr<DX11GeometryShader> gs, std::shared_ptr<DX11PixelShader> ps);
	~Filter2D();

	void apply(std::shared_ptr<DX11RenderTarget> dest);
	void apply(std::shared_ptr<DX11RenderTarget> source, std::shared_ptr<DX11RenderTarget> dest);

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11SamplerState> _textureSamp;

	std::shared_ptr<DX11VertexShader> _vertexShader;
	std::shared_ptr<DX11GeometryShader> _geomShader;
	std::shared_ptr<DX11PixelShader> _pixelShader;
};

