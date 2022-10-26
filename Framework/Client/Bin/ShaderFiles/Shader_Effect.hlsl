#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_BlurTexture;

float g_Frame;
int g_Weapon;

float m_TexW = 256.f;
float m_TexH = 256.f;

static const float Weight[13] =
{
	0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler BlurSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
};



struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	float			fFrame = g_Frame;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUT VS_MUZZLE(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	float			fFrame = g_Frame;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	if (2 != g_Weapon)
	{
		if (fFrame > 4.f)
			fFrame = 3.f;

		Out.vTexUV.x = In.vTexUV.x * (1.f / 2.f) + ((uint)fFrame % 2) * (1.f / 2.f);
		Out.vTexUV.y = In.vTexUV.y * (1.f / 2.f) + ((uint)fFrame / 2) * (1.f / 2.f);
	}
	else
	{
		if (fFrame > 9.f)
			fFrame = 8.f;

		Out.vTexUV.x = In.vTexUV.x * (1.f / 3.f) + ((uint)fFrame % 3) * (1.f / 3.f);
		Out.vTexUV.y = In.vTexUV.y * (1.f / 3.f) + ((uint)fFrame / 3) * (1.f / 3.f);
	}

	return Out;
}

VS_OUT VS_SMOKE(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	float			fFrame = g_Frame;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vTexUV = In.vTexUV;

	if (fFrame > 9.f)
		fFrame = 8.f;

	Out.vTexUV.x = In.vTexUV.x * (1.f / 3.f) + ((uint)fFrame % 3) * (1.f / 3.f);
	Out.vTexUV.y = In.vTexUV.y * (1.f / 3.f) + ((uint)fFrame / 3) * (1.f / 3.f);

	return Out;
}


struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;


	Out.vColor.a = Out.vColor.g;

	//vector vBlur;
	//float2 t = In.vTexUV;
	//float2 uv = 0;

	//float tu = 1.f / (m_TexW / 2.f);

	//for (int i = -6; i < 6; ++i)
	//{
	//	uv = t + float2(tu * i, 0);
	//	vBlur += Weight[6 + i] * g_BlurTexture.Sample(PointSampler, uv);
	//	vBlur.a = vBlur.r;
	//}

	//vBlur /= Total;

	//////
	//Out.vColor = vBlur;

	Out.vColor.r = 255.f / 255.f;
	Out.vColor.g = 255.f / 255.f;
	Out.vColor.b = 20.f / 255.f;

	return Out;
}

PS_OUT PS_BULLET(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	Out.vColor.a = Out.vColor.g;

	Out.vColor.r = 210.f / 255.f;
	Out.vColor.g = 210.f / 255.f;
	Out.vColor.b = 100.f / 255.f;

	return Out;
}

PS_OUT PS_SMOKE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	Out.vColor.a = Out.vColor.g;

	if (0.2f < Out.vColor.r)
	{
		Out.vColor.a = 0.6f;
	}

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass MUZZLE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MUZZLE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass BULLET
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BULLET();
	}
	pass Smoke
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_SMOKE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SMOKE();
	}

}