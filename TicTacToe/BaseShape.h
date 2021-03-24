#pragma once
#include "GfxResources.h"
#include "JobData.h"

namespace GFX::Shape
{
	class BaseShape : public virtual Pipeline::JobData
	{
		GfxResPtr<Resource::IndexBuffer> indexBuffer;
		GfxResPtr<Resource::VertexBuffer> vertexBuffer;
		GfxResPtr<Resource::Topology> topology;
		bool isOutline = false;

	protected:
		BaseShape(Graphics& gfx, GfxResPtr<Resource::IndexBuffer>&& indexBuffer = {},
			GfxResPtr<Resource::VertexBuffer>&& vertexBuffer = {});
		BaseShape(const BaseShape&) = delete;
		BaseShape(BaseShape&&) = default;
		BaseShape& operator=(const BaseShape&) = delete;
		BaseShape& operator=(BaseShape&&) = default;
		virtual ~BaseShape() = default;

		inline void SetIndexBuffer(GfxResPtr<Resource::IndexBuffer>&& index) noexcept { indexBuffer = std::move(index); }
		inline void SetVertexBuffer(GfxResPtr<Resource::VertexBuffer>&& vertex) noexcept { vertexBuffer = std::move(vertex); }

	public:
		constexpr bool IsOutline() const noexcept { return isOutline; }
		inline UINT GetIndexCount() const noexcept override { return indexBuffer->GetCount(); }

		void Bind(Graphics& gfx) override;
		void SetOutline() noexcept override;
		void DisableOutline() noexcept override;
	};
}