#include "SamplersPS.hlsli"

Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
	float4 color = tex.Sample(splr_AW, tc);
	clip(color.a - 0.0039f);
	color.a = 1.0f;
	return color;
}