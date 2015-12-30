#include "stdafx.h"
#include "ForwardRenderer.h"
#include "../Objects/Camera.h"
#include "../Objects/Scene.h"
#include "../Objects/Model.h"
// TODO: remove this
#include "../Mesh/Mesh.h"


ForwardRenderer::ForwardRenderer()
{
}


ForwardRenderer::~ForwardRenderer()
{
}

void ForwardRenderer::render(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene)
{
	RHI::setDefaultRHIStates();	

	ID3D11RenderTargetView* rtvs[] = {RHI::getBackbufferRTV()};
	RHI::_context->OMSetRenderTargets(1, rtvs, nullptr);

	auto models = scene->getModels();
	for (auto model :models)
	{
		auto mesh = model->getMesh();
		mesh->draw(camera.get());
	}

}
