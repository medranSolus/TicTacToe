#include "IndexedTriangleList.h"

namespace GFX::Primitive
{
	IndexedTriangleList::IndexedTriangleList(Data::VertexBufferData verticesIn, std::vector<unsigned int> indicesIn) noexcept(!IS_DEBUG)
		: vertices(std::move(verticesIn)), indices(std::move(indicesIn))
	{
		assert(vertices.Size() > 2);
		assert(indices.size() % 3 == 0);
	}

	void IndexedTriangleList::Transform(DirectX::FXMMATRIX matrix) noexcept(!IS_DEBUG)
	{
		for (size_t i = 0, size = vertices.Size(); i < size; ++i)
			DirectX::XMStoreFloat3(&vertices[i].Get<VertexAttribute::Position3D>(),
				DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&vertices[i].Get<VertexAttribute::Position3D>()), matrix));
	}
}