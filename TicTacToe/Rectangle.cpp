#include "Rectangle.h"
#include "Square.h"
#include "TechniqueFactory.h"

namespace GFX::Shape
{
	Rectangle::Rectangle(Graphics& gfx, Pipeline::RenderGraph& graph, const DirectX::XMFLOAT3& position,
		const std::string& name, const std::string& path, float width, float height)
		: BaseShape(gfx), Object(position, name), width(width), height(height)
	{
		std::string typeName = Primitive::Square::GetName();
		if (Resource::VertexBuffer::NotStored(typeName) && Resource::IndexBuffer::NotStored(typeName))
		{
			auto list = Primitive::Square::Make({ VertexAttribute::Texture2D });
			list.vertices[0].Get<VertexAttribute::Texture2D>() = { 0.0f, 1.0f };
			list.vertices[1].Get<VertexAttribute::Texture2D>() = { 0.0f, 0.0f };
			list.vertices[2].Get<VertexAttribute::Texture2D>() = { 1.0f, 0.0f };
			list.vertices[3].Get<VertexAttribute::Texture2D>() = { 1.0f, 1.0f };
			SetVertexBuffer(Resource::VertexBuffer::Get(gfx, typeName, list.vertices));
			SetIndexBuffer(Resource::IndexBuffer::Get(gfx, typeName, list.indices));
		}
		else
		{
			Primitive::IndexedTriangleList list;
			SetVertexBuffer(Resource::VertexBuffer::Get(gfx, typeName, list.vertices));
			SetIndexBuffer(Resource::IndexBuffer::Get(gfx, typeName, list.indices));
		}

		std::vector<Pipeline::Technique> techniques;
		techniques.reserve(3);
		auto material = std::make_shared<Visual::Material>(gfx, path);
		auto vertexLayout = material->GerVertexLayout();

		techniques.emplace_back(Pipeline::TechniqueFactory::MakeLambertian(gfx, graph, std::move(material)));
		techniques.emplace_back(Pipeline::TechniqueFactory::MakeOutlineBlur(gfx, graph, name, std::move(vertexLayout)));
		SetTechniques(gfx, std::move(techniques), *this);

		UpdateTransformMatrix();
	}

	void Rectangle::ChangeTexture(Graphics& gfx, const std::string& path)
	{
		static_cast<Visual::Material&>(*GetTechnique("Lambertian")->GetStep(0).GetData()).SetTexture(Resource::Texture::Get(gfx, path, 0U, true));
	}

	void Rectangle::UpdateTransformMatrix() noexcept
	{
		DirectX::XMStoreFloat4x4(transform.get(),
			DirectX::XMMatrixScaling(width, height, 1.0f) *
			CreateTransformMatrix());
	}
}