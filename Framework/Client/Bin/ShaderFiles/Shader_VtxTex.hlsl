
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D	g_DiffuseTexture;
texture2D	g_DepthTexture;

float g_Fade;
float g_Frame;
float g_Cost;
float g_UVx;
uint g_DNum;
int g_Dir;
int g_Warning;
bool g_Large;
sampler DefaultSampler = sampler_state 
{		
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (-1.f != g_Fade)
	{
		if(0.1f < Out.vColor.a)
			Out.vColor.a = g_Fade;
	}

	
	return Out;	
}

struct VS_OUT_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT_SOFT VS_MAIN_SOFT(VS_IN In)
{
	VS_OUT_SOFT		Out = (VS_OUT_SOFT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN_SOFT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

PS_OUT PS_MAIN_SOFT(PS_IN_SOFT In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	float2		vTexUV;

	/* -1, 1 => 1, -1 : 투영공간상의 위치. */
	vTexUV.x = In.vProjPos.x / In.vProjPos.w;
	vTexUV.y = In.vProjPos.y / In.vProjPos.w;

	/* 0, 0 => 1, 1 : 텍스쳐 유브이 좌표. */
	vTexUV.x = vTexUV.x * 0.5f + 0.5f;
	vTexUV.y = vTexUV.y * -0.5f + 0.5f;

	vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vTexUV);

	float		fViewZ = vDepthDesc.y * 300.f;

	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w);

	return Out;

}
//cost gauge

struct VS_IN_COST
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT_COST
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT_COST VS_COST(VS_IN_COST In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vTexUV.x = In.vTexUV.x * g_Cost;

	return Out;
}
//warning

VS_OUT VS_WARNING(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	if(0 < g_Dir)
		Out.vTexUV.x = In.vTexUV.x + g_UVx;
	else
		Out.vTexUV.x = In.vTexUV.x - g_UVx;

	return Out;
}

PS_OUT PS_WARNING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*if (0.5 < Out.vColor.a)
		Out.vColor.a = g_Fade;*/

	switch (g_Warning)
	{
	case 0: //중앙 글씨
		Out.vColor.b = 0.f;
		Out.vColor.r = 0.9f;
		Out.vColor.g = g_Fade;
		break;
	case 1:
		Out.vColor += 0.1f;
		if(0.5f < Out.vColor.a)
			Out.vColor.a = g_Fade;

		break;
	}


	return Out;
}

PS_OUT PS_SMOKE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vTexUV = In.vTexUV;

	float	fFrame = g_Frame;

	if (g_Large)
	{
		if (fFrame > 16.f)
			fFrame = 15.f;

		vTexUV.x = vTexUV.x * (1.f / 4.f) + ((uint)fFrame % 4) * (1.f / 4.f);
		vTexUV.y = vTexUV.y * (1.f / 4.f) + ((uint)fFrame / 4) * (1.f / 4.f);
		Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, vTexUV);
		if(Out.vColor.a < 0.01)
			discard;
		//.vColor.a = Out.vColor.r;

	}
	else
	{
		if (fFrame > 9.f)
			fFrame = 8.f;

		vTexUV.x = vTexUV.x * (1.f / 3.f) + ((uint)fFrame % 3) * (1.f / 3.f);
		vTexUV.y = vTexUV.y * (1.f / 3.f) + ((uint)fFrame / 3) * (1.f / 3.f);
		Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, vTexUV);

		Out.vColor.a = Out.vColor.r;
	}

	

	//Out.vFlag = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);


	/*if (0.2f < Out.vColor.r)
	{
		Out.vColor -= 0.3f;
	}*/

	return Out;
}


PS_OUT PS_NUM(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2	vTexUV = In.vTexUV;

	uint	iNum = g_DNum;

	
	vTexUV.x = vTexUV.x * (1.f / 5.f) + ((uint)iNum % 5) * (1.f / 5.f);
	vTexUV.y = vTexUV.y * (1.f / 2.f) + ((uint)iNum / 5) * (1.f / 2.f);

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, vTexUV);

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}
PS_OUT PS_EXP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

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
	pass SoftEffect
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_SOFT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SOFT();
	}
	pass CostGauge
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_COST();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Warning
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_WARNING();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_WARNING();
	}
	pass SMOKE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Z, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SMOKE();
	}
	pass EXP
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_EXP();
	}
	pass NUM
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NUM();
	}
}