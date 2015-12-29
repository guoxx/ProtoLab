#pragma once

class Primitive
{
public:
	friend class Mesh;

	Primitive();
	~Primitive();

private:
	std::string				_name;
	uint32_t				_matIdx;
	ID3D11Buffer*			_positionBuffer{nullptr};
	ID3D11Buffer*			_normalBuffer{nullptr};
	ID3D11Buffer*			_texcoordBuffer{nullptr};
	ID3D11Buffer*			_indexBuffer{nullptr};
	ID3D11InputLayout*		_vertexDeclaration{nullptr};

	D3D_PRIMITIVE_TOPOLOGY	_topology;
	DXGI_FORMAT				_indicesFormat;
	uint32_t				_indicesCount;
};

