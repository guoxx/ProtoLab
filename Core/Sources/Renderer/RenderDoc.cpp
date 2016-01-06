#include "stdafx.h"
#include "RenderDoc.h"
#include <RenderDoc/renderdoc_app.h>

static RENDERDOC_API_1_0_0* s_renderdoc{nullptr};

void RenderDoc::initialize()
{
	s_renderdoc = new RENDERDOC_API_1_0_0{};

	HMODULE renderdoc = LoadLibrary(TEXT("renderdoc.dll"));
	pRENDERDOC_GetAPI renderdocGetApiFunc = (pRENDERDOC_GetAPI)GetProcAddress(renderdoc, "RENDERDOC_GetAPI");
	renderdocGetApiFunc(eRENDERDOC_API_Version_1_0_0, reinterpret_cast<void**>(&s_renderdoc));

	s_renderdoc->SetLogFilePathTemplate("renderdoc/proto_lab");
#ifdef _DEBUG
	s_renderdoc->SetCaptureOptionU32(eRENDERDOC_Option_DebugDeviceMode, 1);
	s_renderdoc->SetCaptureOptionU32(eRENDERDOC_Option_DebugOutputMute, 0);
#endif
	
	RENDERDOC_InputButton k = eRENDERDOC_Key_C;
	s_renderdoc->SetCaptureKeys(&k, 1);
}

void RenderDoc::finalize()
{
	delete s_renderdoc;
	s_renderdoc = nullptr;
}
