#include "TransformVB.hlsli"

struct VSOut
{
	float2 tc : TEXCOORD;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 tc : TEXCOORD)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f), cb_transformViewProjection);
	vso.tc = tc;
	return vso;
}