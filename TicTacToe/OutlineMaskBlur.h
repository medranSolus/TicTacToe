#pragma once
#include "IVisual.h"
#include "ConstBufferExCache.h"

namespace GFX::Visual
{
	class OutlineMaskBlur : public IVisual
	{
	public:
		OutlineMaskBlur(Graphics& gfx, const std::string& tag, std::shared_ptr<Data::VertexLayout> vertexLayout);
		virtual ~OutlineMaskBlur() = default;
	};
}