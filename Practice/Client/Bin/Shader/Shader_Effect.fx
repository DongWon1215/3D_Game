
// 전역변수들 : 컨스턴트테이블.
matrix		g_matWorld, g_matView, g_matProj;

float g_LifeTime;

texture		g_DepthTexture;

sampler	DepthSampler = sampler_state
{	
	texture = g_DepthTexture;
};

texture		g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
	texture = g_DiffuseTexture;
};

// hlsl을 작성하기위한 파일.

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
	
};

VS_OUT VS_MAIN_DEFAULT(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	vector		vPosition;
	// 월드변환 
	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);

	// 뷰변환
	vPosition = mul(vPosition, g_matView);

	// 투영 사전 변환
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = vPosition;

	return Out;	
}


struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);	

	float2	vTexUV;

	vTexUV.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
	vTexUV.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

	float LifeTime = saturate(g_LifeTime);

	vector	vDepthInfo = tex2D(DepthSampler, vTexUV);

	float	fOldViewZ = vDepthInfo.y * 500.0f;
	float	fViewZ = In.vProjPos.w;

	Out.vColor.a = Out.vColor.a * saturate(fOldViewZ - fViewZ) * 2.f * LifeTime;

	return Out;	
}

technique Default_Technique
{
	pass Default_Rendering
	{		
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;		

		//AlphaTestEnable = true;
		//AlphaRef = 50;
		//AlphaFunc = Greater;

		VertexShader = compile vs_3_0 VS_MAIN_DEFAULT();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}













