#include "HorizontalBlurPass.h"
#include "RenderPassesBase.h"
#include "PipelineResources.h"
#include "GfxResources.h"

namespace GFX::Pipeline::RenderPass
{
	HorizontalBlurPass::HorizontalBlurPass(Graphics& gfx, const std::string& name, unsigned int width, unsigned int height)
		: FullscreenPass(gfx, name)
	{
		AddBindableSink<GFX::Resource::ConstBufferExPixelCache>("kernel");
		AddBindableSink<Resource::RenderTarget>("blurTarget");
		RegisterSink(Base::SinkDirectBindable<GFX::Resource::ConstBufferExPixelCache>::Make("direction", direction));

		renderTarget = GfxResPtr<Resource::RenderTargetShaderInput>(gfx, width, height, 0U);
		RegisterSource(Base::SourceDirectBindable<Resource::RenderTarget>::Make("halfTarget", renderTarget));

		AddBind(GFX::Resource::PixelShader::Get(gfx, "BlurPS"));
		AddBind(GFX::Resource::Blender::Get(gfx, GFX::Resource::Blender::Type::None));
	}

	void HorizontalBlurPass::Execute(Graphics& gfx)
	{
		direction->GetBuffer()["vertical"] = false;
		direction->Bind(gfx);
		FullscreenPass::Execute(gfx);
	}
}