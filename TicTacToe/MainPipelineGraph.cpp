#include "MainPipelineGraph.h"
#include "RenderPasses.h"
#include "GfxResources.h"
#include "DialogWindow.h"
#include "Math.h"

#define MakePass(pass, ...) std::make_unique<RenderPass::pass>(__VA_ARGS__)

namespace GFX::Pipeline
{
	inline void MainPipelineGraph::SetupSamplers(Graphics& gfx)
	{
		samplers.emplace_back(gfx, GFX::Resource::Sampler::Type::Anisotropic, GFX::Resource::Sampler::CoordType::Wrap, 0U);
		samplers.emplace_back(gfx, GFX::Resource::Sampler::Type::Linear, GFX::Resource::Sampler::CoordType::Reflect, 1U);
		samplers.emplace_back(gfx, GFX::Resource::Sampler::Type::Point, GFX::Resource::Sampler::CoordType::Wrap, 2U);
		for (auto& sampler : samplers)
			sampler.Bind(gfx);
	}

	void MainPipelineGraph::SetKernel() noexcept(!IS_DEBUG)
	{
		assert(radius <= MAX_RADIUS);
		auto& buffer = kernel->GetBuffer();
		buffer["radius"] = radius;
		float sum = 0.0f;
		for (int i = 0; i <= radius; ++i)
		{
			const float g = Math::Gauss(static_cast<float>(i), sigma);
			sum += g;
			buffer["coefficients"][i] = g;
		}
		for (int i = 0; i <= radius; ++i)
			buffer["coefficients"][i] = static_cast<float>(buffer["coefficients"][i]) / sum;
	}

	MainPipelineGraph::MainPipelineGraph(Graphics& gfx, float sigma, int radius)
		: RenderGraph(gfx), radius(radius), sigma(sigma)
	{
		SetupSamplers(gfx);

#pragma region CBuffers setup
		// Setup blur cbuffers
		Data::CBuffer::DCBLayout kernelLayout;
		kernelLayout.Add(DCBElementType::SInteger, "radius");
		kernelLayout.Add(DCBElementType::UInteger, "width");
		kernelLayout.Add(DCBElementType::UInteger, "height");
		kernelLayout.Add(DCBElementType::Array, "coefficients");
		kernelLayout["coefficients"].InitArray(DCBElementType::Float, MAX_RADIUS + 1);
		Data::CBuffer::DynamicCBuffer kernelBuffer(std::move(kernelLayout));
		kernelBuffer["width"] = gfx.GetWidth();
		kernelBuffer["height"] = gfx.GetHeight();
		kernel = GfxResPtr<GFX::Resource::ConstBufferExPixelCache>(gfx, "$kernelBuffer", kernelBuffer, 1U);
		SetKernel();
		AddGlobalSource(RenderPass::Base::SourceDirectBindable<GFX::Resource::ConstBufferExPixelCache>::Make("kernel", kernel));

		Data::CBuffer::DCBLayout directionLayout;
		directionLayout.Add(DCBElementType::Bool, "vertical");
		Data::CBuffer::DynamicCBuffer directionBuffer(std::move(directionLayout));
		directionBuffer["vertical"] = true;
		blurDirection = GfxResPtr<GFX::Resource::ConstBufferExPixelCache>(gfx, "$blurDirection", directionBuffer, 0U);
		AddGlobalSource(RenderPass::Base::SourceDirectBindable<GFX::Resource::ConstBufferExPixelCache>::Make("blurDirection", blurDirection));
#pragma endregion

		// Setup all passes
		{
			auto pass = MakePass(ClearBufferPass, "clearDS");
			pass->SetSinkLinkage("buffer", "$.depthStencil");
			AppendPass(std::move(pass));
		}
		{
			auto pass = MakePass(ClearBufferPass, "clearRT");
			pass->SetSinkLinkage("buffer", "$.backbuffer");
			AppendPass(std::move(pass));
		}
		{
			auto pass = MakePass(LambertianPass, gfx, "lambertian");
			pass->SetSinkLinkage("renderTarget", "clearRT.buffer");
			AppendPass(std::move(pass));
		}
#pragma region Outline effect
		{
			auto pass = MakePass(OutlineGenerationPass, gfx, "outlineGeneration");
			pass->SetSinkLinkage("depthStencil", "clearDS.buffer");
			AppendPass(std::move(pass));
		}
		const unsigned int blurTargetWidth = gfx.GetWidth() / 4;
		const unsigned int blurTargetHeight = gfx.GetHeight() / 4;
		AppendPass(MakePass(OutlineDrawBlurPass, gfx, "outlineDrawBlur", blurTargetWidth, blurTargetHeight));
		{
			auto pass = MakePass(HorizontalBlurPass, gfx, "horizontalBlur", blurTargetWidth, blurTargetHeight);
			pass->SetSinkLinkage("blurTarget", "outlineDrawBlur.blurTarget");
			pass->SetSinkLinkage("kernel", "$.kernel");
			pass->SetSinkLinkage("direction", "$.blurDirection");
			AppendPass(std::move(pass));
		}
		{
			auto pass = MakePass(VerticalBlurPass, gfx, "verticalBlur");
			pass->SetSinkLinkage("halfTarget", "horizontalBlur.halfTarget");
			pass->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
			pass->SetSinkLinkage("depthStencil", "outlineGeneration.depthStencil");
			pass->SetSinkLinkage("kernel", "$.kernel");
			pass->SetSinkLinkage("direction", "$.blurDirection");
			AppendPass(std::move(pass));
		}
#pragma endregion
		SetSinkSource("backbuffer", "verticalBlur.renderTarget");
		Finalize();
	}

	void MainPipelineGraph::SetKernel(int radius, float sigma) noexcept(!IS_DEBUG)
	{
		this->sigma = sigma;
		this->radius = radius;
		SetKernel();
	}
}