#pragma once
#include "IBufferResource.h"

namespace GFX::Pipeline::Resource
{
	class DepthStencil : public IBufferResource
	{
		friend class Graphics;
		friend class RenderTarget;

	public:
		enum class Usage : bool { DepthStencil, DepthOnly };

	private:
		Usage usage;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;

		static constexpr DXGI_FORMAT UsageTypeless(Usage usage) noexcept;
		static constexpr DXGI_FORMAT UsageTyped(Usage usage) noexcept;

	public:
		DepthStencil(Graphics& gfx, Usage usage = Usage::DepthStencil);
		DepthStencil(Graphics& gfx, unsigned int width, unsigned int height, Usage usage = Usage::DepthStencil);
		virtual ~DepthStencil() = default;

		inline void Bind(Graphics& gfx) override { GetContext(gfx)->OMSetRenderTargets(0U, nullptr, depthStencilView.Get()); BindViewport(gfx); }
		inline void Clear(Graphics& gfx) noexcept override { GetContext(gfx)->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0U); }
	};
}