#include "BaseShape.h"

namespace GFX::Shape
{
	BaseShape::BaseShape(Graphics& gfx, GfxResPtr<Resource::IndexBuffer>&& indexBuffer, GfxResPtr<Resource::VertexBuffer>&& vertexBuffer)
		: indexBuffer(std::move(indexBuffer)), vertexBuffer(std::move(vertexBuffer))
	{
		topology = Resource::Topology::Get(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void BaseShape::Bind(Graphics& gfx)
	{
		indexBuffer->Bind(gfx);
		vertexBuffer->Bind(gfx);
		topology->Bind(gfx);
	}

	void BaseShape::SetOutline() noexcept
	{
		if (auto technique = GetTechnique("Outline"))
		{
			technique->Activate();
			isOutline = true;
		}
	}

	void BaseShape::DisableOutline() noexcept
	{
		if (auto technique = GetTechnique("Outline"))
		{
			technique->Deactivate();
			isOutline = false;
		}
	}
}