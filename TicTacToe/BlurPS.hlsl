#include "SamplersPS.hlsli"
#include "GaussPB.hlsli"
#include "BlurDirectionPB.hlsli"

Texture2D tex : register(t0);

float4 main(float2 tc : TEXCOORD) : SV_TARGET
{
	float2 delta;
	if (cb_vertical)
		delta = float2(0.0f, 1.0f / cb_height);
	else
		delta = float2(1.0f / cb_width, 0.0f);

	float3 maxColor = 0.0f;
	float alpha = 0.0f;

	[unroll(MAX_RADIUS * 2 - 1)]
	for (int i = -cb_radius; i <= cb_radius; ++i)
	{
		const float4 color = tex.Sample(splr_LR, tc + delta * i);
		alpha += color.a * cb_coefficients[abs(i)];
		maxColor = max(maxColor, color.rgb);
	}
	return float4(maxColor, alpha);
}