#include "stdafx.h"
#include "Win32/D3D11HelloTriangle.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	D3D11HelloTriangle sample(1280, 720, L"D3D12 Hello Triangle");
	return Win32Application::Run(&sample, hInstance, nCmdShow);
}