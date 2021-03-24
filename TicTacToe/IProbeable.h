#pragma once
#include "BaseProbe.h"

namespace GFX::Probe
{
	class IProbeable
	{
	public:
		virtual ~IProbeable() = default;

		virtual bool Accept(Graphics& gfx, BaseProbe& probe) noexcept = 0;
	};
}