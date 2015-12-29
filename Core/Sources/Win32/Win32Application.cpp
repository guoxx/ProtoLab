#include "stdafx.h"
#include "Win32Application.h"

#include <DirectXTK/Inc/Keyboard.h>
#include <DirectXTK/Inc/Mouse.h>

HWND Win32Application::m_hwnd = nullptr;

int Win32Application::Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow)
{
	// Parse the command line parameters
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	pSample->ParseCommandLineArgs(argv, argc);
	LocalFree(argv);

	// Initialize the window class.
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"DXSampleClass";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, static_cast<LONG>(pSample->GetWidth()), static_cast<LONG>(pSample->GetHeight()) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	m_hwnd = CreateWindow(
		windowClass.lpszClassName,
		pSample->GetTitle(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,		// We have no parent window.
		nullptr,		// We aren't using menus.
		hInstance,
		pSample);

	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	pSample->OnInit();

	// Initialize keyboard and mouse
	new DirectX::Keyboard();
	new DirectX::Mouse();
	DirectX::Keyboard::Get();
	DirectX::Mouse::Get().SetWindow(m_hwnd);

	ShowWindow(m_hwnd, nCmdShow);

	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pSample->OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	return static_cast<char>(msg.wParam);
}

// Main message handler for the sample.
LRESULT CALLBACK Win32Application::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DXSample* pSample = reinterpret_cast<DXSample*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (message)
	{
	case WM_CREATE:
		{
			// Save the DXSample* passed in to CreateWindow.
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}
		return 0;

     case WM_ACTIVATEAPP:
         DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
         DirectX::Mouse::ProcessMessage(message, wParam, lParam);
         break;

     case WM_KEYDOWN:
     case WM_SYSKEYDOWN:
     case WM_KEYUP:
     case WM_SYSKEYUP:
         DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
         break;

     case WM_INPUT:
     case WM_MOUSEMOVE:
     case WM_LBUTTONDOWN:
     case WM_LBUTTONUP:
     case WM_RBUTTONDOWN:
     case WM_RBUTTONUP:
     case WM_MBUTTONDOWN:
     case WM_MBUTTONUP:
     case WM_MOUSEWHEEL:
     case WM_XBUTTONDOWN:
     case WM_XBUTTONUP:
     case WM_MOUSEHOVER:
         DirectX::Mouse::ProcessMessage(message, wParam, lParam);
         break;

	case WM_PAINT:
		if (pSample)
		{
			pSample->OnUpdate();
			pSample->OnRender();
			pSample->OnPresent();
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}
