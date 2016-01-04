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
}

void RenderDoc::finalize()
{
	delete s_renderdoc;
	s_renderdoc = nullptr;
}
