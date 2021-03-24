#include "LambertianPass.h"
#include "RenderPassesBase.h"
#include "PipelineResources.h"
#include "GfxResources.h"

namespace GFX::Pipeline::RenderPass
{
	LambertianPass::LambertianPass(Graphics& gfx, const std::string& name)
		: QueuePass(name)
	{
		RegisterSink(Base::SinkDirectBuffer<Resource::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(Base::SourceDirectBuffer<Resource::RenderTarget>::Make("renderTarget", renderTarget));

		AddBind(GFX::Resource::DepthStencilState::Get(gfx, GFX::Resource::DepthStencilState::StencilMode::Off));
		AddBind(GFX::Resource::Rasterizer::Get(gfx, D3D11_CULL_NONE));
		AddBind(GFX::Resource::Blender::Get(gfx, GFX::Resource::Blender::Type::None));
	}
}