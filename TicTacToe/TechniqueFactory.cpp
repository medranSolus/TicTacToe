#include "TechniqueFactory.h"

namespace GFX::Pipeline
{
	Technique TechniqueFactory::MakeLambertian(Graphics& gfx, RenderGraph& graph, std::shared_ptr<Visual::Material> material)
	{
		Technique technique("Lambertian");
		technique.AddStep({ graph, "lambertian", material });
		return std::move(technique);
	}

	Technique TechniqueFactory::MakeOutlineBlur(Graphics& gfx, RenderGraph& graph,
		const std::string& name, std::shared_ptr<Data::VertexLayout> layout)
	{
		Technique technique("Blur Outline", false);
		technique.AddStep({ graph, "outlineGeneration", std::make_shared<Visual::DepthWrite>(gfx, layout) });
		technique.AddStep({ graph, "outlineDrawBlur", std::make_shared<Visual::OutlineMaskBlur>(gfx, name + "OutlineBlur", layout) });
		return std::move(technique);
	}
}