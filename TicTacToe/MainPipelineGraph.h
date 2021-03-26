#pragma once
#include "RenderGraph.h"
#include "RenderTargetShaderInput.h"
#include "ConstBufferExCache.h"
#include "Sampler.h"

namespace GFX::Pipeline
{
	class MainPipelineGraph : public RenderGraph
	{
		static constexpr int MAX_RADIUS = 15;

		int radius;
		float sigma;
		std::vector<GFX::Resource::Sampler> samplers;
		GfxResPtr<GFX::Resource::ConstBufferExPixelCache> kernel;
		GfxResPtr<GFX::Resource::ConstBufferExPixelCache> blurDirection;

		inline void SetupSamplers(Graphics& gfx);
		void SetKernel() noexcept(!IS_DEBUG);

	public:
		MainPipelineGraph(Graphics& gfx, float sigma = 2.6f, int radius = MAX_RADIUS);
		virtual ~MainPipelineGraph() = default;

		void SetKernel(int radius, float sigma) noexcept(!IS_DEBUG);
	};
}