#pragma once

namespace GFX::Data::CBuffer
{
	struct Transform
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX transformViewProjection;
	};
}