#pragma once
#include "IVisual.h"
#include "Texture.h"
#include "InputLayout.h"
#include "ConstBufferExCache.h"

namespace GFX::Visual
{
	class Material : public IVisual
	{
		GfxResPtr<Resource::Texture> texture;
		std::shared_ptr<Data::VertexLayout> vertexLayout = nullptr;

	public:
		Material(Graphics& gfx, const std::string& path);
		Material(const Material&) = default;
		Material& operator=(const Material&) = default;
		virtual ~Material() = default;

		inline GfxResPtr<Resource::Texture> GetTexture() noexcept { return texture; }
		inline void SetTexture(GfxResPtr<Resource::Texture> tex) noexcept { texture = tex; }
		inline std::shared_ptr<Data::VertexLayout> GerVertexLayout() noexcept { return vertexLayout; }

		void Bind(Graphics& gfx) override;
	};
}