
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_DepthTexture;

float g_Fade;


sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

float m_TexW = 1280.f;
float m_TexH = 720.f;

static const float Weight[13] =
{
	0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = float4(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAINX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 t  = In.vTexUV;
	float2 uv = 0;

	float tu = 1.f / m_TexW;

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0);
		Out.vColor += Weight[6 + i] * g_DiffuseTexture.Sample(DefaultSampler, uv);
	}

	Out.vColor /= Total;

	return Out;
}

PS_OUT PS_MAINY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 t = In.vTexUV;
	float2 uv = 0;

	float tu = 1.f / m_TexH;

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0);
		Out.vColor += Weight[6 + i] * g_DiffuseTexture.Sample(DefaultSampler, uv);
	}

	Out.vColor /= Total;

	return Out;
}

technique11 DefaultTechnique
{
	pass pass0
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINX();
	}
	pass pass1
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINY();
	}
}
