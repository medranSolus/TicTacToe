#include "OutlineDrawBlurPass.h"
#include "RenderPassesBase.h"
#include "PipelineResources.h"
#include "GfxResources.h"

namespace GFX::Pipeline::RenderPass
{
	OutlineDrawBlurPass::OutlineDrawBlurPass(Graphics& gfx, const std::string& name, unsigned int width, unsigned int height)
		: QueuePass(name)
	{
		renderTarget = GfxResPtr<Resource::RenderTargetShaderInput>(gfx, width, height, 0U);
		RegisterSource(Base::SourceDirectBindable<Resource::RenderTarget>::Make("blurTarget", renderTarget));

		AddBind(GFX::Resource::PixelShader::Get(gfx, "SolidPS"));
		AddBind(GFX::Resource::DepthStencilState::Get(gfx, GFX::Resource::DepthStencilState::StencilMode::Off));
		AddBind(GFX::Resource::Blender::Get(gfx, GFX::Resource::Blender::Type::None));

		Data::CBuffer::DCBLayout layout;
		layout.Add(DCBElementType::Color3, "solidColor");
		Data::CBuffer::DynamicCBuffer buffer(std::move(layout));
		buffer["solidColor"] = Data::ColorFloat3(1.0f, 1.0f, 0.0f);
		outlineColorBuffer = GfxResPtr<GFX::Resource::ConstBufferExPixelCache>(gfx, name, buffer, 0U);
	}

	void OutlineDrawBlurPass::ShowWindow() noexcept
	{
		if (auto color = outlineColorBuffer->GetBufferConst()["solidColor"]; color.Exists())
		{
			Data::ColorFloat3 c = color;
			if (ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&c), ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float))
				outlineColorBuffer->GetBuffer()["solidColor"] = c;
		}
	}

	void OutlineDrawBlurPass::Execute(Graphics& gfx)
	{
		renderTarget->Clear(gfx);
		outlineColorBuffer->Bind(gfx);
		QueuePass::Execute(gfx);
	}
}