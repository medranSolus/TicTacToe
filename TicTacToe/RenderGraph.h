#pragma once
#include "QueuePass.h"

namespace GFX::Pipeline
{
	class RenderGraph
	{
		std::vector<std::unique_ptr<RenderPass::Base::BasePass>> passes;
		std::vector<std::unique_ptr<RenderPass::Base::Sink>> globalSinks;
		std::vector<std::unique_ptr<RenderPass::Base::Source>> globalSources;
		GfxResPtr<Resource::RenderTarget> backbuffer;
		GfxResPtr<Resource::DepthStencil> depthStencil;
		bool finalized = false;

		void LinkSinks(RenderPass::Base::BasePass& pass);
		void LinkGlobalSinks();

	protected:
		inline void AddGlobalSink(std::unique_ptr<RenderPass::Base::Sink> sink) { globalSinks.emplace_back(std::move(sink)); }
		inline void AddGlobalSource(std::unique_ptr<RenderPass::Base::Source> source) { globalSources.emplace_back(std::move(source)); }

		RenderPass::Base::BasePass& FindPass(const std::string& name);
		void AppendPass(std::unique_ptr<RenderPass::Base::BasePass> pass);
		void SetSinkSource(const std::string& sink, const std::string& source);
		void Finalize();

	public:
		RenderGraph(Graphics& gfx);
		RenderGraph(const RenderGraph&) = default;
		RenderGraph& operator=(const RenderGraph&) = default;
		virtual ~RenderGraph() = default;

		RenderPass::Base::QueuePass& GetRenderQueue(const std::string& passName);

		void Execute(Graphics& gfx);
		void Reset() noexcept(!IS_DEBUG);
	};
}