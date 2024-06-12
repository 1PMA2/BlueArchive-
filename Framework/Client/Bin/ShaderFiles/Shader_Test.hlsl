
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_DepthTexture;
texture2D	g_BlurTextureX;
texture2D	g_BlurTextureY;

float g_Fade;

sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};

float m_TexW = 127.f;
float m_TexH = 163.f;

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

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAINX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector vBlur;
	float2 t = In.vTexUV;
	float2 uv = 0;

	float tu = 1.f / (m_TexW / 2.f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0);
		vBlur += Weight[6 + i] * g_BlurTextureX.Sample(DefaultSampler, uv);
	}

	vBlur /= Total;


	Out.vColor = vBlur;

	
	return Out;	
}

PS_OUT PS_MAINY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vBlur;
	float2 t = In.vTexUV;
	float2 uv = 0;

	float tv = 1.f / (m_TexH);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(0, tv * i);
		vBlur += Weight[6 + i] * g_BlurTextureY.Sample(DefaultSampler, uv);
	}

	vBlur /= Total;


	Out.vColor = vBlur;


	return Out;
}

PS_OUT PS_MAINXY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vBlur;
	float2 t = In.vTexUV;
	float2 uv = 0;

	float tu = 1.f / (m_TexW / 1.f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0);
		vBlur += Weight[6 + i] * g_BlurTextureX.Sample(DefaultSampler, uv);
	}

	vBlur /= Total;

	////
	Out.vColor = vBlur;

	vector vBlury;
	t = In.vTexUV;
	uv = 0;

	float tv = 1.f / (m_TexH / 1.f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(0, tv * i);
		vBlury += Weight[6 + i] * g_BlurTextureY.Sample(DefaultSampler, uv);
	}

	vBlury /= Total;


	Out.vColor = vBlur;



	return Out;
}


technique11 DefaultTechnique
{
	pass X
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINXY();
	}
	pass Y
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAINY();
	}
}