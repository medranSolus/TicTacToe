#pragma once
#include "DepthStencil.h"

namespace GFX::Pipeline::Resource
{
	class RenderTarget : public IBufferResource
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView = nullptr;

	protected:
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;

		inline RenderTarget(Graphics& gfx, unsigned int width, unsigned int height, DXGI_FORMAT format) noexcept
			: IBufferResource(gfx, width, height), format(format) {}

		static Microsoft::WRL::ComPtr<ID3D11Texture2D> CreateTexture(Graphics& gfx, unsigned int width,
			unsigned int height, D3D11_TEXTURE2D_DESC& textureDesc, DXGI_FORMAT format);

		void InitializeTargetView(Graphics& gfx, D3D11_TEXTURE2D_DESC& textureDesc, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture);

	public:
		RenderTarget(Graphics& gfx, unsigned int width, unsigned int height, Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer, DXGI_FORMAT format);
		virtual ~RenderTarget() = default;

		inline void BindTarget(Graphics& gfx) { GetContext(gfx)->OMSetRenderTargets(1U, targetView.GetAddressOf(), nullptr); BindViewport(gfx); }
		inline void BindTarget(Graphics& gfx, DepthStencil& depthStencil) { GetContext(gfx)->OMSetRenderTargets(1U, targetView.GetAddressOf(), depthStencil.depthStencilView.Get()); BindViewport(gfx); }
		inline void Bind(Graphics& gfx) override { BindTarget(gfx); }

		inline void Clear(Graphics& gfx) noexcept override { Clear(gfx, { 0.0f, 0.0f, 0.0f, 0.0f }); }
		inline void Clear(Graphics& gfx, const Data::ColorFloat4& color) noexcept override { GetContext(gfx)->ClearRenderTargetView(targetView.Get(), reinterpret_cast<const FLOAT*>(&color.col)); }
	};
}