
matrix		g_matWorld, g_matView, g_matProj;

texture		g_SourTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해ㅣ.

sampler	SourSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_SourTexture;
};

texture		g_DestTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해ㅣ.

sampler	DestSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_DestTexture;
};

texture		g_FilterTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해ㅣ.

sampler	FilterSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_FilterTexture;
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
	float4	vWorldPos : TEXCOORD1;	
	float4	vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;		

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	Out.vProjPos = vPosition;

	return Out;	
}

struct PS_IN
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
	float4	vProjPos : TEXCOORD2;
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

	vector		vSourDiffuse = tex2D(SourSampler, In.vTexUV * 10.f);
	vector		vDestDiffuse = tex2D(DestSampler, In.vTexUV * 10.f);

	
	vector		vFilter = tex2D(FilterSampler, In.vTexUV);
	
	Out.vDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter);
	Out.vDiffuse.a = 1.f;

	// -1 ~ 1 => 0 ~ 1

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);



	return Out;	
}


technique Default_Technique
{
	pass DefaultRendering 
	{
		fillmode = solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	
}