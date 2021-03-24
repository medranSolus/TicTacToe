#include "Blender.h"
#include "GfxExceptionMacros.h"

namespace GFX::Resource
{
	Blender::Blender(Graphics& gfx, Type type) : type(type)
	{
		GFX_ENABLE_ALL(gfx);

		D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT{});
		auto& blendTarget = blendDesc.RenderTarget[0];
		switch (type)
		{
		case Type::Normal:
		{
			blendTarget.BlendEnable = TRUE;
			blendTarget.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA; // Maybe ONE
			blendTarget.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			break;
		}
		}
		GFX_THROW_FAILED(GetDevice(gfx)->CreateBlendState(&blendDesc, &state));
		SET_DEBUG_NAME_RID(state.Get());
	}
}