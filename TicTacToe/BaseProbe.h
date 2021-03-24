#pragma once
#include "DynamicCBuffer.h"

namespace GFX
{
	class Graphics;

	namespace Pipeline
	{
		class Technique;
	}
	namespace Shape
	{
		class BaseShape;
	}
}

namespace GFX::Probe
{
	class BaseProbe
	{
		Pipeline::Technique* technique = nullptr;
		bool compact = false;

		inline std::string MakeTag(const std::string& label) const noexcept;

	public:
		BaseProbe() = default;
		BaseProbe(const BaseProbe&) = default;
		BaseProbe& operator=(const BaseProbe&) = default;
		virtual ~BaseProbe() = default;

		constexpr void ReleaseTechnique() noexcept { technique = nullptr; }
		constexpr void SetCompact() noexcept { compact = true; }
		constexpr void SetNormal() noexcept { compact = false; }

		void SetTechnique(Pipeline::Technique* currentTechnique) noexcept;
		bool Visit(Data::CBuffer::DynamicCBuffer& buffer) const noexcept;
	};
}