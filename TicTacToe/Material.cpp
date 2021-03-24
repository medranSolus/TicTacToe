#include "Material.h"
#include "GfxResources.h"
#include "Math.h"

namespace GFX::Visual
{
	Material::Material(Graphics& gfx, const std::string& path)
	{
		vertexLayout = std::make_shared<Data::VertexLayout>();
		vertexLayout->Append(VertexAttribute::Texture2D);
		texture = Resource::Texture::Get(gfx, path, 0U, true);

		AddBind(Resource::PixelShader::Get(gfx, "TexPS"));
		auto vertexShader = Resource::VertexShader::Get(gfx, "TexVS");
		AddBind(Resource::InputLayout::Get(gfx, vertexLayout, vertexShader));
		AddBind(std::move(vertexShader));
	}

	void Material::Bind(Graphics& gfx)
	{
		IVisual::Bind(gfx);
		texture->Bind(gfx);
	}
}