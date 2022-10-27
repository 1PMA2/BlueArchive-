#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_FlagTexture;

float g_Frame;
int g_Weapon;

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

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vTexUV = In.vTexUV;

	float	fFrame = g_Frame;

	if (2 != g_Weapon)
	{
		if (fFrame > 4.f)
			fFrame = 3.f;

		vTexUV.x = vTexUV.x * (1.f / 2.f) + ((uint)fFrame % 2) * (1.f / 2.f);
		vTexUV.y = vTexUV.y * (1.f / 2.f) + ((uint)fFrame / 2) * (1.f / 2.f);
	}
	else
	{
		if (fFrame > 9.f)
			fFrame = 8.f;

		vTexUV.x = vTexUV.x * (1.f / 3.f) + ((uint)fFrame % 3) * (1.f / 3.f);
		vTexUV.y = vTexUV.y * (1.f / 3.f) + ((uint)fFrame / 3) * (1.f / 3.f);
	}


	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexUV);

	if (Out.vDiffuse.r < 0.1f)
		discard;

	//Out.vFlag = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);


	Out.vDiffuse.r = 255.f / 255.f;
	Out.vDiffuse.g = 255.f / 255.f;
	Out.vDiffuse.b = 20.f / 255.f;

	return Out;
}

PS_OUT PS_BULLET(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = Out.vDiffuse.g;

	if (Out.vDiffuse.r < 0.1f)
		discard;


	Out.vDiffuse.r = 210.f / 255.f;
	Out.vDiffuse.g = 210.f / 255.f;
	Out.vDiffuse.b = 20.f / 255.f;

	return Out;
}

PS_OUT PS_SMOKE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vTexUV = In.vTexUV;

	float	fFrame = g_Frame;

	if (fFrame > 9.f)
		fFrame = 8.f;

	vTexUV.x = vTexUV.x * (1.f / 3.f) + ((uint)fFrame % 3) * (1.f / 3.f);
	vTexUV.y = vTexUV.y * (1.f / 3.f) + ((uint)fFrame / 3) * (1.f / 3.f);


	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexUV);

	if (Out.vDiffuse.r < 0.1f)
		discard;

	//Out.vFlag = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);


	if (0.2f < Out.vDiffuse.r)
	{
		Out.vDiffuse.a = 0.6f;
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

		VertexShader = compile vs_5_0 VS_MAIN();
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

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SMOKE();
	}

}