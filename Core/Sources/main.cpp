#include "stdafx.h"
#include "Win32/D3D11HelloTriangle.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	D3D11HelloTriangle sample(WIN_WIDTH, WIN_HEIGHT, L"D3D12 Hello Triangle");
	return Win32Application::Run(&sample, hInstance, nCmdShow);
}