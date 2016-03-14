#pragma once

class DX11GPUMarker : public Heaponly, Noncopyable, Nonmovable
{
public:
	DX11GPUMarker(DX11GraphicContext* ctx, const wchar_t* name)
		: _ctx{ ctx }
	{
		_ctx->pushMarker(name);
	}

	~DX11GPUMarker()
	{
		_ctx->popMarker();
	}

private:
	DX11GraphicContext* _ctx;
};

#define GPU_MARKER(ctx, name) DX11GPUMarker _gpu_marker_##name{ ctx, L#name };