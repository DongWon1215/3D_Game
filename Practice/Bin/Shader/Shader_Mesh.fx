
// 전역변수들 : 컨스턴트테이블.
matrix		g_matWorld, g_matView, g_matProj;

texture		g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_DiffuseTexture;
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;	
	float4	vProjPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	// 월드변환 

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;

	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = vPosition;

	return Out;	
}



struct PS_IN
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector  vDiffuse : COLOR0;	
	vector  vNormal : COLOR1;
	vector  vDepth : COLOR2;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);

	return Out;	
}

technique Default_Technique
{
	pass GroudShading // 명암연산을 버텍스 단계에서 수행한다.
	{
		AlphaTestEnable = true;
		AlphaRef = 0;
		AlphaFunc = Greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//pass PhongShading // 명암연산을 픽셀 단계에서 수행한다.
	//{
	//	AlphaTestEnable = true;
	//	AlphaRef = 0;
	//	AlphaFunc = Greater;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}
}













