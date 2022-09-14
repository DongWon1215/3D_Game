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
	float3	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float3	vTexUV : TEXCOORD0;
	
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}



struct PS_IN
{
	float4	vPosition : POSITION;
	float3	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(DiffuseSampler, In.vTexUV);	

	//Out.vColor = vector(0.f, 0.f, 0.f, 1.f);

	return Out;	
}

technique Default_Technique
{
	pass Default_Rendering
	{
		CullMode = cw;
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}












