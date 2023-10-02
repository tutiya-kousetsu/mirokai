#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);   // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);        // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0 : SV_TARGET0;
};

PSOutput main(VSOutput input)
{
	PSOutput output;
	float4 colortex0 = tex0.Sample(smp, input.uv);
	/*if (fmod(input.uv.y, 0.1f) < 0.05f) {
		 return tex0.Sample(smp, input.uv);
	}*/
	//���m�N��
	float Y = 0.299f * colortex0.r + 0.587f * colortex0.b + 0.114f * colortex0.b;

	colortex0.r = Y;
	colortex0.g = Y;
	colortex0.b = Y;
	output.target0 = colortex0;
	return output;
}