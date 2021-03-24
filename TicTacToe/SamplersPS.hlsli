/*	Sampler definitions for different types declared in code

	Possible type categories:
	{
		P - Point
		L - Linear
		A - Anisotropic
	}
	{
		B - Exceedeing clamp to border color
		W - Wrap coordinates
		R - Coordinates reflection
	}
*/

SamplerState splr_AW : register(s0);
SamplerState splr_LR : register(s1);
SamplerState splr_PW : register(s2);