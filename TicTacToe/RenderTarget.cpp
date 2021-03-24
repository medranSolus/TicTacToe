#include "RenderTarget.h"
#include "Graphics.h"
#include "GfxExceptionMacros.h"

namespace GFX::Pipeline::Resource
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTarget::CreateTexture(Graphics& gfx, unsigned int width, unsigned int height,
		D3D11_TEXTURE2D_DESC& textureDesc, DXGI_FORMAT format)
	{
		GFX_ENABLE_ALL(gfx);

		textureDesc.Width = static_cast<UINT>(width);
		textureDesc.Height = static_cast<UINT>(height);
		textureDesc.ArraySize = 1U;
		textureDesc.MipLevels = 1U;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1U;
		textureDesc.SampleDesc.Quality = 0U;
		textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0U;
		textureDesc.MiscFlags = 0U;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
		GFX_THROW_FAILED(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &texture));

		return texture;
	}

	void RenderTarget::InitializeTargetView(Graphics& gfx, D3D11_TEXTURE2D_DESC& textureDesc, Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
	{
		GFX_ENABLE_ALL(gfx);

		D3D11_RENDER_TARGET_VIEW_DESC targetViewDesc = {};
		targetViewDesc.Format = textureDesc.Format;
		targetViewDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
		targetViewDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		GFX_THROW_FAILED(GetDevice(gfx)->CreateRenderTargetView(texture.Get(), &targetViewDesc, &targetView));
		SET_DEBUG_NAME(targetView.Get(), "RT" + std::to_string(GetWidth()) + "x" + std::to_string(GetHeight()) + "#" + std::to_string(format));
	}

	RenderTarget::RenderTarget(Graphics& gfx, unsigned int width, unsigned int height, Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer, DXGI_FORMAT format)
		: IBufferResource(gfx, width, height), format(format)
	{
		GFX_ENABLE_ALL(gfx);
		GFX_THROW_FAILED(GetDevice(gfx)->CreateRenderTargetView(backBuffer.Get(), nullptr, &targetView));
	}
}