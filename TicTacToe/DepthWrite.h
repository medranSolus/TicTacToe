#pragma once
#include "IVisual.h"

namespace GFX::Visual
{
	class DepthWrite : public IVisual
	{
	public:
		DepthWrite(Graphics& gfx, std::shared_ptr<Data::VertexLayout> vertexLayout);
		virtual ~DepthWrite() = default;
	};
}