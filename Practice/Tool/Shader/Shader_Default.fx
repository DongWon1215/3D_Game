
// 전역변수들 : 컨스턴트테이블.
matrix		g_matWorld, g_matView, g_matProj;

texture		g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
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
	
};

// 정점에 행렬곱해서 스페이스를 변환시켜주는 작업.(함수)
// VS_MAIN(float3 vPosition : POSITION, float3 vNormal : NORMAL, long Color : COLOR)
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	// 월드변환 

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;	
}

// Out.vPosition.w로 Out.vPosition.xyzw 나눈다.(원근투영)
// 뷰포트 변환.
// 래스터라이즈(픽셀을 생성)


struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};


// 픽셀의 색을 결정하는 작업.(함수)
// vector PS_MAIN(PS_IN In) : COLOR
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);	

	return Out;	
}

technique Default_Technique
{
	pass Default_Rendering
	{
		AlphaBlendEnable = false;
		CullMode = none;
		FillMode = Solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaBlending
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		FillMode = Solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}













