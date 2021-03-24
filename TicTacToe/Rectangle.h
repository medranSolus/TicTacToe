#pragma once
#include "BaseShape.h"
#include "Object.h"

namespace GFX::Shape
{
	class Rectangle : public BaseShape, public Object
	{
		float width;
		float height;

	public:
		Rectangle(Graphics& gfx, Pipeline::RenderGraph& graph, const DirectX::XMFLOAT3& position,
			const std::string& name, const std::string& path, float width, float height);
		Rectangle(Rectangle&&) = default;
		Rectangle& operator=(Rectangle&&) = default;
		virtual ~Rectangle() = default;

		inline const std::string& GetName() const noexcept override { return name; }
		inline bool Accept(Graphics& gfx, Probe::BaseProbe& probe) noexcept override { return Object::Accept(gfx, probe) || BaseShape::Accept(gfx, probe); }

		void ChangeTexture(Graphics& gfx, const std::string& path);
		void UpdateTransformMatrix() noexcept override;
	};
}