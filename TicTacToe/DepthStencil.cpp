#include "DepthStencil.h"
#include "Graphics.h"
#include "GfxExceptionMacros.h"

namespace GFX::Pipeline::Resource
{
	constexpr DXGI_FORMAT DepthStencil::UsageTypeless(Usage usage) noexcept
	{
		switch (usage)
		{
		default:
		case Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		case Usage::DepthOnly:
			return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		}
	}

	constexpr DXGI_FORMAT DepthStencil::UsageTyped(Usage usage) noexcept
	{
		switch (usage)
		{
		default:
		case Usage::DepthStencil:
			return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		case Usage::DepthOnly:
			return DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
		}
	}

	DepthStencil::DepthStencil(Graphics& gfx, Usage usage)
		: DepthStencil(gfx, gfx.GetWidth(), gfx.GetHeight(), usage) {}

	DepthStencil::DepthStencil(Graphics& gfx, unsigned int width, unsigned int height, Usage usage)
		: IBufferResource(gfx, width, height), usage(usage)
	{
		GFX_ENABLE_ALL(gfx);

		D3D11_TEXTURE2D_DESC depthTexDesc = { 0 };
		depthTexDesc.Width = static_cast<UINT>(width);
		depthTexDesc.Height = static_cast<UINT>(height);
		depthTexDesc.MipLevels = 0U;
		depthTexDesc.ArraySize = 1U;
		depthTexDesc.Format = UsageTypeless(usage);
		depthTexDesc.SampleDesc.Count = 1U; // Antialiasing stuff
		depthTexDesc.SampleDesc.Quality = 0U;
		depthTexDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture = nullptr;
		GFX_THROW_FAILED(GetDevice(gfx)->CreateTexture2D(&depthTexDesc, nullptr, &depthTexture));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
		depthViewDesc.Format = UsageTyped(usage);
		depthViewDesc.Flags = 0U;
		depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		depthViewDesc.Texture2D.MipSlice = 0U;
		GFX_THROW_FAILED(GetDevice(gfx)->CreateDepthStencilView(depthTexture.Get(), &depthViewDesc, &depthStencilView));
		SET_DEBUG_NAME(depthStencilView.Get(), "DS" + std::to_string(GetWidth()) + "x" + std::to_string(GetHeight()) + "#" + std::to_string(static_cast<bool>(usage)));
	}
}