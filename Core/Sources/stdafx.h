// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "targetver.h"

#include <windows.h>

// DirectX
#include <d3d11.h>
#include <dxgi1_3.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <shellapi.h>

// c++ std lib
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <cstdlib>

#include "Utils/utFuncs.h"
#include "Utils/utDefines.h"
#include "Utils/Nonmovable.h"
#include "Utils/Noncopyable.h"
#include "Utils/Heaponly.h"

#include "../Shaders/HLSLCommon.h"

#include "RHI.h"

// TODO: reference additional headers your program requires here
