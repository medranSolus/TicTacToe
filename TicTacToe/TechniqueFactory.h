#pragma once
#include "Technique.h"
#include "Visuals.h"

namespace GFX::Pipeline
{
	class TechniqueFactory
	{
	public:
		TechniqueFactory() = delete;

		static Technique MakeLambertian(Graphics& gfx, RenderGraph& graph, std::shared_ptr<Visual::Material> material);
		static Technique MakeOutlineBlur(Graphics& gfx, RenderGraph& graph,
			const std::string& name, std::shared_ptr<Data::VertexLayout> layout);
	};
}