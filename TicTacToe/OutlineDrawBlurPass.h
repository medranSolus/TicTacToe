#pragma once
#include "QueuePass.h"
#include "ConstBufferExCache.h"

namespace GFX::Pipeline::RenderPass
{
	class OutlineDrawBlurPass : public Base::QueuePass
	{
		GfxResPtr<GFX::Resource::ConstBufferExPixelCache> outlineColorBuffer;

	public:
		OutlineDrawBlurPass(Graphics& gfx, const std::string& name, unsigned int width, unsigned int height);
		virtual ~OutlineDrawBlurPass() = default;

		void ShowWindow() noexcept;
		void Execute(Graphics& gfx) override;
	};
}