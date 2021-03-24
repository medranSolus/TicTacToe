#include "OutlineMaskBlur.h"
#include "GfxResources.h"

namespace GFX::Visual
{
	OutlineMaskBlur::OutlineMaskBlur(Graphics& gfx, const std::string& tag, std::shared_ptr<Data::VertexLayout> vertexLayout)
	{
		auto vertexShader = Resource::VertexShader::Get(gfx, "SolidVS");
		AddBind(Resource::InputLayout::Get(gfx, vertexLayout, vertexShader));
		AddBind(std::move(vertexShader));
	}
}